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

#ifndef FB_H
#define FB_H

#define FB_FAIL_GET_RESOLUTION		-1
#define FB_FAIL_INVALID_RESOLUTION	-2
#define FB_FAIL_SETUP_FB		-3
#define FB_FAIL_INVALID_TAGS		-4
#define FB_FAIL_INVALID_TAG_RESPONSE	-5
#define FB_FAIL_INVALID_TAG_DATA	-6
#define FB_FAIL_INVALID_PITCH_RESPONSE	-7
#define FB_FAIL_INVALID_PITCH_DATA	-8

uint8_t *fb_get_framebuffer();
int fb_init();
int fb_get_bpp();
int fb_get_byte_size();
int fb_get_width();
int fb_get_height();
int fb_get_pitch();

#endif

