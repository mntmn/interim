#include "alloc.h"
#include <string.h>
#include "stream.h"

void* byte_heap;
Cell* cell_heap;

size_t cells_used;
size_t byte_heap_used;

Cell** free_list;
size_t free_list_avail;
size_t free_list_consumed;

Cell oom_cell;

//#define DEBUG_GC

// TODO define in machine specs
#define MAX_CELLS 100000
#define MAX_BYTE_HEAP 8*1024

static struct MemStats mem_stats;

void init_allocator() {
  unsigned int cell_mem_reserved = MAX_CELLS*sizeof(Cell);

  oom_cell.tag = TAG_ERROR;
  oom_cell.ar.value = ERR_OUT_OF_MEMORY;

  byte_heap_used = 0;
  cells_used = 0;
  free_list_avail = 0;
  free_list_consumed = 0;
  byte_heap = NULL;

  cell_heap = malloc(cell_mem_reserved);
  printf("\r\n[alloc] cell heap at %p, %d bytes reserved\r\n",cell_heap,cell_mem_reserved);
  memset(cell_heap,0,cell_mem_reserved);

  free_list = malloc(MAX_CELLS*sizeof(Cell*));

  //printf("[alloc] initialized.\r\n");
}

Cell* get_cell_heap() {
  return cell_heap;
}

// FIXME header?
env_t* get_global_env();

Cell* cell_alloc() {
  //printf("alloc %d\r\n",cells_used);

  /*if (free_list_avail<free_list_consumed) {
    // try gc
    // FIXME need access to current frame
    collect_garbage(get_global_env());
  }*/
  
  if (free_list_avail>free_list_consumed) {
    // serve from free list
    int idx = free_list_consumed;
    Cell* res = free_list[idx];
    free_list_consumed++;
    //printf("++ cell_alloc: recycled %d (%p)\r\n",idx,res);
    return res;
  } else {
    Cell* res = &cell_heap[cells_used];
    cells_used++;
    if (cells_used>MAX_CELLS) {
      printf("!! cell_alloc failed, MAX_CELLS used.\n");
      exit(1);
    }
    //printf("++ cell_alloc: %d \r\n",cells_used);
    return res;
  }
}

void* bytes_alloc(int num_bytes) {
//#ifdef SLEDGE_MALLOC
  void* new_mem = malloc(num_bytes);
  //printf("bytes_alloc: %p +%d\r\n",new_mem,num_bytes);
  memset(new_mem, 0, num_bytes);
  return new_mem;
//#endif
    /*void* new_mem = byte_heap + byte_heap_used;
  if (byte_heap_used + num_bytes < MAX_BYTE_HEAP) {
    byte_heap_used += num_bytes;
    //printf("++ byte_alloc: %d (+%d) \r\n",byte_heap_used,num_bytes);
    return new_mem;
  } else {
    printf("~~ bytes_alloc: out of memory: %d (%d)\r\n",byte_heap,byte_heap_used);
    exit(1);
    return NULL;
    }*/
}

void mark_tree(Cell* c) {
  if (!c) {
    //printf("~! warning: mark_tree encountered NULL cell.\n");
    return;
  }

  if (!(c->tag & TAG_MARK)) {
    /*char buf[80];
    lisp_write(c, buf, 79);
    printf("~~ marking live: %p %s\n",c,buf);*/
    
    if (c->tag == TAG_CONS) {
      if (c->ar.addr) mark_tree((Cell*)c->ar.addr);
      if (c->dr.next) mark_tree((Cell*)c->dr.next);
    }
    else if (c->tag == TAG_SYM) {
      // TODO: mark bytes in heap
      // also for STR, BYTES
    }
    else if (c->tag == TAG_LAMBDA) {
      /*static char buf[512];
      lisp_write((Cell*)c->ar.addr, buf, 511);
      printf("~~ mark lambda args: %s\n",buf);*/
      mark_tree((Cell*)c->ar.addr); // function arguments
      //mark_tree((Cell*)c->dr.next); // function body

      // TODO: mark compiled code / free unused compiled code
      // -- keep all compiled blobs in a list
    }
    else if (c->tag == TAG_BUILTIN) {
      mark_tree((Cell*)c->dr.next); // builtin signature
    }
    else if (c->tag == TAG_STREAM) {
      Stream* s = (Stream*)c->ar.addr;
      if (s) {
        mark_tree(s->path);
      }
    }
    else if (c->tag == TAG_FS) {
      Filesystem* fs = (Filesystem*)c->dr.next;
      if (fs) {
        mark_tree(fs->mount_point);
        mark_tree(fs->open_fn);
        mark_tree(fs->close_fn);
        mark_tree(fs->read_fn);
        mark_tree(fs->write_fn);
        mark_tree(fs->delete_fn);
        mark_tree(fs->mmap_fn);
      }
    }
    else if (c->tag == TAG_VEC || c->tag == TAG_STRUCT || c->tag == TAG_STRUCT_DEF) {
      int i=0;
      int sz=c->dr.size;
      Cell** elements=c->ar.addr;
      for (i=0; i<sz; i++) {
        mark_tree(elements[i]);
      }
    }
    
    c->tag |= TAG_MARK;
  }
}

static Cell* _symbols_list;
void list_symbols_iter(const char *key, void *value, const void *obj)
{
  env_entry* e = (env_entry*)value;
  _symbols_list = alloc_cons(alloc_sym(e->name), _symbols_list);
}
Cell* list_symbols(env_t* global_env) {
  _symbols_list = alloc_nil();
  sm_enum(global_env, list_symbols_iter, NULL);
  return _symbols_list;
}

void collect_garbage_iter(const char *key, void *value, const void *obj)
{
  env_entry* e = (env_entry*)value;
  //printf("key: %s value: %s\n", key, value);
  mark_tree(e->cell);
}

Cell* collect_garbage(env_t* global_env, void* stack_end, void* stack_pointer) {
  // mark

  // check all symbols in the environment
  // and look where they lead (cons trees, bytes, strings)
  // mark all of them as usable

  //printf("[gc] stack at: %p, stack end: %p\r\n",stack_pointer,stack_end);

  int gc=0, i;
#ifdef DEBUG_GC
  int highwater=0;
#endif

  int sw_state = 0;
  jit_word_t* a;
  for (a=(jit_word_t*)stack_end; a>=(jit_word_t*)stack_pointer; a--) {
    jit_word_t item = *a;
    jit_word_t next_item = *(a-1);
    if (next_item==STACK_FRAME_MARKER) {
      sw_state=2;
    } else {
      if (sw_state==2) {
        sw_state=1;
      } else if (sw_state==1) {
        // FIXME total hack, need type information for stack
        // maybe type/signature byte frame header?
        if ((Cell*)item>cell_heap) {
          mark_tree((Cell*)item);
        }
      }
    }

    /*if (sw_state==2) {
      printf(KMAG "%p: 0x%08lx\r\n" KWHT,a,item);
    }
    else if (sw_state==1) {
      printf(KCYN "%p: 0x%08lx\r\n" KWHT,a,item);
    }
    else {
      printf(KWHT "%p: 0x%08lx\r\n" KWHT,a,item);
      }*/
  }
  //printf("[gc] stack walk complete -------------------------------\r\n");

  sm_enum(global_env, collect_garbage_iter, NULL);
  mark_tree(get_fs_list());

  /*for (env_entry* e=global_env; e != NULL; e=e->hh.next) {
    //printf("env entry: %s pointing to %p\n",e->name,e->cell);
    if (!e->cell) {
      //printf("~! warning: NULL env entry %s.\n",e->name);
    }
    mark_tree(e->cell);
  }*/

  // sweep -- free all things that are not marked

  free_list_avail = 0;
  free_list_consumed = 0;

#ifdef DEBUG_GC
  printf("\e[1;1H\e[2J");
  printf("~~ cell memory: ");
#endif
  
  for (i=0; i<cells_used; i++) {
    Cell* c = &cell_heap[i];
    // FIXME: we cannot free LAMBDAS currently
    // because nobody points to anonymous closures.
    // this has to be fixed by introducing metadata to their callers. (?)
    if (!(c->tag & TAG_MARK) && c->tag!=TAG_LAMBDA) {
      
#ifdef DEBUG_GC
      printf(".");
#endif
      if (c->tag == TAG_BYTES || c->tag == TAG_STR) {
        //free(c->ar.addr);
      }
      c->tag = TAG_FREED;
      
      free_list[free_list_avail] = c;
      free_list_avail++;
      gc++;
    } else {
#ifdef DEBUG_GC
      highwater = i;
      printf("o");
#endif
    }
    // unset mark bit
    cell_heap[i].tag &= ~TAG_MARK;
  }
  
  //printf("[gc] highwater %d fl_avail %d \r\n",highwater,free_list_avail);

  // FIXME on x64, this line causes corruption over time 
  //cells_used = highwater+1;
  
#ifdef DEBUG_GC
  printf("\n\n");
  printf("~~ %d of %d cells were garbage.\r\n",gc,cells_used);
#endif
  
  //printf("-- %d high water mark.\n\n",cells_used);

  return alloc_int(gc);
}

void* cell_realloc(void* old_addr, unsigned int old_size, unsigned int num_bytes) {
  // FIXME
  //cell_free(old_addr, old_size);
  void* new = bytes_alloc(num_bytes+1);
  memcpy(new, old_addr, old_size);
  return new;
}

MemStats* alloc_stats(void) {
  mem_stats.byte_heap_used = byte_heap_used;
  mem_stats.byte_heap_max = MAX_BYTE_HEAP;
  mem_stats.cells_used = cells_used;
  mem_stats.cells_max = MAX_CELLS;
  return &mem_stats;
}

Cell* alloc_cons(Cell* ar, Cell* dr) {
  //printf("alloc_cons: ar %p dr %p\n",ar,dr);
  Cell* cons = cell_alloc();
  cons->tag = TAG_CONS;
  cons->ar.addr = ar; //?alloc_clone(ar):ar;
  cons->dr.next = dr; //?alloc_clone(dr):dr;
  return cons;
}

Cell* alloc_list(Cell** items, int num) {
  Cell* list = alloc_nil();
  int i;
  for (i=num-1; i>=0; i--) {
    list = alloc_cons(items[i], list);
  }
  return list;
}

//extern void uart_puts(char* str);
extern void memdump(jit_word_t start, size_t len,int raw);

Cell* alloc_sym(char* str) {
  Cell* sym = cell_alloc();
  //printf("++ alloc sym at %p %p %d\r\n",sym,sym->ar.addr,sym->size);
  
  sym->tag = TAG_SYM;
  if (str) {
    int sz = strlen(str)+1;
    sym->dr.size = sz;
    //printf("alloc_sym: %s (%d)\r\n",str,sz);
    //memdump(sym,sizeof(Cell),0);
    
    sym->ar.addr = bytes_alloc(sz+1);

    //memdump(sym,sizeof(Cell),0);
    
    memcpy(sym->ar.addr, str, sz);
    
    //memdump(sym,sizeof(Cell),0);
  } else {
    sym->ar.addr = 0;
    sym->dr.size = 0;
  }
  return sym;
}

Cell* alloc_int(int i) {
  //printf("++ alloc_int %d\r\n",i);
  Cell* num = cell_alloc();
  num->tag = TAG_INT;
  num->ar.value = i;
  return num;
}

Cell* alloc_num_bytes(unsigned int num_bytes) {
  Cell* cell = cell_alloc();
  cell->ar.addr = bytes_alloc(num_bytes+1); // 1 zeroed byte more to defeat clib-str overflows
  cell->tag = TAG_BYTES;
  cell->dr.size = num_bytes;
  return cell;
}

Cell* alloc_bytes(void) {
  return alloc_num_bytes(SYM_INIT_BUFFER_SIZE);
}

Cell* alloc_num_string(unsigned int num_bytes) {
  Cell* cell = cell_alloc();
  cell->ar.addr = bytes_alloc(num_bytes+1); // 1 zeroed byte more to defeat clib-str overflows
  cell->tag = TAG_STR;
  cell->dr.size = num_bytes;
  return cell;
}

Cell* alloc_substr(Cell* str, unsigned int from, unsigned int len) {
  Cell* cell;

  if (!str) return alloc_string_copy("");
  if (str->tag!=TAG_BYTES && str->tag!=TAG_STR) return alloc_string_copy("");

  //printf("substr %s %d %d\n",str->ar.addr,from,len);
  if (from>=str->dr.size) from=str->dr.size-1;
  if (len+from>str->dr.size) len=str->dr.size-from; // FIXME TEST
  
  cell = cell_alloc();
  cell->ar.addr = bytes_alloc(len+1); // 1 zeroed byte more to defeat clib-str overflows
  cell->tag = TAG_STR;
  cell->dr.size = len;
  memcpy(cell->ar.addr, (uint8_t*)str->ar.addr+from, len);
  return cell;
}

Cell* alloc_string(void) {
  return alloc_num_string(SYM_INIT_BUFFER_SIZE);
}

Cell* alloc_string_copy(char* str) {
  Cell* cell = cell_alloc();
  cell->ar.addr = bytes_alloc(strlen(str)+1);
  strcpy(cell->ar.addr, str);
  cell->tag = TAG_STR;
  cell->dr.size = strlen(str)+1;
  return cell;
}

Cell* alloc_string_from_bytes(Cell* bytes) {
  Cell* cell;
  if (!bytes) return alloc_string_copy("");
  if (bytes->tag!=TAG_BYTES && bytes->tag!=TAG_STR) return alloc_string_copy("");
  if (bytes->dr.size<1) return alloc_string_copy("");
  
  cell = cell_alloc();
  cell->ar.addr = bytes_alloc(bytes->dr.size+1);
  memcpy(cell->ar.addr, bytes->ar.addr, bytes->dr.size);
  ((char*)cell->ar.addr)[bytes->dr.size]=0;
  cell->tag = TAG_STR;
  cell->dr.size = bytes->dr.size+1;
  return cell;
}

Cell* alloc_concat(Cell* str1, Cell* str2) {
  Cell* cell;
  int size1, size2, newsize;
  
  if (!str1 || !str2) return alloc_string_copy("");
  if (str1->tag!=TAG_BYTES && str1->tag!=TAG_STR) return alloc_string_copy("");
  if (str2->tag!=TAG_BYTES && str2->tag!=TAG_STR) return alloc_string_copy("");
  
  cell = cell_alloc();

  size1 = strlen(str1->ar.addr); // ,str2->size
  size2 = strlen(str2->ar.addr);
  newsize = size1+size2+1;
  cell->ar.addr = bytes_alloc(newsize+1);
  cell->dr.size = newsize;

  strncpy(cell->ar.addr, str1->ar.addr, size1);
  strncpy(cell->ar.addr+size1, str2->ar.addr, 1+cell->dr.size-size1);
  ((char*)cell->ar.addr)[newsize]=0;
  cell->tag = TAG_STR;
  cell->dr.size = newsize;
  return cell;
}

Cell* alloc_builtin(unsigned int b, Cell* signature) {
  Cell* num = cell_alloc();
  num->tag = TAG_BUILTIN;
  num->ar.value = b;
  num->dr.next = signature;
  return num;
}

Cell* alloc_lambda(Cell* args) {
  Cell* l = cell_alloc();
  l->tag = TAG_LAMBDA;
  l->ar.addr = args; // arguments
  //l->dr.next = cdr(def); // body
  return l;
}

Cell* alloc_nil(void) {
  return alloc_cons(0,0);
}

Cell* alloc_vector(int size) {
  Cell* c = cell_alloc();
  c->tag = TAG_VEC;
  c->ar.addr = malloc(size * sizeof(void*));
  c->dr.size = size;
  return c;
}

Cell* alloc_struct_def(int size) {
  Cell* c = cell_alloc();
  c->tag = TAG_STRUCT_DEF;
  c->ar.addr = malloc(size * sizeof(void*));
  c->dr.size = size;
  return c;
}

Cell* alloc_struct(Cell* struct_def) {
  Cell** elements;
  Cell** def_elements;
  int num_fields = struct_def->dr.size/2;
  int i;
  Cell* result = alloc_vector(num_fields+1);
  def_elements = (Cell**)struct_def->ar.addr;
  elements = (Cell**)result->ar.addr;

  // element zero points to the structure definition
  elements[0] = struct_def;
  
  for (i=0; i<num_fields; i++) {
    elements[i+1] = alloc_clone(def_elements[i*2+1+1]);
  }
  result->tag = TAG_STRUCT;

  return result;
}

int is_nil(Cell* c) {
  return (c==NULL || (c->ar.addr==NULL && c->dr.next==NULL));
}

Cell* alloc_error(unsigned int code) {
  Cell* c = cell_alloc();
  c->tag = TAG_ERROR;
  c->ar.value = code;
  c->dr.next = 0;
  return c;
}

Cell* alloc_clone(Cell* orig) {
  Cell* clone;
  if (!orig) return 0;

  clone = cell_alloc();
  clone->tag  = orig->tag;
  clone->ar.addr = 0;
  clone->dr.next = 0;
  clone->dr.size = orig->dr.size;

  //printf("cloning a %d (value %d)\n",orig->tag,orig->ar.value);
  
  if (orig->tag == TAG_SYM || orig->tag == TAG_STR || orig->tag == TAG_BYTES) {
    clone->ar.addr = bytes_alloc(orig->dr.size+1);
    memcpy(clone->ar.addr, orig->ar.addr, orig->dr.size);
  /*} else if (orig->tag == TAG_BYTES) {
    clone->ar.addr = bytes_alloc(orig->dr.size);
    memcpy(clone->ar.addr, orig->ar.addr, orig->dr.size);*/
  } else if (orig->tag == TAG_CONS) {
    if (orig->ar.addr) {
      clone->ar.addr = alloc_clone(orig->ar.addr);
    }
    if (orig->dr.next) {
      clone->dr.next = alloc_clone(orig->dr.next);
    }
  } else {
    clone->ar.addr = orig->ar.addr;
    clone->dr.next = orig->dr.next;
  }
  return clone;
}
