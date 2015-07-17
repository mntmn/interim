set -e
gcc --std=c99 jit_arm_test.c
./a.out
arm-none-eabi-objdump -D -b binary -m armv5 /tmp/test

