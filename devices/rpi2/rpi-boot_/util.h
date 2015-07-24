/* Copyright (C) 2013 by John Cronin <jncronin@tysos.org>
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:

 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.

 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 */

#ifndef UTIL_H
#define UTIL_H

#include <stdint.h>
#include <stddef.h>

// Support for unaligned data access
static inline void write_word(uint32_t val, uint8_t *buf, int offset)
{
    buf[offset + 0] = val & 0xff;
    buf[offset + 1] = (val >> 8) & 0xff;
    buf[offset + 2] = (val >> 16) & 0xff;
    buf[offset + 3] = (val >> 24) & 0xff;
}

static inline void write_halfword(uint16_t val, uint8_t *buf, int offset)
{
    buf[offset + 0] = val & 0xff;
    buf[offset + 1] = (val >> 8) & 0xff;
}

static inline void write_byte(uint8_t byte, uint8_t *buf, int offset)
{
    buf[offset] = byte;
}

static inline uint32_t read_word(uint8_t *buf, int offset)
{
	uint32_t b0 = buf[offset + 0] & 0xff;
	uint32_t b1 = buf[offset + 1] & 0xff;
	uint32_t b2 = buf[offset + 2] & 0xff;
	uint32_t b3 = buf[offset + 3] & 0xff;

	return b0 | (b1 << 8) | (b2 << 16) | (b3 << 24);
}

static inline uint16_t read_halfword(uint8_t *buf, int offset)
{
	uint16_t b0 = buf[offset + 0] & 0xff;
	uint16_t b1 = buf[offset + 1] & 0xff;

	return b0 | (b1 << 8);
}

static inline uint8_t read_byte(uint8_t *buf, int offset)
{
	return buf[offset];
}

void *quick_memcpy(void *dest, void *src, size_t n);

// A faster memcpy if 16-byte alignment is assured
/*static inline void *qmemcpy(void *dest, void *src, size_t n)
{
	// Can only use quick_memcpy if dest, src and n are multiples
	//  of 16
	if((((uintptr_t)dest & 0xf) == 0) && (((uintptr_t)src & 0xf) == 0) &&
		((n & 0xf) == 0))
		return quick_memcpy(dest, src, n);
	else
		return memcpy(dest, src, n);
    }*/

uintptr_t alloc_buf(size_t size);

// Support for BE to LE conversion
#ifdef __GNUC__
#define byte_swap __builtin_bswap32
#else
static inline uint32_t byte_swap(uint32_t in)
{
	uint32_t b0 = in & 0xff;
    uint32_t b1 = (in >> 8) & 0xff;
    uint32_t b2 = (in >> 16) & 0xff;
    uint32_t b3 = (in >> 24) & 0xff;
    uint32_t ret = (b0 << 24) | (b1 << 16) | (b2 << 8) | b3;
    return ret;
}
#endif // __GNUC__

#endif

