/*
 * (C) Copyright 2000
 * Wolfgang Denk, DENX Software Engineering, wd@denx.de.
 *
 * SPDX-License-Identifier:	GPL-2.0+
 */

#include <stdint.h>

//#include <common.h>
//#include <exports.h>
#include <video_fb.h>

//#define BF_CS1(addr,mask,value) (value? (*(addr) |= mask) : (*(addr) ^= mask))

#include "io.h"
#include "arch/clock.h"
#include "arch/imx-regs.h"
//#include "arch/sys_proto.h"
//#include "asm/imx-common/dma.h"

/* 1 second delay should be plenty of time for block reset. */
#define	RESET_MAX_TIMEOUT	1000000

#define	MXS_BLOCK_SFTRST	(1 << 31)
#define	MXS_BLOCK_CLKGATE	(1 << 30)

#define	PS2KHZ(ps)	(1000000000UL / (ps))

/*
 * The PLL frequency is 480MHz and XTAL frequency is 24MHz
 *   iMX23: datasheet section 4.2
 *   iMX28: datasheet section 10.2
 */
#define	PLL_FREQ_KHZ	480000
#define	PLL_FREQ_COEF	18
#define	XTAL_FREQ_KHZ	24000

#define	PLL_FREQ_MHZ	(PLL_FREQ_KHZ / 1000)
#define	XTAL_FREQ_MHZ	(XTAL_FREQ_KHZ / 1000)
#define MXC_SSPCLK_MAX MXC_SSPCLK0

/******************************************************************
 * Resolution Struct
 ******************************************************************/
struct ctfb_res_modes {
	int xres;		/* visible resolution		*/
	int yres;
	int refresh;		/* vertical refresh rate in hz  */
	/* Timing: All values in pixclocks, except pixclock (of course) */
	int pixclock;		/* pixel clock in ps (pico seconds) */
	int pixclock_khz;	/* pixel clock in kHz           */
	int left_margin;	/* time from sync to picture	*/
	int right_margin;	/* time from picture to sync	*/
	int upper_margin;	/* time from sync to picture	*/
	int lower_margin;
	int hsync_len;		/* length of horizontal sync	*/
	int vsync_len;		/* length of vertical sync	*/
	int sync;		/* see FB_SYNC_*		*/
	int vmode;		/* see FB_VMODE_*		*/
};

static void mxs_lcd_init(GraphicDevice *panel, struct ctfb_res_modes *mode, int bpp);

int mxs_wait_mask_set(struct mxs_register_32 *reg, uint32_t mask, unsigned
                      int timeout);
int mxs_wait_mask_clr(struct mxs_register_32 *reg, uint32_t mask, unsigned
                      int timeout);
int mxs_reset_block(struct mxs_register_32 *reg);
void mxs_set_lcdclk(uint32_t freq);

void blit_string(char* str, int len, int x, int y);
void *memset(void *s, int c, size_t n);

#define FRAMEBUFFER (uint8_t*)0x40003e80
#define FB_PITCH 640

int hello_world (int argc, char * const argv[])
{
	int i;

  struct ctfb_res_modes mode;

/*
 * DENX M28EVK:
 * setenv videomode
 * video=ctfb:x:800,y:480,depth:18,mode:0,pclk:30066,
 *       le:0,ri:256,up:0,lo:45,hs:1,vs:1,sync:100663296,vmode:0
 *
 * Freescale mx23evk/mx28evk with a Seiko 4.3'' WVGA panel:
 * setenv videomode
 * video=ctfb:x:800,y:480,depth:24,mode:0,pclk:29851,
 * 	 le:89,ri:164,up:23,lo:10,hs:10,vs:10,sync:0,vmode:0
 */

  // http://tinyvga.com/vga-timing/640x480@60Hz

  // success!
  // green line: mw.b 0x40003e80 0x04 0x280

  // loadx 0x43000000
  // go 0x43000000
  
  mode.xres = 640;
  mode.yres = 480;
  mode.refresh = 31468; // seems unused
  mode.pixclock_khz = 25175; // unused, see picoseconds
  mode.hsync_len = 96;
  mode.vsync_len = 2;
  mode.pixclock = 39721; // picoseconds
  mode.lower_margin = 33;
  mode.upper_margin = 10;
  mode.left_margin = 48;
  mode.right_margin = 16;

  // exec !! sx /home/mntmn/code/u-boot/examples/standalone/hello_world.bin

  GraphicDevice gdevice; // only framebuffer is relevant here

  gdevice.frameAdrs = (uint32_t)(FRAMEBUFFER-0x3e80);

  /*
    PINS:

    CON1
    28 VSYNC
    29 HSYNC

    4-11 DATA (RGB)
  */ 

  //printf ("Interim? MXS_LCDIF_BASE at %p\n", (void*)MXS_LCDIF_BASE);

  mxs_lcd_init(&gdevice, &mode, 8);

  //printf("mxs_lcd_init done!\n");

  //&imx_ccm->cgr3 |= (MXC_CCM_CCGR3_LCDIF1_PIX_MASK | MXC_CCM_CCGR3_DISP_AXI_MASK);
  //&imx_ccm->cgr2 |= (MXC_CCM_CCGR2_LCD_MASK);

  /*
	mxs_set_lcdclk(PS2KHZ(mode->pixclock));
	mxs_reset_block(&lcdif->hw_lcdif_ctrl_reg);

  
  //enable_lcdif_clock(&lcdif->hw_lcdif_ctrl);

  BF_CS1(&lcdif->hw_lcdif_ctrl, LCDIF_CTRL_DOTCLK_MODE, 1);
  BF_CS1(&lcdif->hw_lcdif_ctrl, LCDIF_CTRL_BYPASS_COUNT, 1);
  BF_CS1(&lcdif->hw_lcdif_vdctrl0, LCDIF_VDCTRL0_VSYNC_OEB, 0); //Vsync is always an output in the DOTCLK mode

  BF_CS1(&lcdif->hw_lcdif_vdctrl0, LCDIF_VDCTRL0_VSYNC_POL, 0);
  BF_CS1(&lcdif->hw_lcdif_vdctrl0, LCDIF_VDCTRL0_HSYNC_POL, 0);
  BF_CS1(&lcdif->hw_lcdif_vdctrl0, LCDIF_VDCTRL0_DOTCLK_POL, 0);
  BF_CS1(&lcdif->hw_lcdif_vdctrl0, LCDIF_VDCTRL0_ENABLE_POL, 0);

  BF_CS1(&lcdif->hw_lcdif_vdctrl0, LCDIF_VDCTRL0_ENABLE_PRESENT, 1);
  BF_CS1(&lcdif->hw_lcdif_vdctrl0, LCDIF_VDCTRL0_VSYNC_PERIOD_UNIT, 1);
  BF_CS1(&lcdif->hw_lcdif_vdctrl0, LCDIF_VDCTRL0_VSYNC_PULSE_WIDTH_UNIT, 1);
  
  lcdif->hw_lcdif_vdctrl0 ^= LCDIF_VDCTRL0_VSYNC_PULSE_WIDTH_MASK;
  lcdif->hw_lcdif_vdctrl0 |= 2<<LCDIF_VDCTRL0_VSYNC_PULSE_WIDTH_OFFSET;
  
  lcdif->hw_lcdif_vdctrl1 ^= LCDIF_VDCTRL1_VSYNC_PERIOD_MASK;
  lcdif->hw_lcdif_vdctrl1 |= 280<<LCDIF_VDCTRL1_VSYNC_PERIOD_OFFSET;

  lcdif->hw_lcdif_vdctrl2 ^= LCDIF_VDCTRL2_HSYNC_PULSE_WIDTH_MASK;
  lcdif->hw_lcdif_vdctrl2 |= 10<<LCDIF_VDCTRL2_HSYNC_PULSE_WIDTH_OFFSET;
  
  lcdif->hw_lcdif_vdctrl2 ^= LCDIF_VDCTRL2_HSYNC_PERIOD_MASK;
  lcdif->hw_lcdif_vdctrl2 |= 360<<LCDIF_VDCTRL2_HSYNC_PERIOD_OFFSET;
  
  //Assuming LCD_DATABUS_WIDTH
  //is 24bit

  BF_CS1(&lcdif->hw_lcdif_vdctrl3, LCDIF_VDCTRL3_VSYNC_ONLY, 0);
  
  lcdif->hw_lcdif_vdctrl3 ^= LCDIF_VDCTRL3_HORIZONTAL_WAIT_CNT_MASK;
  lcdif->hw_lcdif_vdctrl3 |= 20<<LCDIF_VDCTRL3_HORIZONTAL_WAIT_CNT_OFFSET;
  
  lcdif->hw_lcdif_vdctrl3 ^= LCDIF_VDCTRL3_VERTICAL_WAIT_CNT_MASK;
  lcdif->hw_lcdif_vdctrl3 |= 20<<LCDIF_VDCTRL3_VERTICAL_WAIT_CNT_OFFSET;
  
  lcdif->hw_lcdif_vdctrl4 ^= LCDIF_VDCTRL4_DOTCLK_H_VALID_DATA_CNT_MASK;
  lcdif->hw_lcdif_vdctrl4 |= 320<<LCDIF_VDCTRL4_DOTCLK_H_VALID_DATA_CNT_OFFSET;
  
  //Note that DOTCLK_V_VALID_DATA_CNT is
  //implicitly assumed to be HW_LCDIF_TRANSFER_COUNT_V_COUNT

  BF_CS1(&lcdif->hw_lcdif_vdctrl4, LCDIF_VDCTRL4_SYNC_SIGNALS_ON, 1);
  BF_CS1(&lcdif->hw_lcdif_ctrl, LCDIF_CTRL_RUN, 1);

  // HW_LCDIF_CUR_BUF*/

  
	//printf("Enter + to exit ... \n");

  //memset(FRAMEBUFFER, 0, 640*480);
  
  int cursor_x=0;
  int cursor_y=32;
  char* greeting = "Welcome to Interim/IMX 0.0.1!";
  blit_string(greeting,29,0,16);
  
  while (true) {
    while (!tstc());
    /* consume input */
    char c = getc();
    blit_string(&c,1,cursor_x,cursor_y);
    cursor_x+=8;
    if (cursor_x>=640) {
      cursor_x=0;
      cursor_y+=8;
    }
    if (c==10 || c==13) {
      cursor_x=0;
      cursor_y+=8;
    }
    if (c=='+') {
      //printf ("\n\n");
      return(0);
    }
  }

	return (0);
}


static void mxs_lcd_init(GraphicDevice *panel,
			struct ctfb_res_modes *mode, int bpp)
{
	struct mxs_lcdif_regs *regs = (struct mxs_lcdif_regs *)MXS_LCDIF_BASE;
	uint32_t word_len = 0, bus_width = 0;
	uint8_t valid_data = 0;

	/* Kick in the LCDIF clock */
	mxs_set_lcdclk(mode->pixclock_khz); //PS2KHZ(mode->pixclock));

	/* Restart the LCDIF block */
	mxs_reset_block(&regs->hw_lcdif_ctrl_reg);

  struct mxs_pinctrl_regs *pin = (struct mxs_pinctrl_regs*)MXS_PINCTRL_BASE;

  // switch lcd hsync, vsync pins on
  pin->hw_pinctrl_muxsel3 ^= ((1<<16)|(1<<17)|(1<<18)|(1<<19));
  pin->hw_pinctrl_muxsel3 ^= ((1<<12)|(1<<13)|(1<<14)|(1<<15));

  // rgb pins (lcd data)
  pin->hw_pinctrl_muxsel2 ^= ((1<<0)|(1<<1)|(1<<2)|(1<<3));
  pin->hw_pinctrl_muxsel2 ^= ((1<<4)|(1<<5)|(1<<6)|(1<<7));
  pin->hw_pinctrl_muxsel2 ^= ((1<<8)|(1<<9)|(1<<10)|(1<<11));
  pin->hw_pinctrl_muxsel2 ^= ((1<<12)|(1<<13)|(1<<14)|(1<<15));
  
  
	switch (bpp) {
	case 24:
		word_len = LCDIF_CTRL_WORD_LENGTH_24BIT;
		bus_width = LCDIF_CTRL_LCD_DATABUS_WIDTH_24BIT;
		valid_data = 0x7;
		break;
	case 18:
		word_len = LCDIF_CTRL_WORD_LENGTH_24BIT;
		bus_width = LCDIF_CTRL_LCD_DATABUS_WIDTH_18BIT;
		valid_data = 0x7;
		break;
	case 16:
		word_len = LCDIF_CTRL_WORD_LENGTH_16BIT;
		bus_width = LCDIF_CTRL_LCD_DATABUS_WIDTH_16BIT;
		valid_data = 0xf;
		break;
	case 8:
		word_len = LCDIF_CTRL_WORD_LENGTH_8BIT;
		bus_width = LCDIF_CTRL_LCD_DATABUS_WIDTH_8BIT;
		valid_data = 0xf;
		break;
	}

	writel(bus_width | word_len | LCDIF_CTRL_DOTCLK_MODE |
		LCDIF_CTRL_BYPASS_COUNT | LCDIF_CTRL_LCDIF_MASTER,
		&regs->hw_lcdif_ctrl);

	writel(valid_data << LCDIF_CTRL1_BYTE_PACKING_FORMAT_OFFSET,
		&regs->hw_lcdif_ctrl1);

	//mxsfb_system_setup();

	writel((mode->yres << LCDIF_TRANSFER_COUNT_V_COUNT_OFFSET) | mode->xres,
		&regs->hw_lcdif_transfer_count);

	writel(LCDIF_VDCTRL0_ENABLE_PRESENT | LCDIF_VDCTRL0_ENABLE_POL |
		LCDIF_VDCTRL0_VSYNC_PERIOD_UNIT |
		LCDIF_VDCTRL0_VSYNC_PULSE_WIDTH_UNIT |
		mode->vsync_len, &regs->hw_lcdif_vdctrl0);
  
	writel(mode->upper_margin + mode->lower_margin +
		mode->vsync_len + mode->yres,
		&regs->hw_lcdif_vdctrl1);
  
	writel((mode->hsync_len << LCDIF_VDCTRL2_HSYNC_PULSE_WIDTH_OFFSET) |
		(mode->left_margin + mode->right_margin +
		mode->hsync_len + mode->xres),
		&regs->hw_lcdif_vdctrl2);
  
	writel(((mode->left_margin + mode->hsync_len) <<
		LCDIF_VDCTRL3_HORIZONTAL_WAIT_CNT_OFFSET) |
		(mode->upper_margin + mode->vsync_len) | LCDIF_VDCTRL3_MUX_SYNC_SIGNALS,
		&regs->hw_lcdif_vdctrl3);
  
	writel((0 << LCDIF_VDCTRL4_DOTCLK_DLY_SEL_OFFSET) | mode->xres,
		&regs->hw_lcdif_vdctrl4);

	writel(panel->frameAdrs, &regs->hw_lcdif_cur_buf);
	writel(panel->frameAdrs, &regs->hw_lcdif_next_buf);

	/* Flush FIFO first */
	writel(LCDIF_CTRL1_FIFO_CLEAR, &regs->hw_lcdif_ctrl1_set);

	/* Sync signals ON */
	setbits_le32(&regs->hw_lcdif_vdctrl4, LCDIF_VDCTRL4_SYNC_SIGNALS_ON);

	/* FIFO cleared */
	writel(LCDIF_CTRL1_FIFO_CLEAR, &regs->hw_lcdif_ctrl1_clr);

	/* RUN! */
	writel(LCDIF_CTRL_RUN, &regs->hw_lcdif_ctrl_set);
}


void mxs_set_lcdclk(uint32_t freq)
{
	struct mxs_clkctrl_regs *clkctrl_regs =
		(struct mxs_clkctrl_regs *)MXS_CLKCTRL_BASE;
	uint32_t fp, x, k_rest, k_best, x_best, tk;
	int32_t k_best_l = 999, k_best_t = 0, x_best_l = 0xff, x_best_t = 0xff;

	if (freq == 0)
		return;

#if defined(CONFIG_MX23)
	writel(CLKCTRL_CLKSEQ_BYPASS_PIX, &clkctrl_regs->hw_clkctrl_clkseq_clr);
#elif defined(CONFIG_MX28)
	writel(CLKCTRL_CLKSEQ_BYPASS_DIS_LCDIF, &clkctrl_regs->hw_clkctrl_clkseq_clr);
#endif

	/*
	 *             /               18 \     1       1
	 * freq kHz = | 480000000 Hz * --  | * --- * ------
	 *             \                x /     k     1000
	 *
	 *      480000000 Hz   18
	 *      ------------ * --
	 *        freq kHz      x
	 * k = -------------------
	 *             1000
	 */

	fp = ((PLL_FREQ_KHZ * 1000) / freq) * 18;

	for (x = 18; x <= 35; x++) {
		tk = fp / x;
		if ((tk / 1000 == 0) || (tk / 1000 > 255))
			continue;

		k_rest = tk % 1000;

		if (k_rest < (k_best_l % 1000)) {
			k_best_l = tk;
			x_best_l = x;
		}

		if (k_rest > (k_best_t % 1000)) {
			k_best_t = tk;
			x_best_t = x;
		}
	}

	if (1000 - (k_best_t % 1000) > (k_best_l % 1000)) {
		k_best = k_best_l;
		x_best = x_best_l;
	} else {
		k_best = k_best_t;
		x_best = x_best_t;
	}

	k_best /= 1000;

#if defined(CONFIG_MX23)
	writeb(CLKCTRL_FRAC_CLKGATE,
		&clkctrl_regs->hw_clkctrl_frac0_set[CLKCTRL_FRAC0_PIX]);
	writeb(CLKCTRL_FRAC_CLKGATE | (x_best & CLKCTRL_FRAC_FRAC_MASK),
		&clkctrl_regs->hw_clkctrl_frac0[CLKCTRL_FRAC0_PIX]);
	writeb(CLKCTRL_FRAC_CLKGATE,
		&clkctrl_regs->hw_clkctrl_frac0_clr[CLKCTRL_FRAC0_PIX]);

	writel(CLKCTRL_PIX_CLKGATE,
		&clkctrl_regs->hw_clkctrl_pix_set);
	clrsetbits_le32(&clkctrl_regs->hw_clkctrl_pix,
			CLKCTRL_PIX_DIV_MASK | CLKCTRL_PIX_CLKGATE,
			k_best << CLKCTRL_PIX_DIV_OFFSET);

	while (readl(&clkctrl_regs->hw_clkctrl_pix) & CLKCTRL_PIX_BUSY)
		;
#elif defined(CONFIG_MX28)
	writeb(CLKCTRL_FRAC_CLKGATE,
		&clkctrl_regs->hw_clkctrl_frac1_set[CLKCTRL_FRAC1_PIX]);
	writeb(CLKCTRL_FRAC_CLKGATE | (x_best & CLKCTRL_FRAC_FRAC_MASK),
		&clkctrl_regs->hw_clkctrl_frac1[CLKCTRL_FRAC1_PIX]);
	writeb(CLKCTRL_FRAC_CLKGATE,
		&clkctrl_regs->hw_clkctrl_frac1_clr[CLKCTRL_FRAC1_PIX]);

	writel(CLKCTRL_DIS_LCDIF_CLKGATE,
		&clkctrl_regs->hw_clkctrl_lcdif_set);
	clrsetbits_le32(&clkctrl_regs->hw_clkctrl_lcdif,
			CLKCTRL_DIS_LCDIF_DIV_MASK | CLKCTRL_DIS_LCDIF_CLKGATE,
			k_best << CLKCTRL_DIS_LCDIF_DIV_OFFSET);

	while (readl(&clkctrl_regs->hw_clkctrl_lcdif) & CLKCTRL_DIS_LCDIF_BUSY)
		;
#endif
}


int mxs_wait_mask_set(struct mxs_register_32 *reg, uint32_t mask, unsigned
								int timeout)
{
	while (--timeout) {
		if ((readl(&reg->reg) & mask) == mask)
			break;
		udelay(1);
	}

	return !timeout;
}

int mxs_wait_mask_clr(struct mxs_register_32 *reg, uint32_t mask, unsigned
								int timeout)
{
	while (--timeout) {
		if ((readl(&reg->reg) & mask) == 0)
			break;
		udelay(1);
	}

	return !timeout;
}


int mxs_reset_block(struct mxs_register_32 *reg)
{
	/* Clear SFTRST */
	writel(MXS_BLOCK_SFTRST, &reg->reg_clr);

	if (mxs_wait_mask_clr(reg, MXS_BLOCK_SFTRST, RESET_MAX_TIMEOUT))
		return 1;

	/* Clear CLKGATE */
	writel(MXS_BLOCK_CLKGATE, &reg->reg_clr);

	/* Set SFTRST */
	writel(MXS_BLOCK_SFTRST, &reg->reg_set);

	/* Wait for CLKGATE being set */
	if (mxs_wait_mask_set(reg, MXS_BLOCK_CLKGATE, RESET_MAX_TIMEOUT))
		return 1;

	/* Clear SFTRST */
	writel(MXS_BLOCK_SFTRST, &reg->reg_clr);

	if (mxs_wait_mask_clr(reg, MXS_BLOCK_SFTRST, RESET_MAX_TIMEOUT))
		return 1;

	/* Clear CLKGATE */
	writel(MXS_BLOCK_CLKGATE, &reg->reg_clr);

	if (mxs_wait_mask_clr(reg, MXS_BLOCK_CLKGATE, RESET_MAX_TIMEOUT))
		return 1;

	return 0;
}

void *memset(void *s, int c, size_t n)
{
    unsigned char* p=s;
    while(n--)
        *p++ = (unsigned char)c;
    return s;
}

#include "font_c64.h"
#define FONT FONT_C64

void blit_string(char* str, int len, int x, int y) {

  uint8_t* dst = FRAMEBUFFER+y*FB_PITCH+x;
  int i = 0, line = 0;
  
  for (i=0; i<len; i++) {
    int offset = ((int)str[i]) * 8;
    for (line=0; line<8; line++) {
      uint8_t pixels = FONT[offset+line];
      
      *dst++ = (pixels&128)>0?0xff:0x00;
      *dst++ = (pixels&64)>0?0xff:0x00;
      *dst++ = (pixels&32)>0?0xff:0x00;
      *dst++ = (pixels&16)>0?0xff:0x00;
      *dst++ = (pixels&8)>0?0xff:0x00;
      *dst++ = (pixels&4)>0?0xff:0x00;
      *dst++ = (pixels&2)>0?0xff:0x00;
      *dst   = (pixels&1)>0?0xff:0x00;
      
      dst+=FB_PITCH-7;
    }
    dst-=(FB_PITCH*8);
    dst+=8;
  }
}
