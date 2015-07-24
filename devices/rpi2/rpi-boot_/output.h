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

#ifndef OUTPUT_H
#define OUTPUT_H

#include <stdint.h>

typedef uint32_t rpi_boot_output_state;

#ifdef BUILDING_RPIBOOT
rpi_boot_output_state output_get_state();
void output_restore_state(rpi_boot_output_state state);
void output_enable_fb();
void output_disable_fb();
void output_enable_uart();
void output_disable_uart();
void output_enable_log();
void output_disable_log();
void output_enable_custom();
void output_disable_custom();
void output_init();
int split_putc(int c);
int register_custom_output_function(int (*putc_function)(int c));
#endif

#define RPIBOOT_OUTPUT_FB      (1 << 0)
#define RPIBOOT_OUTPUT_UART    (1 << 1)
#define RPIBOOT_OUTPUT_LOG		(1 << 2)
#define RPIBOOT_OUTPUT_CUSTOM	(1 << 3)

#endif
