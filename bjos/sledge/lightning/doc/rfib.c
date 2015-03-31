#include <stdio.h>
#include <lightning.h>

static jit_state_t *_jit;

typedef int (*pifi)(int);       /* Pointer to Int Function of Int */

int main(int argc, char *argv[])
{
  pifi       fib;
  jit_node_t *label;
  jit_node_t *call;
  jit_node_t *in;                 /* offset of the argument */
  jit_node_t *ref;                /* to patch the forward reference */

  init_jit(argv[0]);
  _jit = jit_new_state();

  label = jit_label();
        jit_prolog   ();
  in =  jit_arg      ();
        jit_getarg   (JIT_V0, in);              /* V0 = n */
  ref = jit_blti     (JIT_V0, 2);
        jit_subi     (JIT_V1, JIT_V0, 1);       /* V1 = n-1 */
        jit_subi     (JIT_V2, JIT_V0, 2);       /* V2 = n-2 */
        jit_prepare();
          jit_pushargr(JIT_V1);
        call = jit_finishi(NULL);
        jit_patch_at(call, label);
        jit_retval(JIT_V1);                     /* V1 = fib(n-1) */
        jit_prepare();
          jit_pushargr(JIT_V2);
        call = jit_finishi(NULL);
        jit_patch_at(call, label);
        jit_retval(JIT_V2);                     /* V2 = fib(n-2) */
        jit_addi(JIT_V1,  JIT_V1,  1);
        jit_addr(JIT_R0, JIT_V1, JIT_V2);       /* R0 = V1 + V2 + 1 */
        jit_retr(JIT_R0);

  jit_patch(ref);                               /* patch jump */
        jit_movi(JIT_R0, 1);                    /* R0 = 1 */
        jit_retr(JIT_R0);

  /* call the generated code, passing 32 as an argument */
  fib = jit_emit();
  jit_clear_state();
  printf("fib(%d) = %d\n", 32, fib(32));
  jit_destroy_state();
  finish_jit();
  return 0;
}
