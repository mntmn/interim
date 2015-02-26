#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>

/*
void __assert_fail() {
  printf("assert_fail()\n");
  exit(0);
}

void __assert_func(const char *_file, int _line, const char *_func, const char *_expr)
{
  printf("assert_func()\n");
  exit(0);
  }*/


// copypasted from https://raw.githubusercontent.com/cnplab/mini-os/master/lib/string.c
/* newlib defines ffs but not ffsll or ffsl */
int __ffsti2 (long long int lli)
{
    int i, num, t, tmpint, len;

    num = sizeof(long long int) / sizeof(int);
    if (num == 1) return (ffs((int) lli));
    len = sizeof(int) * 8;

    for (i = 0; i < num; i++) {
        tmpint = (int) (((lli >> len) << len) ^ lli);

        t = ffs(tmpint);
        if (t)
            return (t + i * len);
        lli = lli >> len;
    }
    return 0;
}

int __ffsdi2 (long int li)
{
    return __ffsti2 ((long long int) li);
}

int ffsl (long int li)
{
    return __ffsti2 ((long long int) li);
}

int ffsll (long long int lli)
{
    return __ffsti2 (lli);
}

void *mmap(void *addr, size_t length, int prot, int flags, int fd, off_t offset) {
  //printf("mmap! %p %x\n",addr,length);
  return malloc(length);
}

void munmap(void* addr) {
  free(addr);
}

void mprotect() {
}
