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

#include <string.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>

int errno;

void *memcpy(void *dest, const void *src, size_t n)
{
	char *s = (char *)src;
	char *d = (char *)dest;
	while(n > 0)
	{
		*d++ = *s++;
		n--;
	}
	return dest;
}

void *memset(void *s, int c, size_t n)
{
	char *dest = (char *)s;
	while(n > 0)
	{
		*dest++ = (char)c;
		n--;
	}
	return s;
}

void abort(void)
{
	fputs("abort() called\n", stdout);
	fputs("abort() called\n", stderr);

	while(1);
}

char *strcpy(char *dest, const char *src)
{
	char *d = dest;
	while(*src != 0)
		*d++ = *src++;
	*d = 0;
	return dest;
}

char *strcat(char *dest, const char *src)
{
	char *d = dest;
	while(*d) d++;
	while(*src) *d++ = *src++;
	*d = 0;

	return dest;
}

char *strncpy(char *dest, const char *src, size_t n)
{
	char *d = dest;
	while((*src != 0) && (n > 0))
	{
		*d++ = *src++;
		n--;
	}
	if(n > 0)
		*d = 0;
	return dest;
}

size_t strlen(const char *s)
{
	size_t ret = 0;
	while(*s++ != 0) ret++;
	return ret;
}

int strcmp(const char *s1, const char *s2)
{
	while(*s1 || *s2)
	{
		char s = *s1++ - *s2++;
		if(s != 0)
			return (int)s;
	}
	return 0;
}

int raise(int sig)
{
    printf("ERROR: signal %i raised.  Halted.\n", sig);
    while(1);
    return 0;
}

int tolower(int c)
{
	if((c >= 'A') && (c <= 'Z'))
		return 'a' + (c - 'A');
	else
		return c;
}

int toupper(int c)
{
	if((c >= 'a') && (c <= 'z'))
		return 'A' + (c - 'a');
	else
		return c;
}

char *strlwr(char *s)
{
	size_t len = strlen(s);
	char *ret = (char *)malloc(len + 1);
	ret[len] = 0;

	for(size_t i = 0; i < len; i++)
		ret[i] = tolower(s[i]);
	return ret;
}

char *strupr(char *s)
{
	size_t len = strlen(s);
	char *ret = (char *)malloc(len + 1);
	ret[len] = 0;

	for(size_t i = 0; i < len; i++)
		ret[i] = toupper(s[i]);
	return ret;
}

