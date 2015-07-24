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

#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include "mbox.h"
#include "fb.h"

#define WIDTH		640
#define HEIGHT		480
#define BYTES_PER_PIXEL	2
#define BPP		(BYTES_PER_PIXEL << 3)
#define S_PITCH		(WIDTH * BYTES_PER_PIXEL)	// The pitch of the backbuffer

#define TAG_ALLOCATE_BUFFER		0x40001
#define TAG_RELEASE_BUFFER		0x48001
#define TAG_BLANK_SCREEN		0x40002
#define TAG_GET_PHYS_WH			0x40003
#define TAG_TEST_PHYS_WH		0x44003
#define TAG_SET_PHYS_WH			0x48003
#define TAG_GET_VIRT_WH			0x40004
#define TAG_TEST_VIRT_WH		0x44004
#define TAG_SET_VIRT_WH			0x48004
#define TAG_GET_DEPTH			0x40005
#define TAG_TEST_DEPTH			0x44005
#define TAG_SET_DEPTH			0x48005
#define TAG_GET_PIXEL_ORDER		0x40006
#define TAG_TEST_PIXEL_ORDER		0x44006
#define TAG_SET_PIXEL_ORDER		0x48006
#define TAG_GET_ALPHA_MODE		0x40007
#define TAG_TEST_ALPHA_MODE		0x44007
#define TAG_SET_ALPHA_MODE		0x48007
#define TAG_GET_PITCH			0x40008
#define TAG_GET_VIRT_OFFSET		0x40009
#define TAG_TEST_VIRT_OFFSET		0x44009
#define TAG_SET_VIRT_OFFSET		0x48009
#define TAG_GET_OVERSCAN		0x4000a
#define TAG_TEST_OVERSCAN		0x4400a
#define TAG_SET_OVERSCAN		0x4800a
#define TAG_GET_PALETTE			0x4000b
#define TAG_TEST_PALETTE		0x4400b
#define TAG_SET_PALETTE			0x4800b

static uint32_t phys_w, phys_h, virt_w, virt_h, pitch;
static uint32_t fb_addr, fb_size;

int fb_init()
{
	// define a mailbox buffer
	uint32_t mb_addr = 0x40007000;		// 0x7000 in L2 cache coherent mode
	volatile uint32_t *mailbuffer = (uint32_t *)mb_addr;

	/* Get the display size */
	// set up the buffer
	mailbuffer[0] = 8 * 4;			// size of this message
	mailbuffer[1] = 0;			// this is a request

	// next comes the first tag
	mailbuffer[2] = TAG_GET_PHYS_WH;	// get physical width/height tag
	mailbuffer[3] = 0x8;			// value buffer size
	mailbuffer[4] = 0;			// request/response
	mailbuffer[5] = 0;			// space to return width
	mailbuffer[6] = 0;			// space to return height

	// closing tag
	mailbuffer[7] = 0;

	// send the message
	mbox_write(MBOX_PROP, mb_addr);

	// read the response
	mbox_read(MBOX_PROP);

	/* Check for a valid response */
	if(mailbuffer[1] != MBOX_SUCCESS)
		return FB_FAIL_GET_RESOLUTION;
	phys_w = mailbuffer[5];
	phys_h = mailbuffer[6];

	/* Request 640x480 if not otherwise specified */
	if((phys_w == 0) && (phys_h == 0))
	{
		phys_w = WIDTH;
		phys_h = HEIGHT;
	}

	if((phys_w == 0) || (phys_h == 0))
		return FB_FAIL_INVALID_RESOLUTION;

	/* For now set the physical and virtual sizes to be the same */
	virt_w = phys_w;
	virt_h = phys_h;

	/* Now set the physical and virtual sizes and bit depth and allocate the framebuffer */
	mailbuffer[0] = 22 * 4;		// size of buffer
	mailbuffer[1] = 0;		// request

	mailbuffer[2] = TAG_SET_PHYS_WH;
	mailbuffer[3] = 8;
	mailbuffer[4] = 8;
	mailbuffer[5] = phys_w;
	mailbuffer[6] = phys_h;

	mailbuffer[7] = TAG_SET_VIRT_WH;
	mailbuffer[8] = 8;
	mailbuffer[9] = 8;
	mailbuffer[10] = virt_w;
	mailbuffer[11] = virt_h;

	mailbuffer[12] = TAG_SET_DEPTH;
	mailbuffer[13] = 4;
	mailbuffer[14] = 4;
	mailbuffer[15] = BPP;

	mailbuffer[16] = TAG_ALLOCATE_BUFFER;
	mailbuffer[17] = 8;
	mailbuffer[18] = 4;	// request size = 4, response size = 8
	mailbuffer[19] = 16;	// requested alignment of buffer, space for returned address
	mailbuffer[20] = 0;	// space for returned size

	mailbuffer[21] = 0;	// terminating tag

	mbox_write(MBOX_PROP, mb_addr);
	mbox_read(MBOX_PROP);

	/* Validate the response */
	if(mailbuffer[1] != MBOX_SUCCESS)
		return FB_FAIL_SETUP_FB;

	/* Check the allocate_buffer response */
	if(mailbuffer[18] != (MBOX_SUCCESS | 8))
		return FB_FAIL_INVALID_TAG_RESPONSE;

	fb_addr = mailbuffer[19];
	fb_size = mailbuffer[20];

	if((fb_addr == 0) || (fb_size == 0))
		return FB_FAIL_INVALID_TAG_DATA;

	/* Get the pitch of the display */
	mailbuffer[0] = 7 * 4;
	mailbuffer[1] = 0;

	mailbuffer[2] = TAG_GET_PITCH;
	mailbuffer[3] = 4;
	mailbuffer[4] = 0;
	mailbuffer[5] = 0;

	mailbuffer[6] = 0;

	mbox_write(MBOX_PROP, mb_addr);
	mbox_read(MBOX_PROP);

	/* Validate the response */
	if(mailbuffer[1] != MBOX_SUCCESS)
		return FB_FAIL_INVALID_PITCH_RESPONSE;
	if(mailbuffer[4] != (MBOX_SUCCESS | 4))
		return FB_FAIL_INVALID_PITCH_RESPONSE;

	pitch = mailbuffer[5];
	if(pitch == 0)
		return FB_FAIL_INVALID_PITCH_DATA;

	return 0;
}

int fb_get_bpp()
{
	return BPP;
}

int fb_get_byte_size()
{
	return virt_w * virt_h * BYTES_PER_PIXEL;
}

int fb_get_width()
{
	return virt_w;
}

int fb_get_height()
{
	return virt_h;
}

int fb_get_pitch()
{
	return pitch;
}

uint8_t *fb_get_framebuffer()
{
	return (uint8_t *)fb_addr;
}

