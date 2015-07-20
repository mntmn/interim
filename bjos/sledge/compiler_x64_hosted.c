
Cell* execute_jitted(void* binary) {
  return (Cell*)((funcptr)binary)(0);
}

int compile_for_platform(Cell* expr, Cell** res) {

  int success = 1;
      
  jit_out = fopen("/tmp/jit_out.s","w");
  
  jit_init();
  int tag = compile_expr(expr, NULL, TAG_ANY);
  jit_ret();

  if (success) {
    fclose(jit_out);

    FILE* asm_f = fopen("/tmp/jit_out.s","r");
    uint32_t* jit_asm = malloc(4096);
    memset(jit_asm, 0, 4096);
    fread(jit_asm,1,4096,asm_f);
    fclose(asm_f);
        
#ifdef DEBUG
    printf("\nJIT ---------------------\n%s-------------------------\n\n",jit_asm);
#endif
        
    // prefix with arm-none-eabi- on ARM  -mlittle-endian
      
    system("as /tmp/jit_out.s -o /tmp/jit_out.o");
    system("objcopy /tmp/jit_out.o -O binary /tmp/jit_out.bin");

    FILE* binary_f = fopen("/tmp/jit_out.bin","r");

    uint32_t* jit_binary = mmap(0, 4096, PROT_READ | PROT_WRITE, MAP_PRIVATE|MAP_ANONYMOUS, 0, 0);
        
    int bytes_read = fread(jit_binary,1,4096,binary_f);
    fclose(binary_f);

#ifdef DEBUG
    printf("<assembled bytes: %d at: %p>\n",bytes_read,jit_binary);
#endif

    // read symbols for linking lambdas
    system("nm /tmp/jit_out.o > /tmp/jit_out.syms");
    FILE* link_f = fopen("/tmp/jit_out.syms","r");
    if (link_f) {
      char link_line[128];
      while(fgets(link_line, sizeof(link_line), link_f)) {

        if (strlen(link_line)>22) {
          char ida=link_line[19];
          char idb=link_line[20];
          char idc=link_line[21];
          //printf("link_line: %s %c %c %c\n",link_line,ida,idb,idc);

          if (ida=='f' && idc=='_') {
            Cell* lambda = (Cell*)strtoul(&link_line[24], NULL, 16);
            if (idb=='0') {
              // function entrypoint
              // TODO: 64/32 bit
              unsigned long long offset = strtoul(link_line, NULL, 16);
              void* binary = ((uint8_t*)jit_binary) + offset;
              //printf("function %p entrypoint: %p (+%ld)\n",lambda,binary,offset);

              if (lambda->tag == TAG_LAMBDA) {
                lambda->next = binary;
              } else {
                printf("fatal error: no lambda found at %p!\n",lambda);
              }
            }
            else if (idb=='1') {
              // function exit
              unsigned long long offset = strtoul(link_line, NULL, 16);
              //printf("function exit point: %p\n",offset);
            }
          }
        }
      }
    }
      
    int mp_res = mprotect(jit_binary, 4096, PROT_EXEC|PROT_READ);

    if (!mp_res) {
      *res = execute_jitted(jit_binary);
      success = 1;
    } else {
      printf("<mprotect result: %d\n>",mp_res);
      *res = NULL;
      success = 0;
    }
  }
  return success;
}
