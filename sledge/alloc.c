#include "alloc.h"
#include <malloc.h>
#include <stdint.h>

void* byte_heap;
Cell* cell_heap;

uint32_t cells_used;
uint32_t byte_heap_used;

Cell oom_cell;

#define MAX_CELLS 100000
#define MAX_BYTE_HEAP 1024*1024*128

static struct MemStats mem_stats;

void init_allocator() {
  oom_cell.tag = TAG_ERROR;
  oom_cell.value = ERR_OUT_OF_MEMORY;

  byte_heap_used = 0;
  cells_used = 0;

#ifndef SLEDGE_MALLOC
  //cell_heap = malloc(heap_bytes_max);
  uint32_t cell_mem_reserved = MAX_CELLS * sizeof(Cell);
  cell_heap = malloc(cell_mem_reserved);
  printf("\r\n++ cell heap at %p, %d bytes reserved\r\n",cell_heap,cell_mem_reserved);
  memset(cell_heap,0,cell_mem_reserved);
  
  byte_heap = malloc(MAX_BYTE_HEAP);
#endif
}

Cell* cell_alloc() {
  Cell* res = &cell_heap[cells_used];
  cells_used++;
  printf("++ cell_alloc: %d \r\n",cells_used);
  return res;
}

void* bytes_alloc(int num_bytes) {
#ifdef SLEDGE_MALLOC
  void* new_mem = malloc(num_bytes);
  memset(new_mem, 0, num_bytes);
  return new_mem;
#endif
  void* new_mem = byte_heap + byte_heap_used;
  if (byte_heap_used + num_bytes < MAX_BYTE_HEAP) {
    byte_heap_used += num_bytes;
    printf("++ byte_alloc: %d (+%d) \r\n",byte_heap_used,num_bytes);
    return new_mem;
  } else {
    printf("~~ bytes_alloc: out of memory: %d (%d)\r\n",byte_heap,byte_heap_used);
    exit(1);
    return NULL;
  }
}

void mark_tree(Cell* c) {
  if (!(c->tag & TAG_MARK)) {
    char buf[300];
    lisp_write(c, buf, 299);
    //printf("marking live: %s\n",buf);
    
    c->tag |= TAG_MARK;
    
    if (c->tag == TAG_CONS) {
      if (c->addr) mark_tree((Cell*)c->addr);
      if (c->next) mark_tree((Cell*)c->next);
    }
    else if (c->tag == TAG_SYM) {
      if (c->next) mark_tree((Cell*)c->next);
    }
  }
}

void collect_garbage(env_entry* global_env) {
  // mark

  // check all symbols in the environment
  // and look where they lead (cons trees, bytes, strings)
  // mark all of them as usable

  for (env_entry* e=global_env; e != NULL; e=e->hh.next) {
    //printf("env entry: %s pointing to %p\n",e->name,e->cell);
    mark_tree(e->cell);
  }

  // sweep -- free all things that are not marked

  int gc = 0;
  char buf[300];

  // note: all pointers can be fixed by simply subtracting
  // one cell size from each for every memmove

  printf("before: \n");
  for (int i=0; i<cells_used; i++) {
    Cell* c = &cell_heap[i];
    if (!(c->tag & TAG_MARK)) {
      printf(".");
    } else {
      printf("o");
    }
  }
  printf("\n\n");
  
  for (int i=0; i<cells_used; i++) {
    Cell* c = &cell_heap[i];
    if (!(c->tag & TAG_MARK)) {
      // garbage!
      //lisp_write(c, buf, 299);
      //printf("garbage: %s\n",buf);

      // garbage = hole
      // copy next contiguous block here
      // and put it in an array for remapping

      int hole_i = i;
      int hole_size = 0;
      int done = 0;
      int block_size = 1;
      do {
        // find next with TAG_MARK
        hole_size++;
        if (cell_heap[hole_i+hole_size].tag & TAG_MARK) {
          //printf("~~ end of hole at %d\n",hole_i+j);
          // stop + copy
          //printf("<< move %d cells from %d to %d\n",block_size,hole_i+j,hole_i);
          //memmove(&cell_heap[hole_i], &cell_heap[hole_i+hole_size], sizeof(Cell)*block_size);

          void* old_address = &cell_heap[hole_i+hole_size];
          void* new_address = &cell_heap[hole_i];

          // do we have an env entry pointing to this?
          for (env_entry* e=global_env; e != NULL; e=e->hh.next) {
            if (e->cell == old_address) {
              e->cell = new_address;
              printf("~~ moved env cell pointer of %s to %p\n",e->name,e->cell);
            }
          }

          // do we have a future cell pointing back to this?
          for (int j=i; j<cells_used; j++) {
            if (cell_heap[j].addr == old_address) {
              cell_heap[j].addr = new_address;
              printf("~~ rewrote ar of cell %d\n",j);
            }
            else if (cell_heap[j].next == old_address) {
              cell_heap[j].next = new_address;
              printf("~~ rewrote dr of cell %d\n",j);
            }
          }
          
          // swap
          Cell tmp = cell_heap[hole_i];
          cell_heap[hole_i] = cell_heap[hole_i+hole_size];
          cell_heap[hole_i+hole_size] = tmp;
          
          done = 1;
        }
        if (hole_i+hole_size>=cells_used) {
          //printf("~~ gc reached end of live block at %d\r\n",hole_i+j);
          done = 1;
        }
      } while(!done);
      //i+=j-block_size; // moved
      
    } else {
    }
  }
  printf("\n");
  
  printf("after: \n");
  for (int i=0; i<cells_used; i++) {
    Cell* c = &cell_heap[i];
    if (!(c->tag & TAG_MARK)) {
      printf(".");
      gc++;
    } else {
      printf("o");
    }
  }
  printf("\n\n");

  printf("~~ %d of %d cells are garbage.\r\n",gc,cells_used);
  //cells_used -= gc;
  
  cells_used -= gc;
  
  for (int i=0; i<cells_used; i++) {
    // unset mark bits
    cell_heap[i].tag &= ~TAG_MARK;
  }

  printf("-- %d live cells.\n\n",cells_used);
}

void* cell_realloc(void* old_addr, unsigned int old_size, unsigned int num_bytes) {
  // FIXME
  //cell_free(old_addr, old_size);
  void* new = bytes_alloc(num_bytes);
  memcpy(new, old_addr, old_size);
  return new;
}

MemStats* alloc_stats() {
  /*mem_stats.stack_bytes_used = stack_bytes_used;
  mem_stats.stack_bytes_max = stack_bytes_max;
  mem_stats.heap_bytes_used = heap_bytes_used;
  mem_stats.heap_bytes_max = heap_bytes_max;*/
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
    
    sym->addr = bytes_alloc(sz);

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
  cell->addr = bytes_alloc(num_bytes);
  memset(cell->addr, 0, num_bytes);
  cell->tag = TAG_BYTES;
  cell->size = num_bytes;
  return cell;
}

Cell* alloc_bytes() {
  return alloc_num_bytes(SYM_INIT_BUFFER_SIZE);
}

Cell* alloc_num_string(unsigned int num_bytes) {
  Cell* cell = cell_alloc();
  cell->addr = bytes_alloc(num_bytes);
  memset(cell->addr, 0, num_bytes);
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
  Cell* cell = cell_alloc();
  unsigned int newsize = strlen(str1->addr)+strlen(str2->addr)+1;
  cell->addr = bytes_alloc(newsize);
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
  
  if (orig->tag == TAG_SYM || orig->tag == TAG_STR) {
    clone->addr = bytes_alloc(orig->size+1);
    memcpy(clone->addr, orig->addr, orig->size+1);
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
