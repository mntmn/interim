Cell* execute_jitted(void* binary) {
  return (Cell*)((funcptr)binary)(0);
}

int compile_for_platform(Cell* expr, Cell** res) {

  int codesz = 8192;
  
  //jit_out = fopen("/tmp/jit_out.s","w");
  
  jit_init();
  
  register void* sp asm ("sp"); // FIXME maybe unportable
  Frame empty_frame = {NULL, 0, 0, sp};
  int success = compile_expr(expr, &empty_frame, TAG_ANY);
  jit_ret();

  if (success) {
    /*fclose(jit_out);

    FILE* asm_f = fopen("/tmp/jit_out.s","r");
    uint32_t* jit_asm = malloc(10000);
    memset(jit_asm, 0, 10000);
    fread(jit_asm,1,10000,asm_f);
    fclose(asm_f);*/
        
#ifdef DEBUG
    //printf("\nJIT ---------------------\n%s-------------------------\n\n",jit_asm);
#endif
    //free(jit_asm);
        
    // prefix with arm-none-eabi- on ARM  -mlittle-endian
      
    //system("as /tmp/jit_out.s -o /tmp/jit_out.o");
    //system("objcopy /tmp/jit_out.o -O binary /tmp/jit_out.bin");
    //FILE* binary_f = fopen("/tmp/jit_out.bin","r");

    uint32_t* jit_binary = malloc(codesz);
        
    int bytes_read = 0; //fread(jit_binary,1,codesz,binary_f);
    //fclose(binary_f);

#ifdef DEBUG
    printf("<assembled bytes: %d at: %p>\n",bytes_read,jit_binary);
#endif

    if (bytes_read>codesz) {
      printf("<error: max assembly size of %d exhausted. aborting>\n",codesz);
      munmap(jit_binary,codesz);
      return 0;
    }
      
    int mp_res = -1;

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
