void _exit(int status) {
  uart_puts("-- clib exit called. hanging.\r\n");
  
	while (1) {
    uart_putc(uart_getc());
  }
}

void* _sbrk(int incr)
{
  /*uart_puts("-- sbrk: ");
  printhex((uint32_t)heap_end);
  uart_puts(" ");
  printhex_signed(incr);
  uart_puts("\r\n");*/
  
  uint8_t* prev_heap_end;

  prev_heap_end = heap_end;

  heap_end += incr;
  return (void*)prev_heap_end;
}

void* sbrk(int incr)
{
  return _sbrk(incr);
}

void _kill() {
  uart_puts("-- clib kill called. not implemented.\r\n");
}

int _getpid() {
  uart_puts("-- clib getpid_r called. stubbed.\r\n");
  return 1;
}

int _isatty_r() {
  uart_puts("-- clib isatty_r called. stubbed.\r\n");
  return 1;
}

int _close() {
  uart_puts("-- clib close called. stubbed.\r\n");
  return 1;
}

int _fstat() {
  //uart_puts("-- clib fstat called. stubbed.\n");
  return 0;
}

int _fseek() {
  //uart_puts("-- clib fseek called. stubbed.\n");
  return 0;
}

int _lseek() {
  //uart_puts("-- clib lseek called. stubbed.\n");
  return 0;
}

int __sseek64() {
  //uart_puts("-- clib lseek called. stubbed.\n");
  return 0;
}

int _read() {
  //uart_puts("-- clib read called. stubbed.\n");
  return 0;
}


size_t _write(int fildes, const void *buf, size_t nbytes) {
  uart_puts("-- clib _write called:\n");
  for (int i=0; i<nbytes; i++) {
    uart_putc(((char*)buf)[i]);
  }
  return nbytes;
}

int _fini() {
  uart_puts("-- clib _fini called. stubbed.\n");
  return 0;
}

// this is actually used
size_t __swrite64(int fildes, int cookie, const void *buf, size_t nbytes) {
  for (int i=0; i<nbytes; i++) {
    uart_putc(((char*)buf)[i]);
  }
  return nbytes;
}
