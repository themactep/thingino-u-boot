/*
 * T5 clock definitions
 *
 * Copyright (c) 2013 Ingenic Semiconductor Co.,Ltd
 *
 * SPDX-License-Identifier:	GPL-2.0+
 */

#ifndef __CLK_H__
#define __CLK_H__

#include <asm/arch/base.h>

enum clk_id {
	DDR,
	EL200,
	MACPHY,
	LCD,
#ifdef CONFIG_JZ_PWM
	PWM,
#endif
	I2S,
	MSC,
	MSC0 = MSC,
	MSC1,
	SFC0,
	SFC1,
	CIM0,
	ISPA,
	ISPS,
	ISPM,
	CGU_CNT,
	CPU = CGU_CNT,
	H2CLK,
	APLL,
	MPLL,
	VPLL,
	EXCLK,
	USBPHY,
};

struct cgu {
	unsigned en:8;
	unsigned off:8;
	unsigned sel_bit:8;
	unsigned sel_src:8;
	unsigned char sel[4];
	unsigned ce;
	unsigned busy;
	unsigned stop;
};

struct cgu_clk_src {
	unsigned int cgu_clk;
	unsigned int src;
};
#define SRC_EOF -1

typedef union cpm_cpxpcr {
	/** raw register data */
	unsigned int d32;
	/** register bits */
	struct {
		unsigned PLLEN:1;
        unsigned RESERVE1:1;
		unsigned LOCK:1;
		unsigned PLL_ON:1;
        unsigned PLLRG:3;
		unsigned PLLOD1:4;
		unsigned PLLOD0:3;
		unsigned PLLN:6;
		unsigned PLLM:9;
        unsigned RESERVE2:3;
	} b; /* CPAPCR */
} cpm_cpxpcr_t;

unsigned int clk_get_rate(int clk);
void clk_set_rate(int clk, unsigned long rate);
void cgu_clks_init(struct cgu *cgu_sel, int nr_cgu_clks);
void clk_prepare(void);
void clk_init(void);
void enable_uart_clk(void);
enum otg_mode_t {
	OTG_MODE = 0,
	DEVICE_ONLY_MODE,
	HOST_ONLY_MODE,
};
void otg_phy_init(enum otg_mode_t mode,unsigned extclk);
#endif /* __CLK_H__ */
