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

/* Based on an idea by https://github.com/JamesC1 */

/* Support for writing the console log to a file.
 *
 * The main problem here is that some devices (e.g. SD cards) have limited
 * write cycles, so writing every character to them is problematic.  Thus, we
 * buffer writes to try and do, e.g. one block at a time.  The problem here is
 * that if the guest OS crashes at some point, the buffer may not be flushed to
 * disk.  We try and ameliorate this by 1) providing the guest OS with a
 * fflush() function to flush at important moments, and 2) automatically
 * flushing if log_putc is called and it has been more than a certain time
 * since the last flush.
 *
 * This can still miss certain log outputs, but is probably the best
 * compromise.  The ideal situation would be to have a background thread
 * periodically flushing the buffer to disk, but as we are only a bootloader,
 * we cannot hijack the timer interrupt that the guest OS would probably want
 * to use itself.
 */

#include <stdint.h>
#include <stdlib.h>
#include "timer.h"
#include "vfs.h"
#include "output.h"

FILE *log_fp = NULL;
uint8_t *log_buf = NULL;
size_t buf_size;
size_t buf_ptr;
struct timer_wait last_update;

// Time between flushes (requires that log_putc is actually called at some point)
#define LOG_TIMEOUT			5000000

int log_putc(int c)
{
	if(last_update.trigger_value == 0)
		last_update = register_timer(LOG_TIMEOUT);

	if(log_buf && buf_size)
	{
		log_buf[buf_ptr++] = c;
		if(buf_ptr >= buf_size)
		{
			if((log_fp && log_fp->fflush_cb) || (last_update.rollover && compare_timer(last_update)))
			{
				log_fp->fflush_cb(log_fp);
				last_update = register_timer(LOG_TIMEOUT);
			}
			buf_ptr = 0;
		}
		return 0;
	}
	else if(log_fp)
	{
		// Disable output to the log for the write
		rpi_boot_output_state state = output_get_state();
		output_disable_log();

		// Write one character
		fwrite(&c, 1, 1, log_fp);

		// Restore saved output state
		output_restore_state(state);
		return 0;
	}
	return EOF;
}

static int log_fflush(FILE *fp)
{
	// Flush the buffer
	if(log_fp && log_buf)
	{
		// Disable output to the log for the write
		rpi_boot_output_state state = output_get_state();
		output_disable_log();

		// Write the buffer
		fwrite(log_buf, 1, buf_ptr, fp);

		// Restore the state
		output_restore_state(state);
	}
	return 0;
}

int register_log_file(FILE *fp, size_t buffer_size)
{
	// If we have a current log, flush it
	if(log_fp)
	{
		fflush(log_fp);

		// deregister fflush callback
		log_fp->fflush_cb = NULL;
	}

	// If passed NULL, then set no log file
	if(fp == NULL)
	{
		if(log_buf)
			free(log_buf);

		// We can still use a buffer without a file, for flushing
		//  later to the file
		if(buffer_size)
			log_buf = (uint8_t *)malloc(buffer_size);
		else
			log_buf = NULL;
		buf_size = buffer_size;
		buf_ptr = 0;
		log_fp = NULL;
		return 0;
	}
	
	// Store the fflush callback
	fp->fflush_cb = log_fflush;

	// If no current log, and there is a buffer, then flush
	//  what's in it to the new file
	if(!log_fp && log_buf)
	{
		log_fp = fp;
		fflush(fp);
	}

	// If we have a buffer free it
	if((buf_size != buffer_size) && log_buf)
		log_buf = (uint8_t *)realloc(log_buf, buffer_size);
	else if(log_buf == NULL)
		log_buf = (uint8_t *)malloc(buffer_size);

	buf_size = buffer_size;
	buf_ptr = 0;

	// Store the log file pointer
	log_fp = fp;


	return 0;
}

FILE *get_log_file()
{
	return log_fp;
}
