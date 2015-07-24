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

/* This defines the basic usb structures and functions */

#ifndef USB_H
#define USB_H

#define USB_REQ_TYPE_CONTROL		0
#define USB_REQ_TYPE_BULK		1
#define USB_REQ_TYPE_INTERRUPT		2
#define USB_REQ_TYPE_ISO		3

#define USB_REQ_DIR_HD			0
#define USB_REQ_DIR_DH			1

struct usb_hcd;

struct usb_request
{
	struct usb_hcd 	*hcd;
	uint32_t	pipe;

	int		type;
	int		direction;
};

struct usb_hcd
{
	char	*driver_name;
	char	*device_name;

	int	(*send_req)(struct usb_hcd *, struct usb_request *);
	int	(*reset)(struct usb_hcd *);
};


#endif

