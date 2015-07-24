/* $OpenBSD: strtoll.c,v 1.7 2013/03/28 18:09:38 martynas Exp $ */
/*-
 * Copyright (c) 1992 The Regents of the University of California.
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 * 3. Neither the name of the University nor the names of its contributors
 *    may be used to endorse or promote products derived from this software
 *    without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE REGENTS AND CONTRIBUTORS ``AS IS'' AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED.  IN NO EVENT SHALL THE REGENTS OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS
 * OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY
 * OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
 * SUCH DAMAGE.
 */

/*
 * Modified 21 May 2013 for rpi-boot by github.org/JamesC1
 *
 * Interestingly, longlong division pulls in a bunch of
 * exception-handling code.
 * Trying to rewrite to avoid doing it.
 */

/*
#include <sys/types.h>
*/

#include "ctype.h"
#include <errno.h>
#include <limits.h>
#include <stdlib.h>

#ifdef STRTOLL_DEBUG
#include <stdio.h>
#endif

/*
 * Convert a string to a long long.
 *
 * Ignores `locale' stuff.  Assumes that the upper and lower case
 * alphabets and digits are each contiguous.
 */
long long
strtoll(const char *nptr, char **endptr, int base)
{
	const char *s;
	unsigned long long acc;
	int c;
	int neg, any;

	/*
	 * Skip white space and pick up leading +/- sign if any.
	 * If base is 0, allow 0x for hex and 0 for octal, else
	 * assume decimal; if base is already 16, allow 0x.
	 */
	s = nptr;
	do {
		c = (unsigned char) *s++;
	} while (isspace(c));

	if (c == '-') {
		neg = 1;
		c = *s++;
	} else {
		neg = 0;
		if (c == '+')
			c = *s++;
	}

	if ((base == 0 || base == 16) &&
	    c == '0' && (*s == 'x' || *s == 'X')) {
		c = s[1];
		s += 2;
		base = 16;
	}

	if (base == 0)
		base = c == '0' ? 8 : 10;

	/*
	 * acc accumulates a positive number.
	 * Overflow is when the correctly signed version of
	 * that number has the wrong sign.
	 */

	for (acc = 0, any = 0;; c = (unsigned char) *s++) {
		if (isdigit(c))
			c -= '0';
		else if (isalpha(c))
			c -= isupper(c) ? 'A' - 10 : 'a' - 10;
		else
			break;
		if (c >= base)
			break;
		if (any < 0)
			continue;
		any = 1;
		acc *= base;
		acc += c;
#ifdef STRTOLL_DEBUG
		printf("  accumulator so far: %u(0x%x)",
		        (unsigned long)acc, (unsigned long)acc);
#endif
		if (neg) {
			if (0LL < (long long)(-acc))
			{
#ifdef STRTOLL_DEBUG
				printf("  OVERFLOW negative: %lld\n",
				       (long long)(-acc));
#endif
				any = -1;
				acc = LLONG_MIN;
				errno = ERANGE;
			}
		} else {
			if (0LL > (long long)(acc))
			{
#ifdef STRTOLL_DEBUG
				printf("  OVERFLOW positive: %lld\n",
				       (long long)(-acc));
#endif
				any = -1;
				acc = LLONG_MAX;
				errno = ERANGE;
			}
		}
	}
	if (endptr != 0)
		*endptr = (char *) (any ? s - 1 : nptr);
	if (neg)
		return (-acc);
	else
		return (acc);
}

