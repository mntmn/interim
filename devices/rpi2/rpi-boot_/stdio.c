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

#include <stdint.h>
#include <stddef.h>
#include <stdarg.h>
#include "stdio.h"

const char lowercase[] = "0123456789abcdef";
const char uppercase[] = "0123456789abcdef";

int (*stderr_putc)(int c);
int (*stdout_putc)(int c);
int (*stream_putc)(int c, FILE *stream);

int fputc(int c, FILE *stream)
{
	if(stream == stdout)
		return stdout_putc(c);
	else if(stream == stderr)
		return stderr_putc(c);
	else
		return stream_putc(c, stream);
}

int putc(int c, FILE *stream)
{
	return fputc(c, stream);
}

int putchar(int c)
{
	return fputc(c, stdout);
}

int fputs(const char *s, FILE *stream)
{
	while(*s)
		fputc(*s++, stream);
	return 0;
}

int puts(const char *s)
{
	fputs(s, stdout);
	fputc('\n', stdout);
	return 0;
}

void puthex(uint32_t val)
{
	for(int i = 7; i >= 0; i--)
		putchar(lowercase[(val >> (i * 4)) & 0xf]);
}

void putval(uint32_t val, int base, char *dest, int dest_size, int dest_start, int padding, char *case_str)
{
	int i;

	if(padding > (dest_size - dest_start))
		padding = dest_size - dest_start;

	for(i = 0; i < padding; i++)
		dest[i + dest_start] = '0';

	i = 0;
	while((val != 0) && (i < (dest_size - dest_start)))
	{
		uint32_t digit = val % base;
		dest[dest_size - i - 1 + dest_start] = case_str[digit];
		i++;
		val /= base;
	}
}
