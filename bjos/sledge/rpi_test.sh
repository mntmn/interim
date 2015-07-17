set -e
gcc --std=gnu99 jit_arm_test.c
./a.out
objdump -D -b binary -m armv5 /tmp/test

