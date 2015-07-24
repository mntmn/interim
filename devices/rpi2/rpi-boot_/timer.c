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

#include "timer.h"
#include "mmio.h"
#include <errno.h>
#include <stdint.h>
#include <stdlib.h>

#define useconds_t int

#define TIMER_CLO		0x3f003004

int usleep(useconds_t usec)
{
	struct timer_wait tw = register_timer(usec);
	while(!compare_timer(tw));
	return 0;	
}

struct timer_wait register_timer(useconds_t usec)
{
	struct timer_wait tw;
	tw.rollover = 0;
	tw.trigger_value = 0;

	if(usec < 0)
	{
		errno = EINVAL;
		return tw;
	}
	uint32_t cur_timer = mmio_read(TIMER_CLO);
	uint32_t trig = cur_timer + (uint32_t)usec;

	tw.trigger_value = trig;
	if(trig > cur_timer)
		tw.rollover = 0;
	else
		tw.rollover = 1;
	return tw;
}

int compare_timer(struct timer_wait tw)
{
	uint32_t cur_timer = mmio_read(TIMER_CLO);

	if(cur_timer < tw.trigger_value)
	{
		if(tw.rollover)
			tw.rollover = 0;
	}
	else if(!tw.rollover)
		return 1;

	return 0;
}

