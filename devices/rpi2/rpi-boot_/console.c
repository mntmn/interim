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
#include <stdint.h>
#include <stdio.h>
#include "fb.h"
#include "console.h"
#include "util.h"

#ifdef ENABLE_DEFAULT_FONT
extern uint8_t vgafont8[];
#define CHAR_W		8
#define CHAR_H		8
#define FONT		vgafont8
#endif

#ifdef ENABLE_ALTERNATIVE_FONT
#ifdef ENABLE_DEFAULT_FONT
#error Cannot enable both the default and alternative fonts
#endif
#define FONT		altfont
extern uint8_t		FONT[];
#define CHAR_W		ALTERNATIVE_FONT_W
#define CHAR_H		ALTERNATIVE_FONT_H
#ifdef ALTERNATIVE_FONT_LSB_LEFT
#define FONT_LSB_LEFT
#endif
#endif

#ifndef FONT
#error No console font is defined! Please see config.h
#endif

// Some fonts are encoded that the least significant byte is to the left, others have it to the right
#ifdef FONT_LSB_LEFT
#define BIT_SHIFT (s_bit_no)
#else
#define BIT_SHIFT (7 - s_bit_no)
#endif

#define DEF_FORE	0xffffffff
#define DEF_BACK	0x00000000

static int cur_x = 0;
static int cur_y = 0;

static uint32_t cur_fore = DEF_FORE;
static uint32_t cur_back = DEF_BACK;

void clear()
{
	int height = fb_get_height();
	int pitch = fb_get_pitch();
	int line_byte_width = fb_get_width() * (fb_get_bpp() >> 3);
	uint8_t *fb = (uint8_t *)fb_get_framebuffer();

	for(int line = 0; line < height; line++)
		memset(&fb[line * pitch], 0, line_byte_width);

	cur_x = 0;
	cur_y = 0;
}

#ifdef FONT
void newline()
{
	cur_y++;
	cur_x = 0;

	// Scroll up if necessary
	if(cur_y == fb_get_height() / CHAR_H)
	{
		uint8_t *fb = (uint8_t *)fb_get_framebuffer();
		int line_byte_width = fb_get_width() * (fb_get_bpp() >> 3);
		int pitch = fb_get_pitch();
		int height = fb_get_height();

		for(int line = 0; line < (height - CHAR_H); line++)
			quick_memcpy(&fb[line * pitch], &fb[(line + CHAR_H) * pitch], line_byte_width);
		for(int line = height - CHAR_H; line < height; line++)
			memset(&fb[line * pitch], 0, line_byte_width);

		cur_y--;
	}
}

int console_putc(int c)
{
	int line_w = fb_get_width() / CHAR_W;

	if(c == '\n')
		newline();
	else
	{
		draw_char((char)c, cur_x, cur_y, cur_fore, cur_back);
		cur_x++;
		if(cur_x == line_w)
			newline();
	}

	return c;
}

void draw_char(char c, int x, int y, uint32_t fore, uint32_t back)
{
	volatile uint8_t *fb = (uint8_t *)fb_get_framebuffer();
	int bpp = fb_get_bpp();
	int bytes_per_pixel = bpp >> 3;

	int d_offset = y * CHAR_H * fb_get_pitch() + x * bytes_per_pixel * CHAR_W;
	int line_d_offset = d_offset;
	int s_offset = (int)c * CHAR_W * CHAR_H;

	for(int c_y = 0; c_y < CHAR_H; c_y++)
	{
		d_offset = line_d_offset;

		for(int c_x = 0; c_x < CHAR_W; c_x++)
		{
			int s_byte_no = s_offset / 8;
			int s_bit_no = s_offset % 8;

			uint8_t s_byte = FONT[s_byte_no];
			uint32_t colour = back;
			if((s_byte >> BIT_SHIFT) & 0x1)
				colour = fore;

			for(int i = 0; i < bytes_per_pixel; i++)
			{
				fb[d_offset + i] = (uint8_t)(colour & 0xff);
				colour >>= 8;
			}	

			d_offset += bytes_per_pixel;
			s_offset++;
		}

		line_d_offset += fb_get_pitch();
	}
}
#endif
