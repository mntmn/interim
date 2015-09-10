#ifndef DEBUG_UTIL_H
#define DEBUG_UTIL_H

#include <stdint.h>

void memdump(void* start,uint32_t len,int raw);
void printhex(uint32_t num);
void printhex_signed(int32_t num);

#endif
