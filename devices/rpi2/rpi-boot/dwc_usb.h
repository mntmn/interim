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

#ifndef DWC_USB_H
#define DWC_USB_H

// The FIFOs - one page per FIFO
#define DWC_USB_FIFO_SIZE		0x1000
#define DWC_USB_FIFO_LEN		(3 * DWC_USB_FIFO_SIZE)
#define DWC_USB_FIFO_START		(0x100000 - DWC_USB_FIFO_LEN)

// Register addresses
#define DWC_USB_BASE			0x20980000
#define DWC_USB_OTG_CTRL		0
#define DWC_USB_OTG_IRPT		4
#define DWC_USB_AHB_CONF		8
#define DWC_USB_CORE_CONF		0xC
#define DWC_USB_CORE_RESET		0x10
#define DWC_USB_CORE_IRPT		0x14
#define DWC_USB_CORE_IRPT_MASK		0x18
#define DWC_USB_RECV_STATUS_DBG		0x1C
#define DWC_USB_STATUS_READ_POP		0x1C
#define DWC_USB_DEVICE_STATUS_READ_POP	0x20
#define DWC_USB_RECV_FIFO_SIZE		0x24
#define DWC_USB_NON_PERIODIC_FIFO_SIZE	0x28
#define DWC_USB_NON_PERIODIC_FIFO	0x2C
#define DWC_USB_I2C_ACCESS		0x30
#define DWC_USB_PHY_VENDOR_CONTROL	0x34
#define DWC_USB_GPIO			0x38
#define DWC_USB_USER_ID			0x3C
#define DWC_USB_SYNOPSYS_ID		0x40
#define DWC_USB_CONFIG_1		0x44
#define DWC_USB_CONFIG_2		0x48
#define DWC_USB_CONFIG_3		0x4C
#define DWC_USB_CONFIG_4		0x50
#define DWC_USB_PERIODIC_FIFO_SIZE	0x100

#define DWC_USB_HOST_CONF		0x400
#define DWC_USB_HOST_FRAME_INTERVAL	0x404
#define DWC_USB_HOST_FRAME_NUMBER	0x408
#define DWC_USB_HOST_PERIODIC_FIFO	0x410
#define DWC_USB_HOST_ALL_CHAN_IRPT	0x414
#define DWC_USB_HOST_IPRT_MASK		0x418
#define DWC_USB_HOST_FRAME_LIST		0x41C
#define DWC_USB_HOST_PORT_CTRL_STATUS	0x440

#define DWC_USB_HOST_CHAN_CHAR		0x500
#define DWC_USB_HOST_CHAN_SPLIT_CTRL	0x504
#define DWC_USB_HOST_CHAN_IRPT		0x508
#define DWC_USB_HOST_CHAN_IRPT_MASK	0x50C
#define DWC_USB_HOST_CHAN_TFER_SIZE	0x510
#define DWC_USB_HOST_CHAN_DMA_ADDR	0x514

#define DWC_USB_POWER			0xE00

// DWC_USB_HOST_PORT_CTRL_STATUS bits
#define DWC_USB_HPCS_PRTCONNSTS		(1 << 0)
#define DWC_USB_HPCS_PRTCONNDET		(1 << 1)
#define DWC_USB_HPCS_PRTENA			(1 << 2)
#define DWC_USB_HPCS_PRTENCHNG		(1 << 3)
#define DWC_USB_HPCS_PRTOVRCURACT		(1 << 4)
#define DWC_USB_HPCS_PRTOVRCURCHNG		(1 << 5)
#define DWC_USB_HPCS_PRTRES			(1 << 6)
#define DWC_USB_HPCS_PRTSUSP		(1 << 7)
#define DWC_USB_HPCS_PRTRST			(1 << 8)
#define DWC_USB_HPCS_PRTLNSTS		(3 << 10)
#define DWC_USB_HPCS_PRTPWR			(1 << 12)
#define DWC_USB_HPCS_PRTTSTCTL		(15 << 13)
#define DWC_USB_HPCS_PRTSPD			(3 << 17)

#endif

