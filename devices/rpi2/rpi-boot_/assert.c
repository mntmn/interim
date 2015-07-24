/* Copyright (C) 2013 by github.org/JamesC1
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

/* Use of libgcc unwinder added by John Cronin 17/6/13 */

#include "assert.h"

#ifdef HAVE_UNWIND_H
#include <unwind.h>
#endif

#include <stdio.h>

#ifdef HAVE_UNWIND_H
static _Unwind_Reason_Code unwind_cb(struct _Unwind_Context *ctx, void *trace_argument)
{
	int *depth = (int *)trace_argument;
	printf("%i:\tFunction: %08x, IP: %08x\n", *depth, _Unwind_GetRegionStart(ctx),
		_Unwind_GetIP(ctx));
	(*depth)++;

	return _URC_NO_REASON;
}

void assertStackDump(void)
{
	printf("\nStack dump:\n");
	int depth = 0;
	_Unwind_Backtrace(unwind_cb, &depth);
}
#else
void assertStackDump(void) {
	uint32_t start = 0x12345678;
	int rowcount = 20;
	uint32_t *rowpos = &start;

	printf("\nStack dump:\n");

	while (rowcount--) {
		printf("%06x: %08x %08x %08x %08x %08x %08x %08x %08x\n",
		       rowpos, rowpos[0], rowpos[1], rowpos[2], rowpos[3],
		       rowpos[4], rowpos[5], rowpos[6], rowpos[7]);
		rowpos += 8;
	}
}
#endif
