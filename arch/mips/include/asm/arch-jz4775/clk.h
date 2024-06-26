/*
 * JZ4775 clock definitions
 *
 * Copyright (c) 2013 Ingenic Semiconductor Co.,Ltd
 *
 * SPDX-License-Identifier:	GPL-2.0+
 */

#ifndef __CLK_H__
#define __CLK_H__

#include <asm/arch/base.h>

enum {
	APLL,
	MPLL,
	EPLL,
	VPLL,
};

enum {
	DDR = 1,
	CPU,
	BCH,
	H2CLK,
	MSC0,
	MSC1,
	MSC2,
	SSI,
};

struct cgu {
	unsigned off:8;
	unsigned sel:8;
	unsigned sel_bit:8;
	unsigned en_bit:8;
	unsigned busy_bit:8;
	unsigned div:8;
	unsigned reserved:16;
};

typedef union cpm_cpapcr {
	/** raw register data */
	uint32_t d32;
	/** register bits */
	struct {
		unsigned PLLST:8;
		unsigned PLLEN:1;
		unsigned PLLBP:1;
		unsigned ON:1;
		unsigned reserved11_14:4;
		unsigned LOCK0:1;
		unsigned PLLOD:2;
		unsigned PLLN:5;
		unsigned reserved23:1;
		unsigned PLLM:7;
		unsigned BS:1;
	} b; /* CPAPCR */
} cpm_cpapcr_t;

#define CGU_MSC_FREQ 24000000
#define CGU_MSC_DIV (CONFIG_SYS_APLL_FREQ / CGU_MSC_FREQ / 2 - 1)
#define CGU_BCH_DIV 7
#define CGU_LCD_DIV (CONFIG_SYS_APLL_FREQ / CONFIG_SYS_PCLK_FREQ - 1)

unsigned int clk_get_rate(int clk);
void clk_set_rate(int clk, unsigned long rate);
void cgu_clks_init(struct cgu *cgu_sel, int nr_cgu_clks);
void clk_init(void);
void enable_uart_clk(void);
enum otg_mode_t {
	OTG_MODE = 0,
	DEVICE_ONLY_MODE,
	HOST_ONLY_MODE,
};
void otg_phy_init(enum otg_mode_t mode,unsigned extclk);
#endif /* __CLK_H__ */
