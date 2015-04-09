#include "alloc.h"
#include <malloc.h>
#include <stdint.h>

void* byte_heap;
Cell* cell_heap;

uint32_t cells_used;
uint32_t byte_heap_used;

Cell** free_list;
uint32_t free_list_avail;
uint32_t free_list_consumed;

Cell oom_cell;

#define MAX_CELLS 5000000
#define MAX_BYTE_HEAP 1024*1024*128

static struct MemStats mem_stats;

void init_allocator() {
  oom_cell.tag = TAG_ERROR;
  oom_cell.value = ERR_OUT_OF_MEMORY;

  byte_heap_used = 0;
  cells_used = 0;
  free_list_avail = 0;
  free_list_consumed = 0;
  byte_heap = NULL;

  uint32_t cell_mem_reserved = MAX_CELLS * sizeof(Cell);
  cell_heap = malloc(cell_mem_reserved);
  printf("\r\n++ cell heap at %p, %d bytes reserved\r\n",cell_heap,cell_mem_reserved);
  memset(cell_heap,0,cell_mem_reserved);

  free_list = malloc(MAX_CELLS*sizeof(Cell*));
  
  //byte_heap = malloc(MAX_BYTE_HEAP);
}

Cell* cell_alloc() {
  if (free_list_avail>free_list_consumed) {
    // serve from free list
    int idx = free_list_consumed;
    free_list_consumed++;
    Cell* res = free_list[idx];
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
    
    c->tag |= TAG_MARK;
    
    if (c->tag & TAG_CONS) {
      if (c->addr) mark_tree((Cell*)c->addr);
      if (c->next) mark_tree((Cell*)c->next);
    }
    else if (c->tag & TAG_SYM) {
      // TODO: mark bytes in heap
      // also for STR, BYTES
    }
    else if (c->tag & TAG_LAMBDA) {
      /*static char buf[512];
      lisp_write((Cell*)c->addr, buf, 511);
      printf("~~ mark lambda args: %s\n",buf);*/
      mark_tree((Cell*)c->addr); // function arguments
    }
  }
}

int collect_garbage(env_entry* global_env) {
  // mark

  // check all symbols in the environment
  // and look where they lead (cons trees, bytes, strings)
  // mark all of them as usable

  for (env_entry* e=global_env; e != NULL; e=e->hh.next) {
    //printf("env entry: %s pointing to %p\n",e->name,e->cell);
    if (!e->cell) {
      //printf("~! warning: NULL env entry %s.\n",e->name);
    }
    mark_tree(e->cell);
  }

  // sweep -- free all things that are not marked

  int gc = 0;
  char buf[300];

  free_list_avail = 0;
  free_list_consumed = 0;

#ifdef DEBUG_GC
  printf("\e[1;1H\e[2J");
  printf("~~ cell memory: ");
#endif
  
  for (int i=0; i<cells_used; i++) {
    Cell* c = &cell_heap[i];
    // FIXME: we cannot free LAMBDAS currently
    // because nobody points to anonymous closures.
    // this has to be fixed by introducing metadata to their callers. (?)
    if (!(c->tag & TAG_MARK) && c->tag!=TAG_LAMBDA) {
      
#ifdef DEBUG_GC
      printf(".");
#endif
      if (c->tag == TAG_BYTES || c->tag == TAG_STR) {
        free(c->addr);
      }
      c->tag = TAG_FREED;
      
      free_list[free_list_avail] = c;
      free_list_avail++;
      gc++;
    } else {
      
#ifdef DEBUG_GC
      printf("o");
#endif
    }
    // unset mark bit
    cell_heap[i].tag &= ~TAG_MARK;
  }
  
#ifdef DEBUG_GC
  printf("\n\n");
  printf("~~ %d of %d cells were garbage.\r\n",gc,cells_used);
#endif
  
  //printf("-- %d high water mark.\n\n",cells_used);

  return 0;
}

void* cell_realloc(void* old_addr, unsigned int old_size, unsigned int num_bytes) {
  // FIXME
  //cell_free(old_addr, old_size);
  void* new = bytes_alloc(num_bytes+1);
  memcpy(new, old_addr, old_size);
  return new;
}

MemStats* alloc_stats() {
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
  cons->addr = ar?alloc_clone(ar):ar;
  cons->next = dr?alloc_clone(dr):dr;
  return cons;
}

//extern void uart_puts(char* str);
extern void memdump(jit_word_t start,uint32_t len,int raw);

Cell* alloc_sym(char* str) {
  Cell* sym = cell_alloc();
  //printf("++ alloc sym at %p %p %d\r\n",sym,sym->addr,sym->size);
  
  sym->tag = TAG_SYM;
  if (str) {
    int sz = strlen(str)+1;
    sym->size = sz;
    //printf("alloc_sym: %s (%d)\r\n",str,sz);
    //memdump(sym,sizeof(Cell),0);
    
    sym->addr = bytes_alloc(sz+1);

    //memdump(sym,sizeof(Cell),0);
    
    memcpy(sym->addr, str, sz);
    
    //memdump(sym,sizeof(Cell),0);
  } else {
    sym->addr = 0;
    sym->size = 0;
  }
  return sym;
}

Cell* alloc_int(int i) {
  //printf("++ alloc_int %d\r\n",i);
  Cell* num = cell_alloc();
  num->tag = TAG_INT;
  num->value = i;
  return num;
}

Cell* alloc_num_bytes(unsigned int num_bytes) {
  Cell* cell = cell_alloc();
  cell->addr = bytes_alloc(num_bytes+1); // 1 zeroed byte more to defeat clib-str overflows
  cell->tag = TAG_BYTES;
  cell->size = num_bytes;
  return cell;
}

Cell* alloc_bytes() {
  return alloc_num_bytes(SYM_INIT_BUFFER_SIZE);
}

Cell* alloc_num_string(unsigned int num_bytes) {
  Cell* cell = cell_alloc();
  cell->addr = bytes_alloc(num_bytes+1); // 1 zeroed byte more to defeat clib-str overflows
  cell->tag = TAG_STR;
  cell->size = num_bytes;
  return cell;
}

Cell* alloc_string() {
  return alloc_num_string(SYM_INIT_BUFFER_SIZE);
}

Cell* alloc_string_copy(char* str) {
  Cell* cell = cell_alloc();
  cell->addr = bytes_alloc(strlen(str)+1);
  strcpy(cell->addr, str);
  cell->tag = TAG_STR;
  cell->size = strlen(str)+1;
  return cell;
}

Cell* alloc_concat(Cell* str1, Cell* str2) {
  if (!str1 || !str2) return alloc_error(ERR_INVALID_PARAM_TYPE);
  if (str1->tag!=TAG_BYTES && str1->tag!=TAG_STR) return alloc_error(ERR_INVALID_PARAM_TYPE);
  if (str2->tag!=TAG_BYTES && str2->tag!=TAG_STR) return alloc_error(ERR_INVALID_PARAM_TYPE);
  
  Cell* cell = cell_alloc();
  unsigned int newsize = strlen(str1->addr)+strlen(str2->addr)+1;
  cell->addr = bytes_alloc(newsize+1);
  strcpy(cell->addr, str1->addr);
  strcpy(cell->addr+strlen(str1->addr), str2->addr);
  cell->tag = TAG_STR;
  cell->size = newsize;
  return cell;
}

Cell* alloc_builtin(unsigned int b) {
  Cell* num = cell_alloc();
  num->tag = TAG_BUILTIN;
  num->value = b;
  num->next = 0;
  return num;
}

Cell* alloc_lambda(Cell* args) {
  Cell* l = cell_alloc();
  l->tag = TAG_LAMBDA;
  l->addr = args; // arguments
  //l->next = cdr(def); // body
  return l;
}

Cell* alloc_nil() {
  return alloc_cons(0,0);
}

int is_nil(Cell* c) {
  return (!c || (c->addr==0 && c->next==0));
}

Cell* alloc_error(unsigned int code) {
  Cell* c = cell_alloc();
  c->tag = TAG_ERROR;
  c->value = code;
  c->next = 0;
  return c;
}

Cell* alloc_clone(Cell* orig) {
  if (!orig) return 0;
  Cell* clone = cell_alloc();
  clone->tag  = orig->tag;
  clone->addr = 0;
  clone->next = 0;
  clone->size = orig->size;

  //printf("cloning a %d (value %d)\n",orig->tag,orig->value);
  
  if (orig->tag == TAG_SYM || orig->tag == TAG_STR || orig->tag == TAG_BYTES) {
    clone->addr = bytes_alloc(orig->size+1);
    memcpy(clone->addr, orig->addr, orig->size);
  /*} else if (orig->tag == TAG_BYTES) {
    clone->addr = bytes_alloc(orig->size);
    memcpy(clone->addr, orig->addr, orig->size);*/
  } else if (orig->tag == TAG_CONS) {
    if (orig->addr) {
      clone->addr = alloc_clone(orig->addr);
    }
    if (orig->next) {
      clone->next = alloc_clone(orig->next);
    }
  } else {
    clone->addr = orig->addr;
    clone->next = orig->next;
  }
  return clone;
}
