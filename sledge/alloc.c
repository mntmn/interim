#include "alloc.h"
#include <malloc.h>

static void* cell_heap;
static void* cell_stack;

static unsigned long stack_bytes_max = 0;
static unsigned long heap_bytes_max = 0;
static unsigned long heap_bytes_used = 0;
static unsigned long stack_bytes_used = 0;

static enum cell_allocator_t cell_allocator = CA_STACK;

static Cell oom_cell;

void init_allocator() {
  oom_cell.tag = TAG_ERROR;
  oom_cell.value = ERR_OUT_OF_MEMORY;

  stack_bytes_max = 1024*1024*10;
  heap_bytes_max = 1024*1024*10;

  cell_heap = malloc(heap_bytes_max);
  cell_stack = malloc(stack_bytes_max);
}

void* cell_malloc(int num_bytes) {
  if (cell_allocator == CA_STACK) {
#ifdef DEBUG
    printf("cell_malloc/stack: %d (%d)\n",num_bytes,stack_bytes_used);
#endif
    void* new_mem = cell_stack + stack_bytes_used;
    if (stack_bytes_used + num_bytes < stack_bytes_max) {
      stack_bytes_used += num_bytes;
      return new_mem;
    } else {
      printf("cell_malloc/stack: out of memory\n");
      exit(1);
      return &oom_cell;
    }
  } else {
    void* new_mem = cell_heap + heap_bytes_used;
    if (heap_bytes_used + num_bytes < heap_bytes_max) {
      heap_bytes_used += num_bytes;
      return new_mem;
    } else {
      printf("cell_malloc/heap: out of memory\n");
      exit(1);
      return &oom_cell;
    }
  }
}

void* cell_realloc(void* old_addr, unsigned int old_size, unsigned int num_bytes) {
  // FIXME
  //cell_free(old_addr, old_size);
  void* new = cell_malloc(num_bytes);
  memcpy(new, old_addr, old_size);
  return new;
}

Cell* alloc_cons(Cell* ar, Cell* dr) {
  //printf("alloc_cons: ar %p dr %p\n",ar,dr);
  Cell* cons = cell_malloc(sizeof(Cell));
  cons->tag = TAG_CONS;
  cons->addr = ar?alloc_clone(ar):ar;
  cons->next = dr?alloc_clone(dr):dr;
  return cons;
}

Cell* alloc_sym(char* str) {
  Cell* sym = cell_malloc(sizeof(Cell));
  sym->tag = TAG_SYM;
  if (str) {
    sym->size = strlen(str)+1;
    sym->addr = cell_malloc(sym->size);
    memcpy(sym->addr, str, sym->size);
  } else {
    sym->addr = 0;
    sym->size = 0;
  }
  return sym;
}

Cell* alloc_int(int i) {
  Cell* num = cell_malloc(sizeof(Cell));
  num->tag = TAG_INT;
  num->value = i;
  return num;
}

Cell* alloc_num_bytes(unsigned int num_bytes) {
  Cell* cell = cell_malloc(sizeof(Cell));
  cell->addr = cell_malloc(num_bytes);
  memset(cell->addr, 0, num_bytes);
  cell->tag = TAG_BYTES;
  cell->size = num_bytes;
  return cell;
}

Cell* alloc_bytes() {
  return alloc_num_bytes(SYM_INIT_BUFFER_SIZE);
}

Cell* alloc_string() {
  Cell* cell = cell_malloc(sizeof(Cell));
  cell->addr = cell_malloc(SYM_INIT_BUFFER_SIZE);
  memset(cell->addr, 0, SYM_INIT_BUFFER_SIZE);
  cell->tag = TAG_STR;
  cell->size = SYM_INIT_BUFFER_SIZE;
  return cell;
}

Cell* alloc_string_copy(char* str) {
  Cell* cell = cell_malloc(sizeof(Cell));
  cell->addr = cell_malloc(strlen(str)+1);
  strcpy(cell->addr, str);
  cell->tag = TAG_STR;
  cell->size = strlen(str)+1;
  return cell;
}

Cell* alloc_builtin(unsigned int b) {
  Cell* num = cell_malloc(sizeof(Cell));
  num->tag = TAG_BUILTIN;
  num->value = b;
  num->next = 0;
  return num;
}

Cell* alloc_lambda(Cell* args) {
  Cell* l = cell_malloc(sizeof(Cell));
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
  Cell* c = cell_malloc(sizeof(Cell));
  c->tag = TAG_ERROR;
  c->value = code;
  c->next = 0;
  return c;
}

Cell* alloc_clone(Cell* orig) {
  if (!orig) return 0;
  Cell* clone = cell_malloc(sizeof(Cell));
  clone->tag  = orig->tag;
  clone->addr = 0;
  clone->next = 0;
  clone->size = orig->size;
  if (orig->tag == TAG_SYM || orig->tag == TAG_STR) {
    clone->addr = cell_malloc(orig->size+1);
    memcpy(clone->addr, orig->addr, orig->size+1);
  /*} else if (orig->tag == TAG_BYTES) {
    clone->addr = cell_malloc(orig->size);
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
