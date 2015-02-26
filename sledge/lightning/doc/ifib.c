#include <stdio.h>
#include <lightning.h>

static jit_state_t *_jit;

typedef int (*pifi)(int);       /* Pointer to Int Function of Int */

int main(int argc, char *argv[])
{
  pifi       fib;
  jit_node_t *in;               /* offset of the argument */
  jit_node_t *ref;              /* to patch the forward reference */
  jit_node_t *jump;             /* jump to start of loop */
  jit_node_t *loop;             /* start of the loop */

  init_jit(argv[0]);
  _jit = jit_new_state();

        jit_prolog   ();
  in =  jit_arg      ();
        jit_getarg   (JIT_R2, in);              /* R2 = n */
        jit_movi     (JIT_R1, 1);
  ref = jit_blti     (JIT_R2, 2);
        jit_subi     (JIT_R2, JIT_R2, 1);
        jit_movi     (JIT_R0, 1);

  loop= jit_label();
        jit_subi     (JIT_R2, JIT_R2, 1);       /* decr. counter */
        jit_addr     (JIT_V0, JIT_R0, JIT_R1);  /* V0 = R0 + R1 */
        jit_movr     (JIT_R0, JIT_R1);          /* R0 = R1 */
        jit_addi     (JIT_R1, JIT_V0, 1);       /* R1 = V0 + 1 */
  jump= jit_bnei     (JIT_R2, 0);               /* if (R2) goto loop; */
  jit_patch_at(jump, loop);

  jit_patch(ref);                               /* patch forward jump */
        jit_movr     (JIT_R0, JIT_R1);          /* R0 = R1 */
        jit_retr     (JIT_R0);

  /* call the generated code, passing 36 as an argument */
  fib = jit_emit();
  jit_clear_state();
  printf("fib(%d) = %d\n", 36, fib(36));
  jit_destroy_state();
  finish_jit();
  return 0;
}
