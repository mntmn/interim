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

/* RPi USB host controller driver
 *
 * The controller is a Synopsys DWC USB 2 OTG
 *
 * References:
 * 	Altera cv_54018-1.2, chapter 12, USB 2.0 OTG controller
 * 	Raspberry Pi - USB Controller ver 1.03 by Luke Robertson
 * 	BCM2835 Peripherals Guide
 * 	Chadderz USB driver - https://github.com/Chadderz121/csud
 */

#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "mmio.h"
#include "mbox.h"
#include "usb.h"
#include "dwc_usb.h"
#include "timer.h"

#define SYNOPSYS_ID	0x4f54280a
#define USER_ID		0x2708a000

#ifdef DEBUG2
#define DWC_USB_DEBUG
#endif

struct dwc_usb_hcd
{
	struct usb_hcd		b;

	uint32_t		base;

	uint32_t		config1;
	uint32_t		config2;
	uint32_t		config3;
	uint32_t		config4;
};

static int hcd_id = 0;
static char driver_name[] = "dwc_usb";

static int dwc_usb_reset(struct usb_hcd *dev);
static int dwc_usb_send_req(struct usb_hcd *dev, struct usb_request *req);
static int dwc_usb_post_reset_init(struct dwc_usb_hcd *d);

int dwc_usb_init(struct usb_hcd **dev, uint32_t base)
{
	// First check we have a valid controller
	uint32_t vendor = mmio_read(base + DWC_USB_SYNOPSYS_ID);
	uint32_t userid = mmio_read(base + DWC_USB_USER_ID);

	if(vendor != SYNOPSYS_ID)
	{
		printf("DWC_USB: invalid vendor id %08x\n", vendor);
		return -1;
	}

	if(userid != USER_ID)
	{
		printf("DWC_USB: invalid used id %08x\n", userid);
		return -1;
	}

	// Build the device structure
	int cur_hcd_id = hcd_id++;

	// Limit driver interfaces to 1 currently as we use fixed FIFO addresses
	if(cur_hcd_id >= 1)
	{
		printf("DWC_USB: too many host controllers registered\n");
		return -1;
	}
	struct dwc_usb_hcd *ret =
		(struct dwc_usb_hcd *)malloc(sizeof(struct dwc_usb_hcd));
	ret->b.driver_name = driver_name;
	char *device_name = malloc(strlen(driver_name) + 2);
	strncpy(device_name, driver_name, strlen(driver_name));
	device_name[strlen(driver_name)] = '0' + cur_hcd_id;
	device_name[strlen(driver_name) + 1] = '\0';
	ret->b.device_name = device_name;
	ret->base = base;
	ret->config1 = mmio_read(base + DWC_USB_CONFIG_1);
	ret->config2 = mmio_read(base + DWC_USB_CONFIG_2);
	ret->config3 = mmio_read(base + DWC_USB_CONFIG_3);
	ret->config4 = mmio_read(base + DWC_USB_CONFIG_4);

	ret->b.reset = dwc_usb_reset;
	ret->b.send_req = dwc_usb_send_req;

	// Mask the interrupt line to the ARM
	uint32_t ahb_conf = mmio_read(base + DWC_USB_AHB_CONF);
	ahb_conf &= ~0x1;
	usleep(2000);
	mmio_write(base + DWC_USB_AHB_CONF, ahb_conf);

	// Enable all interrupts
	usleep(2000);
	mmio_write(base + DWC_USB_CORE_IRPT_MASK, 0xffffffff);

	// Power on the USB via the mailbox interface (undocumented - from csud)
	mbox_write(0, 0x80);
	if(mbox_read(0) != 0x80)
	{
		printf("DWC_USB: unable to power up USB\n");
		return -1;
	}

	// Reset the controller
	if(dwc_usb_reset((struct usb_hcd *)ret) != 0)
	{
		printf("DWC_USB: usb_reset() failed\n");
		return -1;
	}

	// Return success
	printf("DWC_USB: Initialized host controller %s\n", ret->b.device_name);
	*dev = (struct usb_hcd *)ret;

#ifdef DWC_USB_DEBUG
	usleep(2000000);		// Pause so we can see the screen
#endif
	return 0;	
}

int dwc_usb_post_reset_init(struct dwc_usb_hcd *d)
{
#ifdef DWC_USB_DEBUG
	printf("DWC_USB: beginning post reset init\n");
#endif

	// Clear all interrupts
	usleep(2000);
	mmio_write(d->base + DWC_USB_CORE_IRPT, 0xffffffff);

	// Clear the power register (write StopPClock - csud does this)
	usleep(2000);
	mmio_write(d->base + DWC_USB_POWER, 0);

	// Initialize the FIFOs
	usleep(2000);
	mmio_write(d->base + DWC_USB_RECV_FIFO_SIZE, DWC_USB_FIFO_SIZE << 16);
	usleep(2000);
	mmio_write(d->base + DWC_USB_NON_PERIODIC_FIFO_SIZE,
			(DWC_USB_FIFO_START & 0xffff) | (DWC_USB_FIFO_SIZE << 16));
	usleep(2000);
	mmio_write(d->base + DWC_USB_PERIODIC_FIFO_SIZE,
			((DWC_USB_FIFO_START + DWC_USB_FIFO_SIZE) & 0xffff) |
			(DWC_USB_FIFO_SIZE << 16));

	// Power on the host port
	usleep(2000);
	uint32_t ctrl_status = mmio_read(d->base + DWC_USB_HOST_PORT_CTRL_STATUS);
	ctrl_status |= DWC_USB_HPCS_PRTPWR;
	mmio_write(d->base + DWC_USB_HOST_PORT_CTRL_STATUS, ctrl_status);

#ifdef DWC_USB_DEBUG
	printf("DWC_USB: completed post reset init\n");
#endif

	return 0;
}

int dwc_usb_reset(struct usb_hcd *dev)
{
	struct dwc_usb_hcd *d = (struct dwc_usb_hcd *)dev;

#ifdef DWC_USB_DEBUG
	printf("DWC_USB: beginning core reset\n");
#endif

	// Wait for the AHB IDLE state
	TIMEOUT_WAIT(mmio_read(d->base + DWC_USB_CORE_RESET) & (1 << 31), 500000);
	if((mmio_read(d->base + DWC_USB_CORE_RESET) & (1 << 31)) == 0)
	{
		printf("DWC_USB: reset(): timeout waiting for AHB IDLE state\n");
		return -1;
	}

	// Raise the core soft reset bit high
	uint32_t reset = mmio_read(d->base + DWC_USB_CORE_RESET);
	reset |= 0x1;
	mmio_write(d->base + DWC_USB_CORE_RESET, reset);
	usleep(2000);

	// Wait for core reset to go low and AHB IDLE high
	int success = 0;
	struct timer_wait *tw = register_timer(500000);
	do
	{
		reset = mmio_read(d->base + DWC_USB_CORE_RESET);
		if(((reset & 0x1) == 0) && ((reset & (1 << 31)) != 0))
		{
			success = 1;
			break;
		}
	} while(!compare_timer(tw));
	free(tw);

	if(!success)
	{
		printf("DWC_USB: reset(): timeout waiting for reset (%08x)\n", reset);
		return -1;
	}

#ifdef DWC_USB_DEBUG
	printf("DWC_USB: core reset complete\n");
#endif

	dwc_usb_post_reset_init(d);

	return 0;
}

int dwc_usb_send_req(struct usb_hcd *dev, struct usb_request *req)
{
	// TODO
	(void)dev;
	(void)req;
	printf("DWC_USB: usb_send_req(): unimplemented\n");
	while(1);

	return 0;
}

