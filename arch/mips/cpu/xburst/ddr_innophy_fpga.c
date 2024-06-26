/*
 * DDR driver for Synopsys DWC DDR PHY.
 * Used by Jz4775, JZ4780...
 *
 * Copyright (C) 2013 Ingenic Semiconductor Co.,Ltd
 * Author: Zoro <ykli@ingenic.cn>
 *
 * SPDX-License-Identifier:	GPL-2.0+
 */

/*#define DEBUG*/
#include <config.h>
#include <common.h>
#include <ddr/ddr_common.h>
#include <generated/ddr_reg_values.h>

#include <asm/io.h>
#include <asm/arch/clk.h>
#include <asm/ddr_dwc.h>
#define CONFIG_DWC_DEBUG 0
#include "ddr_debug.h"

#if (CONFIG_DDR_CS1 == 1)
#ifndef DDR_ROW1
#error "please define DDR_ROW1"
#endif /* DDR_ROW1 */
#ifndef DDR_COL1
#error "please define DDR_COL1"
#endif /* DDR_COL1 */
#endif /* CONFIG_DDR_CS1 */

DECLARE_GLOBAL_DATA_PTR;
extern unsigned int sdram_size(int cs, struct ddr_params *p);

struct ddr_params *ddr_params_p = NULL;
#ifndef CONFIG_FPGA
extern void reset_dll(void);
#endif

#if defined(CONFIG_SPL_DDR_SOFT_TRAINING) || defined(CONFIG_DDR_FORCE_SOFT_TRAINING)
extern bool dqs_gate_train(int rank_cnt, int byte_cnt);
extern void send_MR0(int a);
#else /* CONFIG_SPL_DDR_SOFT_TRAINING */
#define send_MR0(a)	do {} while(0);
#define dqs_gate_train(rank_cnt, byte_cnt) true
#endif /* CONFIG_SPL_DDR_SOFT_TRAINING */

#define BYPASS_ENABLE       1
#define BYPASS_DISABLE      0
#define IS_BYPASS_MODE(x)     (((x) & 1) == BYPASS_ENABLE)
	/* DDR3, */
	/* LPDDR, */
	/* LPDDR2, */
	/* DDR2,  */
	/* VARIABLE, */

#define DDR_TYPE_MODE(x)     (((x) >> 1) & 0xf)

static void dump_ddrc_register(void)
{
#ifdef CONFIG_DWC_DEBUG
	dwc_debug("DDRC_STATUS         0x%x\n", ddr_readl(DDRC_STATUS));
	dwc_debug("DDRC_CFG            0x%x\n", ddr_readl(DDRC_CFG));
	dwc_debug("DDRC_CTRL           0x%x\n", ddr_readl(DDRC_CTRL));
	dwc_debug("DDRC_LMR            0x%x\n", ddr_readl(DDRC_LMR));
	dwc_debug("DDRC_DLP            0x%x\n", ddr_readl(DDRC_DLP));
	dwc_debug("DDRC_TIMING1        0x%x\n", ddr_readl(DDRC_TIMING(1)));
	dwc_debug("DDRC_TIMING2        0x%x\n", ddr_readl(DDRC_TIMING(2)));
	dwc_debug("DDRC_TIMING3        0x%x\n", ddr_readl(DDRC_TIMING(3)));
	dwc_debug("DDRC_TIMING4        0x%x\n", ddr_readl(DDRC_TIMING(4)));
	dwc_debug("DDRC_TIMING5        0x%x\n", ddr_readl(DDRC_TIMING(5)));
	dwc_debug("DDRC_TIMING6        0x%x\n", ddr_readl(DDRC_TIMING(6)));
	dwc_debug("DDRC_REFCNT         0x%x\n", ddr_readl(DDRC_REFCNT));
	dwc_debug("DDRC_MMAP0          0x%x\n", ddr_readl(DDRC_MMAP0));
	dwc_debug("DDRC_MMAP1          0x%x\n", ddr_readl(DDRC_MMAP1));
	dwc_debug("DDRC_REMAP1         0x%x\n", ddr_readl(DDRC_REMAP(1)));
	dwc_debug("DDRC_REMAP2         0x%x\n", ddr_readl(DDRC_REMAP(2)));
	dwc_debug("DDRC_REMAP3         0x%x\n", ddr_readl(DDRC_REMAP(3)));
	dwc_debug("DDRC_REMAP4         0x%x\n", ddr_readl(DDRC_REMAP(4)));
	dwc_debug("DDRC_REMAP5         0x%x\n", ddr_readl(DDRC_REMAP(5)));
	dwc_debug("DDRC_AUTOSR_EN      0x%x\n", ddr_readl(DDRC_AUTOSR_EN));
#endif
}
static void dump_ddrp_register(void)
{
#ifdef CONFIG_DWC_DEBUG
	dwc_debug("DDRP_PIR		0x%x\n", ddr_readl(DDRP_PIR));
	dwc_debug("DDRP_PGCR		0x%x\n", ddr_readl(DDRP_PGCR));
	dwc_debug("DDRP_PGSR		0x%x\n", ddr_readl(DDRP_PGSR));
	dwc_debug("DDRP_PTR0		0x%x\n", ddr_readl(DDRP_PTR0));
	dwc_debug("DDRP_PTR1		0x%x\n", ddr_readl(DDRP_PTR1));
	dwc_debug("DDRP_PTR2		0x%x\n", ddr_readl(DDRP_PTR2));
	dwc_debug("DDRP_DCR		0x%x\n", ddr_readl(DDRP_DCR));
	dwc_debug("DDRP_DTPR0		0x%x\n", ddr_readl(DDRP_DTPR0));
	dwc_debug("DDRP_DTPR1		0x%x\n", ddr_readl(DDRP_DTPR1));
	dwc_debug("DDRP_DTPR2		0x%x\n", ddr_readl(DDRP_DTPR2));
	dwc_debug("DDRP_MR0		0x%x\n", ddr_readl(DDRP_MR0));
	dwc_debug("DDRP_MR1		0x%x\n", ddr_readl(DDRP_MR1));
	dwc_debug("DDRP_MR2		0x%x\n", ddr_readl(DDRP_MR2));
	dwc_debug("DDRP_MR3		0x%x\n", ddr_readl(DDRP_MR3));
	dwc_debug("DDRP_ODTCR		0x%x\n", ddr_readl(DDRP_ODTCR));
	dwc_debug("DDRP_DSGCR		0x%x\n", ddr_readl(DDRP_DSGCR));
	int i=0;
	for(i=0;i<4;i++) {
		dwc_debug("DX%dGSR0: %x\n", i, ddr_readl(DDRP_DXGSR0(i)));
		dwc_debug("@pas:DXDQSTR(%d)= 0x%x\n", i,ddr_readl(DDRP_DXDQSTR(i)));
	}
#endif
}
static void reset_controller(void)
{
#ifndef CONFIG_FPGA
	ddr_writel(0xf << 20, DDRC_CTRL);
#else
	ddr_writel(0xc0 << 16, DDRC_CTRL);
#endif
	mdelay(5);
	ddr_writel(0, DDRC_CTRL);
	mdelay(5);
}

static void remap_swap(int a, int b)
{
	uint32_t remmap[2], tmp[2];

	remmap[0] = ddr_readl(DDRC_REMAP(a / 4 + 1));
	remmap[1] = ddr_readl(DDRC_REMAP(b / 4 + 1));

#define BIT(bit) ((bit % 4) * 8)
#define MASK(bit) (0x1f << BIT(bit))
	tmp[0] = (remmap[0] & MASK(a)) >> BIT(a);
	tmp[1] = (remmap[1] & MASK(b)) >> BIT(b);

	remmap[0] &= ~MASK(a);
	remmap[1] &= ~MASK(b);

	ddr_writel(remmap[0] | (tmp[1] << BIT(a)), DDRC_REMAP(a / 4 + 1));
	ddr_writel(remmap[1] | (tmp[0] << BIT(b)), DDRC_REMAP(b / 4 + 1));
#undef BIT
#undef MASK
}

static void mem_remap(void)
{
	uint32_t start = 0, num = 0;
	int row, col, dw32, bank8, cs0, cs1;
	uint32_t size0 = 0, size1 = 0;

#ifdef CONFIG_DDR_HOST_CC
#if (CONFIG_DDR_CS0 == 1)
			row = DDR_ROW;
			col = DDR_COL;
			dw32 = CONFIG_DDR_DW32;
			bank8 = DDR_BANK8;
#endif

	size0 = (unsigned int)(DDR_CHIP_0_SIZE);
	size1 = (unsigned int)(DDR_CHIP_1_SIZE);

	/* For two different size ddr chips, just don't remmap */

#if (CONFIG_DDR_CS1 == 1)
	if (size0 != size1)
		return;
#endif

#if (CONFIG_DDR_CS0 == 1)
#if (CONFIG_DDR_CS1 == 1)
	if (size1 && size0) {
		if (size1 <= size0) {
			row = DDR_ROW1;
			col = DDR_COL1;
			dw32 = CONFIG_DDR_DW32;
			bank8 = DDR_BANK8;
		} else {
			row = DDR_ROW;
			col = DDR_COL;
			dw32 = CONFIG_DDR_DW32;
			bank8 = DDR_BANK8;
		}
	} else {
		dwc_debug("Error: The DDR size is 0\n");
		hang();
	}
#else /*CONFIG_DDR_CS1 == 1*/
	if (size0) {
		row = DDR_ROW;
		col = DDR_COL;
		dw32 = CONFIG_DDR_DW32;
		bank8 = DDR_BANK8;
	} else {
		dwc_debug("Error: The DDR size is 0\n");
		hang();
	}

#endif /* CONFIG_DDR_CS1 == 1 */
#else /* CONFIG_DDR_CS0 == 1 */
	if (size1) {
		row = DDR_ROW1;
		col = DDR_COL1;
		dw32 = CONFIG_DDR_DW32;
		bank8 = DDR_BANK8;
	} else {
		dwc_debug("Error: The DDR size is 0\n");
		hang();
	}

#endif /* CONFIG_DDR_CS0 == 1 */
	cs0 = CONFIG_DDR_CS0;
	cs1 = CONFIG_DDR_CS1;
#else /* CONFIG_DDR_HOST_CC */
	size0 = ddr_params_p->size.chip0;
	size1 = ddr_params_p->size.chip1;
	if (size0 && size1) {
		if (size1 <= size0) {
			row = ddr_params_p->row1;
			col = ddr_params_p->col1;
			dw32 = ddr_params_p->dw32;
			bank8 = ddr_params_p->bank8;
		} else {
			row = ddr_params_p->row;
			col = ddr_params_p->col;
			dw32 = ddr_params_p->dw32;
			bank8 = ddr_params_p->bank8;
		}
	} else if (size0) {
		row = ddr_params_p->row;
		col = ddr_params_p->col;
		dw32 = ddr_params_p->dw32;
		bank8 = ddr_params_p->bank8;
	} else if (size1) {
		row = ddr_params_p->row1;
		col = ddr_params_p->col1;
		dw32 = ddr_params_p->dw32;
		bank8 = ddr_params_p->bank8;
	} else {
		dwc_debug("Error: The DDR size is 0\n");
		hang();
	}

	cs0 = ddr_params_p->cs0;
	cs1 = ddr_params_p->cs1;
#endif /* CONFIG_DDR_HOST_CC */
	start += row + col + (dw32 ? 4 : 2) / 2;
	start -= 12;

	if (bank8)
		num += 3;
	else
		num += 2;

	if (cs0 && cs1)
		num++;

	for (; num > 0; num--)
		remap_swap(0 + num - 1, start + num - 1);
}

void ddr_controller_init(void)
{
	dwc_debug("DDR Controller init\n");
// dsqiu
//	mdelay(1);
	ddr_writel(DDRC_CTRL_CKE | DDRC_CTRL_ALH, DDRC_CTRL);
	ddr_writel(0, DDRC_CTRL);
	/* DDRC CFG init*/
	ddr_writel(DDRC_CFG_VALUE, DDRC_CFG);
	/* DDRC timing init*/
	ddr_writel(DDRC_TIMING1_VALUE, DDRC_TIMING(1));
	ddr_writel(DDRC_TIMING2_VALUE, DDRC_TIMING(2));
	ddr_writel(DDRC_TIMING3_VALUE, DDRC_TIMING(3));
	ddr_writel(DDRC_TIMING4_VALUE, DDRC_TIMING(4));
	ddr_writel(DDRC_TIMING5_VALUE, DDRC_TIMING(5));
	ddr_writel(DDRC_TIMING6_VALUE, DDRC_TIMING(6));

	/* DDRC memory map configure*/
	ddr_writel(0 /*DDRC_MMAP0_VALUE*/, DDRC_MMAP0);
	ddr_writel(DDRC_MMAP1_VALUE, DDRC_MMAP1);
	ddr_writel(DDRC_CTRL_CKE | DDRC_CTRL_ALH, DDRC_CTRL);
	ddr_writel(DDRC_REFCNT_VALUE, DDRC_REFCNT);
	ddr_writel(DDRC_CTRL_VALUE & 0xffff8fff, DDRC_CTRL);
}
static void ddr_phy_param_init(unsigned int mode)
{
	int i;
	unsigned int timeout = 10000;
	ddr_writel(DDRP_DCR_VALUE, DDRP_DCR);
	ddr_writel(DDRP_MR0_VALUE, DDRP_MR0);
	switch(DDR_TYPE_MODE(mode)){
	case LPDDR:
		break;
	//note: lpddr2 should set mr1 mr2 mr3.
	case LPDDR2:
		ddr_writel(DDRP_MR3_VALUE, DDRP_MR3);
	//note: ddr3 should set mr1 mr2 only.
	case DDR3:
		ddr_writel(DDRP_MR1_VALUE, DDRP_MR1);
		ddr_writel(DDRP_MR2_VALUE, DDRP_MR2);

		break;
	case DDR2:
		ddr_writel(DDRP_MR1_VALUE, DDRP_MR1);
		break;
	}

#ifdef CONFIG_SYS_DDR_CHIP_ODT
	ddr_writel(0, DDRP_ODTCR);
#endif

	ddr_writel(DDRP_PTR0_VALUE, DDRP_PTR0);
	ddr_writel(DDRP_PTR1_VALUE, DDRP_PTR1);
	ddr_writel(DDRP_PTR2_VALUE, DDRP_PTR2);
	ddr_writel(DDRP_DTPR0_VALUE, DDRP_DTPR0);
	ddr_writel(DDRP_DTPR1_VALUE, DDRP_DTPR1);
	ddr_writel(DDRP_DTPR2_VALUE, DDRP_DTPR2);

	for (i = 0; i < 4; i++) {
		unsigned int tmp = ddr_readl(DDRP_DXGCR(i));

		tmp &= ~(3 << 9);
#if 0
#ifdef CONFIG_DDR_PHY_ODT
#ifdef CONFIG_DDR_PHY_DQ_ODT
		tmp |= 1 << 10;
#endif /* CONFIG_DDR_PHY_DQ_ODT */
#ifdef CONFIG_DDR_PHY_DQS_ODT
		tmp |= 1 << 9;
#endif /* CONFIG_DDR_PHY_DQS_ODT */
#endif /* CONFIG_DDR_PHY_ODT */
#ifndef CONFIG_DDR_HOST_CC
		if ((i > 1) && (ddr_params_p->dw32 == 0))
			tmp &= ~DDRP_DXGCR_DXEN;
#elif (CONFIG_DDR_DW32 == 0)
		if (i > 1)
			tmp &= ~DDRP_DXGCR_DXEN;
#endif /* CONFIG_DDR_HOST_CC */
#endif
		ddr_writel(tmp, DDRP_DXGCR(i));
	}
	ddr_writel(DDRP_PGCR_VALUE, DDRP_PGCR);

	/***************************************************************
	 *  DXCCR:
	 *       DQSRES:  4...7bit  is DQSRES[].
	 *       DQSNRES: 8...11bit is DQSRES[] too.
	 *
	 *      Selects the on-die pull-down/pull-up resistor for DQS pins.
	 *      DQSRES[3]: selects pull-down (when set to 0) or pull-up (when set to 1).
	 *      DQSRES[2:0] selects the resistor value as follows:
	 *      000 = Open: On-die resistor disconnected
	 *      001 = 688 ohms
	 *      010 = 611 ohms
	 *      011 = 550 ohms
	 *      100 = 500 ohms
	 *      101 = 458 ohms
	 *      110 = 393 ohms
	 *      111 = 344 ohms
	 *****************************************************************
	 *      Note: DQS resistor must be connected for LPDDR/LPDDR2    *
	 *****************************************************************
	 *     the config will affect power and stablity
	 */
	switch(DDR_TYPE_MODE(mode)){
	case LPDDR:
		ddr_writel(0x30c00813, DDRP_ACIOCR);
		ddr_writel(0x4802, DDRP_DXCCR);
		break;
	case DDR3:
		break;
	case LPDDR2:
		ddr_writel(0x910, DDRP_DXCCR);
		break;
	case DDR2:
		break;
	}
	while (!(ddr_readl(DDRP_PGSR) == (DDRP_PGSR_IDONE
					| DDRP_PGSR_DLDONE
					| DDRP_PGSR_ZCDONE))
			&& (ddr_readl(DDRP_PGSR) != 0x1f)
			&& --timeout);
	if (timeout == 0) {
		dwc_debug("DDR PHY init timeout: PGSR=%X\n", ddr_readl(DDRP_PGSR));
		hang();
	}
}

static void ddr_chip_init(unsigned int mode)
{
	int timeout = 10000;
	unsigned int pir_val = DDRP_PIR_INIT;
	unsigned int val;
	dwc_debug("DDR chip init\n");

	// DDRP_PIR_DRAMRST for ddr3 only
#ifndef CONFIG_FPGA
	switch(DDR_TYPE_MODE(mode)){
	case DDR3:
		pir_val |= DDRP_PIR_DRAMINT | DDRP_PIR_DRAMRST | DDRP_PIR_DLLSRST;
		break;
	case LPDDR2:
		pir_val |= DDRP_PIR_DRAMINT | DDRP_PIR_DLLSRST;
		break;
	case LPDDR:
		pir_val |= DDRP_PIR_DRAMINT;
		break;
	case DDR2:
		pir_val |= DDRP_PIR_DRAMINT | DDRP_PIR_DRAMINT;
		break;
	}
#else
	pir_val |= DDRP_PIR_DRAMINT | DDRP_PIR_DRAMRST | DDRP_PIR_DLLBYP;
#endif
	if(IS_BYPASS_MODE(mode)) {
		pir_val |= DDRP_PIR_DLLBYP | (1 << 29);
		pir_val &= ~DDRP_PIR_DLLSRST;
		// DLL Disable: only bypassmode
		ddr_writel(0x1 << 31, DDRP_ACDLLCR);
		val = ddr_readl(DDRP_DSGCR);
		/*  LPDLLPD:  only for ddr bypass mode
		 * Low Power DLL Power Down: Specifies if set that the PHY should respond to the *
		 * DFI low power opportunity request and power down the DLL of the PHY if the *
		 * wakeup time request satisfies the DLL lock time */
		val &= ~(1 << 4);
		ddr_writel(val,DDRP_DSGCR);

	}
	ddr_writel(pir_val, DDRP_PIR);
	while (!(ddr_readl(DDRP_PGSR) == (DDRP_PGSR_IDONE
					  | DDRP_PGSR_DLDONE
					  | DDRP_PGSR_ZCDONE
					  | DDRP_PGSR_DIDONE))
	       && (ddr_readl(DDRP_PGSR) != 0x1f)
	       && --timeout);
	if (timeout == 0) {
		dwc_debug("DDR init timeout: PGSR=%X\n", ddr_readl(DDRP_PGSR));
		hang();
	}
}

static int ddr_training_hardware(unsigned int mode)
{
	int result = 0;
	int timeout = 500000;
	unsigned int pir_val = DDRP_PIR_INIT;
	switch(DDR_TYPE_MODE(mode)){
	case DDR3:
	case LPDDR2:
		pir_val |= DDRP_PIR_QSTRN;
		break;
	case LPDDR:
		pir_val |= DDRP_PIR_QSTRN | DDRP_PIR_DLLLOCK;
		break;
	case DDR2:
		pir_val |= DDRP_PIR_QSTRN;
		break;
	}
	if(IS_BYPASS_MODE(mode))
		pir_val |= DDRP_PIR_DLLBYP | (1 << 29);

	ddr_writel(pir_val, DDRP_PIR);
	while ((ddr_readl(DDRP_PGSR) != (DDRP_PGSR_IDONE
					 | DDRP_PGSR_DLDONE
					 | DDRP_PGSR_ZCDONE
					 | DDRP_PGSR_DIDONE
					 | DDRP_PGSR_DTDONE))
	       && !(ddr_readl(DDRP_PGSR)
		    & (DDRP_PGSR_DTDONE | DDRP_PGSR_DTERR | DDRP_PGSR_DTIERR))
	       && --timeout);

	if (timeout == 0) {
		dwc_debug("DDR training timeout\n");
		result = -1;
	} else if (ddr_readl(DDRP_PGSR)
		   & (DDRP_PGSR_DTERR | DDRP_PGSR_DTIERR)) {
		dwc_debug("DDR hardware training error\n");
		result = ddr_readl(DDRP_PGSR);
	}
	return result;
}
static int ddr_training_software(unsigned int mode)
{
	unsigned int result = 0;
	unsigned int ddr_bl, ddr_cl;
	unsigned int mr0_tmp = 1;
	unsigned int cs0;
	unsigned int cs1;
	unsigned int tmp = 0;
	dwc_debug("Now try soft training\n");
#ifdef CONFIG_DDR_HOST_CC
	cs0 = CONFIG_DDR_CS0;
	cs1 = CONFIG_DDR_CS1;
#else /* CONFIG_DDR_HOST_CC */
	cs0 = ddr_params_p->cs0;
	cs1 = ddr_params_p->cs1;
#endif /* CONFIG_DDR_HOST_CC */
	if (dqs_gate_train(cs0 + cs1, 4)) {
		dwc_debug("DDR soft train fail too!!!\n");
		dump_ddrp_register();
		result = 1;
	}
	if((result) && (!cs1)){
		dwc_debug("try again to soft train may be the problem for cs\n");
		tmp = ddr_readl(DDRC_CFG);
		tmp |= 3 << 6;
		ddr_writel(tmp, DDRC_CFG);
		if (dqs_gate_train(cs0, 4)) {
			dwc_debug("try again DDR soft train fail too please check the hardware!!!\n");
			dump_ddrp_register();
			result = -1;
		}
	}

#ifdef CONFIG_DDR_HOST_CC
	ddr_bl = DDR_BL;
	ddr_cl = DDR_CL;
#else /* CONFIG_DDR_HOST_CC */
	ddr_cl = ddr_params_p->cl;
	ddr_bl = ddr_params_p->bl;
#endif /* CONFIG_DDR_HOST_CC */

	if(DDR_TYPE_MODE(mode) == LPDDR){
		while (ddr_bl >> mr0_tmp)
			mr0_tmp++;
		ddr_writel((ddr_cl << 4) | (mr0_tmp - 1), DDRP_MR0);
		send_MR0(ddr_readl(DDRP_MR0));
	}
	return result;
}
static int lpddr_retrain_bypass(void)
{
	unsigned int result = 0;
	int timeout = 10000;
	unsigned int ddr_bl, ddr_cl;
	unsigned int mr0_tmp = 1;

#ifdef CONFIG_DDR_HOST_CC
	ddr_bl = DDR_BL;
	ddr_cl = DDR_CL;
#else /* CONFIG_DDR_HOST_CC */
	ddr_cl = ddr_params_p->cl;
	ddr_bl = ddr_params_p->bl;
#endif /* CONFIG_DDR_HOST_CC */

	while (ddr_bl >> mr0_tmp)
		mr0_tmp++;
	ddr_writel((ddr_cl << 4) | (mr0_tmp - 1), DDRP_MR0);


#ifndef CONFIG_DDR_PHY_ODT
	ddr_writel(DDRP_PIR_INIT | DDRP_PIR_DRAMINT, DDRP_PIR);
#else /* CONFIG_DDR_PHY_ODT */
	ddr_writel(DDRP_PIR_INIT | DDRP_PIR_DRAMINT | DDRP_PIR_DLLLOCK | DDRP_PIR_DLLBYP | (1 << 29),
		   DDRP_PIR);
	ddr_writel(0x1, DDRP_ACDLLCR);
#endif /* CONFIG_DDR_PHY_ODT */

	while ((ddr_readl(DDRP_PGSR) != (DDRP_PGSR_IDONE
					 | DDRP_PGSR_DLDONE
					 | DDRP_PGSR_ZCDONE
					 | DDRP_PGSR_DIDONE
					 | DDRP_PGSR_DTDONE))
	       && --timeout);
	if (timeout == 0) {
		dwc_debug("DDR PHY init timeout: PGSR=%X\n", ddr_readl(DDRP_PGSR));
		result = -1;
	}
	return result;
}

static void ddr_training(unsigned int mode)
{
	unsigned int training_state = -1;
	dwc_debug("DDR training\n");
#ifndef CONFIG_DDR_FORCE_SOFT_TRAINING
	training_state = ddr_training_hardware(mode);
#endif
	if(training_state)
	{
		int i = 0;
		for (i = 0; i < 4; i++) {
			dwc_debug("DX%dGSR0: %x\n", i, ddr_readl(DDRP_DXGSR0(i)));
		}
		dump_ddrp_register();
#ifdef CONFIG_SPL_DDR_SOFT_TRAINING
		training_state = ddr_training_software(mode);
#endif // CONFIG_SPL_DDR_SOFT_TRAINING
	}
	if(DDR_TYPE_MODE(mode) == LPDDR)
		training_state = lpddr_retrain_bypass();
	if(training_state)
		hang();
}
static void ddr_impedance_matching(void)
{
#if defined(CONFIG_DDR_PHY_IMPED_PULLUP) && defined(CONFIG_DDR_PHY_IMPED_PULLDOWN)
	/**
	 * DDR3 240ohm RZQ output impedance:
	 * 	55.1ohm		0xc
	 * 	49.2ohm		0xd
	 * 	44.5ohm		0xe
	 * 	40.6ohm		0xf
	 * 	37.4ohm		0xa
	 * 	34.7ohm		0xb
	 * 	32.4ohm		0x8
	 * 	30.4ohm		0x9
	 * 	28.6ohm		0x18
	 */
	unsigned int i;
	i = ddr_readl(DDRP_ZQXCR0(0)) & ~0x3ff;
	i |= DDRP_ZQXCR_ZDEN
		| ((CONFIG_DDR_PHY_IMPED_PULLUP & 0x1f) << DDRP_ZQXCR_PULLUP_IMPED_BIT)
		| ((CONFIG_DDR_PHY_IMPED_PULLDOWN & 0x1f) << DDRP_ZQXCR_PULLDOWN_IMPED_BIT);
	ddr_writel(i, DDRP_ZQXCR0(0));
#endif
}
void ddr_phy_init(unsigned int mode)
{
	unsigned int timeout = 10000, i;
	unsigned int mr0_tmp = 1;
	bool	soft_training = false;
	unsigned int ddr_bl, ddr_cl;

	dwc_debug("DDR PHY init\n");
#ifdef CONFIG_PALLADIUM
	ddr_writel(0xfc00, DDRP_DTAR);  // 0x80010000 - 1K
#else
	ddr_writel(0x150000, DDRP_DTAR);
#endif
	/* DDR training address set*/
	ddr_phy_param_init(mode);
	ddr_chip_init(mode);
	ddr_training(mode);
	ddr_impedance_matching();
	dwc_debug("DDR PHY init OK\n");
}

#define PHY_BASE						(0xb3011000)
#define APB_BASE						(0xb3012000)
#define AHB_BASE						(0xb34f0000)


//#define REG_INNOPHY_CHANNEL_EN			(PHY_BASE + 0x0)
#define REG_INNOPHY_PHY_RST            (PHY_BASE + 0x0)
#define REG_INNOPHY_MEM_SEL            (PHY_BASE + 0x04)
#define REG_INNOPHY_TRANING_CTRL       (PHY_BASE + 0x08)
#define REG_INNOPHY_WRITE_LEVEL_MODE1  (PHY_BASE + 0x0C)
#define REG_INNOPHY_WRITE_LEVEL_MODE2  (PHY_BASE + 0x0C)
#define REG_INNOPHY_CL                 (PHY_BASE + 0x14)
#define REG_INNOPHY_AL                 (PHY_BASE + 0x18)
#define REG_INNOPHY_CWL				   (PHY_BASE + 0x1C)
#define REG_INNOPHY_DQ_WIDTH           (PHY_BASE + 0x7C)

#define REG_INNOPHY_PLL_FB_DIV_L        (PHY_BASE + 0x80)
#define REG_INNOPHY_PLL_CTRL            (PHY_BASE + 0x84)
#define REG_INNOPHY_PLL_FBDIV           (PHY_BASE + 0x88)
#define REG_INNOPHY_WL_DONE             (PHY_BASE + 0xc0)
#define REG_INNOPHY_WL_DONE             (PHY_BASE + 0xcc)
#define REG_INNOPHY_INIT_COMP           (PHY_BASE + 0xd0)


#define REG_INNOPHY_DQS_DELAY_L			(PHY_BASE + 0x120)
#define REG_INNOPHY_DQS_DELAY_H			(PHY_BASE + 0x160)

//#define REG_INNOPHY_REG02				(PHY_BASE + 0x8)
//#define REG_INNOPHY_REG08				(PHY_BASE + 0x20)
#define REG_INNOPHY_REG46				(PHY_BASE + 0x118)
#define REG_INNOPHY_REG56				(PHY_BASE + 0x158)

#define DDR_APB_PHY_INIT				(APB_BASE + 0x8c)

#define REG_DDR_CTRL					(AHB_BASE + 0x008)
#define REG_DDR_CFG						(AHB_BASE + 0x004)
#define REG_DDR_LMR						(AHB_BASE + 0x00c)


static void dump_ddrp_register_new(void)
{
	dwc_debug("REG_INNOPHY_PLL_FB_DIV_L: %x\n", *(volatile unsigned int *)REG_INNOPHY_PLL_FB_DIV_L	);
	dwc_debug("REG_INNOPHY_PLL_CTRL    : %x\n", *(volatile unsigned int *)REG_INNOPHY_PLL_CTRL		);
	dwc_debug("REG_INNOPHY_PLL_FBDIV   : %x\n", *(volatile unsigned int *)REG_INNOPHY_PLL_FBDIV	);
    dwc_debug("REG_INNOPHY_MEM_SEL     : %x\n", *(volatile unsigned int *)REG_INNOPHY_MEM_SEL		);
    dwc_debug("REG_INNOPHY_PHY_RST     : %x\n", *(volatile unsigned int *)REG_INNOPHY_PHY_RST	    );
    dwc_debug("REG_INNOPHY_CWL         : %x\n", *(volatile unsigned int *)REG_INNOPHY_CWL			);
    dwc_debug("REG_INNOPHY_AL          : %x\n", *(volatile unsigned int *)REG_INNOPHY_AL			);
    dwc_debug("REG_INNOPHY_CL          : %x\n", *(volatile unsigned int *)REG_INNOPHY_CL           );
	dwc_debug("REG_INNOPHY_DQ_WIDTH    : %x\n", *(volatile unsigned int *)REG_INNOPHY_DQ_WIDTH     );
	dwc_debug("REG_INNOPHY_DQS_DELAY_H : %x\n", *(volatile unsigned int *)REG_INNOPHY_DQS_DELAY_H  );
	dwc_debug("REG_INNOPHY_DQS_DELAY_L : %x\n", *(volatile unsigned int *)REG_INNOPHY_DQS_DELAY_L  );
	dwc_debug("REG_INNOPHY_REG46       : %x\n", *(volatile unsigned int *)REG_INNOPHY_REG46        );
	dwc_debug("REG_INNOPHY_REG56       : %x\n", *(volatile unsigned int *)REG_INNOPHY_REG56        );
	dwc_debug("REG_INNOPHY_TRANING_CTRL: %x\n", *(volatile unsigned int *)REG_INNOPHY_TRANING_CTRL );
}

void ddr_phy_init_new()
{
	dwc_debug("ddr_phy_init_new ..!\n");

	*(volatile unsigned int *)REG_INNOPHY_TRANING_CTRL = 0x0;
	*(volatile unsigned int *)REG_INNOPHY_DQ_WIDTH     = 0x3;
	*(volatile unsigned int *)REG_INNOPHY_PLL_FB_DIV_L = 0x14;
	*(volatile unsigned int *)REG_INNOPHY_PLL_CTRL	   = 0x1a;
	*(volatile unsigned int *)REG_INNOPHY_PLL_FBDIV    = 0x5;
	*(volatile unsigned int *)REG_INNOPHY_PLL_CTRL 	   = 0x18;
#ifdef CONFIG_DDR_TYPE_DDR3
	*(volatile unsigned int *)REG_INNOPHY_MEM_SEL	   = 0x10;
#endif
#ifdef CONFIG_DDR_TYPE_DDR2
	*(volatile unsigned int *)REG_INNOPHY_MEM_SEL	   = 0x11;      //modify by randy for  DDR2
#endif
	*(volatile unsigned int *)REG_INNOPHY_PHY_RST      = 0x0d;
	*(volatile unsigned int *)REG_INNOPHY_CWL		   = 0x6;
	*(volatile unsigned int *)REG_INNOPHY_CL	       = 0x07;
    *(volatile unsigned int *)REG_INNOPHY_AL           = 0x00;

	dwc_debug("DDR phy regs set!\n");

	*(volatile unsigned int *)DDR_APB_PHY_INIT	   = 0x1;                                    //enable dfi_init_start;

	while(!(*(volatile unsigned int *)DDR_APB_PHY_INIT & (1<<2)));                           //polling ddr_plllock=1;
	dwc_debug("ddr_phy_init_new ..! 11:  %X\n", *(volatile unsigned int *)DDR_APB_PHY_INIT);
	*(volatile unsigned int *)DDR_APB_PHY_INIT	   = 0;                                      //disable dfi_init_start;

	while(!(*(volatile unsigned int *)DDR_APB_PHY_INIT & (1<<1)));                           //polling dfi_init_complete=1;
	dwc_debug("ddr_phy_init_new ..! 22:  %X\n", *(volatile unsigned int *)DDR_APB_PHY_INIT);
	while(!(*(volatile unsigned int *)REG_INNOPHY_INIT_COMP));                           //polling dfi_init_complete=1;
	dwc_debug("ddr_phy_init_new ..! 33:  %X\n", *(volatile unsigned int *)DDR_APB_PHY_INIT);
	*(volatile unsigned int *)REG_DDR_CTRL		   = 0;
	dwc_debug("ddr_phy_init_new ..! 44\n");

	*(volatile unsigned int *)REG_DDR_CFG		   = DDRC_CFG_VALUE;                         //enable CS0;

	*(volatile unsigned int *)REG_DDR_CTRL		   = 0xa;                                    //enable CKE0;
#ifdef CONFIG_DDR_TYPE_DDR3
	*(volatile unsigned int *)REG_DDR_LMR		   = 0x8211;
	dwc_debug("REG_DDR_LMR: %x\n", *(volatile unsigned int *)REG_DDR_LMR);
	*(volatile unsigned int *)REG_DDR_LMR		   = 0x0;

	*(volatile unsigned int *)REG_DDR_LMR		   = 0x311;
	dwc_debug("REG_DDR_LMR: %x\n", *(volatile unsigned int *)REG_DDR_LMR);
	*(volatile unsigned int *)REG_DDR_LMR		   = 0x0;

	*(volatile unsigned int *)REG_DDR_LMR		   = 0x6111;
	dwc_debug("REG_DDR_LMR: %x\n", *(volatile unsigned int *)REG_DDR_LMR);
	*(volatile unsigned int *)REG_DDR_LMR		   = 0x0;

	*(volatile unsigned int *)REG_DDR_LMR		   = 0x530011;
	dwc_debug("REG_DDR_LMR: %x\n", *(volatile unsigned int *)REG_DDR_LMR);
	*(volatile unsigned int *)REG_DDR_LMR		   = 0x0;

	*(volatile unsigned int *)REG_DDR_LMR		   = 0x19;									  //ZQCL;
	dwc_debug("REG_DDR_LMR: %x\n", *(volatile unsigned int *)REG_DDR_LMR);
	*(volatile unsigned int *)REG_DDR_LMR		   = 0x0;

#endif

#ifdef CONFIG_DDR_TYPE_DDR2
	*(volatile unsigned int *)REG_DDR_LMR		   = 0x400001;
	*(volatile unsigned int *)REG_DDR_LMR		   = 0x0;

	*(volatile unsigned int *)REG_DDR_LMR		   = 0x0211;
	*(volatile unsigned int *)REG_DDR_LMR		   = 0x0;

	*(volatile unsigned int *)REG_DDR_LMR		   = 0x0311;
	*(volatile unsigned int *)REG_DDR_LMR		   = 0x0;

	*(volatile unsigned int *)REG_DDR_LMR		   = 0x4111;
	*(volatile unsigned int *)REG_DDR_LMR		   = 0x0;

	*(volatile unsigned int *)REG_DDR_LMR		   = 0xB63011;
	*(volatile unsigned int *)REG_DDR_LMR		   = 0x0;

	*(volatile unsigned int *)REG_DDR_LMR		   = 0x400001;
	*(volatile unsigned int *)REG_DDR_LMR		   = 0x0;

	*(volatile unsigned int *)REG_DDR_LMR		   = 0x9;
	*(volatile unsigned int *)REG_DDR_LMR		   = 0x0;

	*(volatile unsigned int *)REG_DDR_LMR		   = 0x9;
	*(volatile unsigned int *)REG_DDR_LMR		   = 0x0;

	*(volatile unsigned int *)REG_DDR_LMR		   = 0xA63011;

	*(volatile unsigned int *)REG_DDR_LMR		   = 0x384111;
	*(volatile unsigned int *)REG_DDR_LMR		   = 0x0;

	*(volatile unsigned int *)REG_DDR_LMR		   = 0x4111;
#endif
	*(volatile unsigned int *)REG_DDR_LMR		   = 0x0;

//	phy_dqs_delay(0x20,0x20);
	phy_calibration(0x1,0x1,0x1,0x1);   //(int al8_1x,int ah8_1x,int al8_2x,int ah8_2x);
   /* while( (*(volatile unsigned int *)REG_INNOPHY_STATU1 != 0x3)||(*(volatile unsigned int *)REG_INNOPHY_STATU2 != 0x3)){*/
		/*dwc_debug("************** PHY init error ******************\n");*/
	/*}*/

	dwc_debug("DDR PHY init OK\n");
}

void phy_dqs_delay(int delay_l,int delay_h)
{
    *(volatile unsigned int *)REG_INNOPHY_DQS_DELAY_L =delay_l;
	*(volatile unsigned int *)REG_INNOPHY_DQS_DELAY_H =delay_h;
	dwc_debug("REG_INNOPHY_DQS_DELAY_L: %x\n", *(volatile unsigned int *)REG_INNOPHY_DQS_DELAY_L);
	dwc_debug("REG_INNOPHY_DQS_DELAY_H: %x\n", *(volatile unsigned int *)REG_INNOPHY_DQS_DELAY_H);
}
/*
 * Name     : phy_calibration()
 * Function : control the RX DQS window delay to the DQS
 *
 * a_low_8bit_delay		= al8_2x * clk_2x + al8_1x * clk_1x;
 * a_high_8bit_delay	= ah8_2x * clk_2x + ah8_1x * clk_1x;
 *
 * */
void phy_calibration(int al8_1x,int ah8_1x,int al8_2x,int ah8_2x)
{
    int m=*(volatile unsigned int *)REG_INNOPHY_TRANING_CTRL;
	m|=(0x1<<1);
	*(volatile unsigned int *)REG_INNOPHY_TRANING_CTRL=m;
	dwc_debug("REG_INNOPHY_REG02: %x\n", *(volatile unsigned int *)REG_INNOPHY_TRANING_CTRL);
	int x=*(volatile unsigned int *)REG_INNOPHY_REG46;
	int y=*(volatile unsigned int *)REG_INNOPHY_REG56;
	//x=(x&(0x83)+(al8_1x<<3)+(al8_2x<<4));
	//y=(y&(0x83)+(ah8_1x<<3)+(ah8_2x<<4));
	*(volatile unsigned int *)REG_INNOPHY_REG46=30;
	*(volatile unsigned int *)REG_INNOPHY_REG56=27;
	dwc_debug("REG_INNOPHY_REG2c: %x\n", *(volatile unsigned int *)REG_INNOPHY_REG46);
	dwc_debug("REG_INNOPHY_REG3c: %x\n", *(volatile unsigned int *)REG_INNOPHY_REG56);

}

/**************************************
 * DDR INNOPHY REGISTER BITS DEFINE
**************************************/

/* DDRP DQ Width Register */
#define DDRP_DQ_WIDTH_DQ_H		(1 << 1)
#define DDRP_DQ_WIDTH_DQ_L		(1 << 0)

/* DDRP Pll Ctrl Register */
#define DDRP_PLL_CTRL_PLLPDEN		    (1 << 1)

/* DDRP Training Ctrl Register */
#define DDRP_TRAINING_CTRL_WL_BP	    (1 << 3)
#define DDRP_TRAINING_CTRL_WL_START	    (1 << 2)
#define DDRP_TRAINING_CTRL_DSCSE_BP	    (1 << 1)
#define DDRP_TRAINING_CTRL_DSACE_START	(1 << 0)

/* DDRP Training Done Register */
#define DDRP_CALIB_DONE_HDQCFB 		(1 << 3)
#define DDRP_CALIB_DONE_LDQCFB 		(1 << 2)
#define DDRP_CALIB_DONE_HDQCFA 		(1 << 1)
#define DDRP_CALIB_DONE_LDQCFA 		(1 << 0)
#define DDRP_CALIB_DONE_HWRLFB 		(1 << 3)
#define DDRP_CALIB_DONE_LWRLFB 		(1 << 2)
#define DDRP_CALIB_DONE_HWRLFA 		(1 << 1)
#define DDRP_CALIB_DONE_LWRLFA 		(1 << 0)

/* DDRP CALIB BP Register */
#define DDRP_CALIB_BP_CYCLESELBH_BIT	4
#define DDRP_CALIB_BP_OPHCSELBH_BIT	    3
#define DDRP_CALIB_BP_DLLSELBH_BIT	    0

/* DDRP Init Complete Register */
#define DDRP_INIT_COMP			(1 << 0)

/* DDRP PLL LOCK Register */
#define DDRP_PLL_LOCK			(1 << 3)


/*************************************************************************
 * DDRP (DDR Innophy registers)
 *************************************************************************/

#define DDRP_INNOPHY_INNO_PHY_RST	(DDR_PHY_OFFSET + 0x000)
#define DDRP_INNOPHY_MEM_CFG		(DDR_PHY_OFFSET + 0x004)
#define DDRP_INNOPHY_TRAINING_CTRL	(DDR_PHY_OFFSET + 0x008)
#define DDRP_INNOPHY_INNO_WR_LEVEL1	(DDR_PHY_OFFSET + 0x00c)
#define DDRP_INNOPHY_INNO_WR_LEVEL2	(DDR_PHY_OFFSET + 0x010)
#define DDRP_INNOPHY_CL				(DDR_PHY_OFFSET + 0x014)
#define DDRP_INNOPHY_AL				(DDR_PHY_OFFSET + 0x018)
#define DDRP_INNOPHY_CWL			(DDR_PHY_OFFSET + 0x01c)
#define DDRP_INNOPHY_DQ_WIDTH		(DDR_PHY_OFFSET + 0x07c)
#define DDRP_INNOPHY_PLL_FBDIV		(DDR_PHY_OFFSET + 0x080)
#define DDRP_INNOPHY_PLL_CTRL		(DDR_PHY_OFFSET + 0x084)
#define DDRP_INNOPHY_PLL_PDIV		(DDR_PHY_OFFSET + 0x088)
#define DDRP_INNOPHY_WL_DONE		(DDR_PHY_OFFSET + 0x100)
#define DDRP_INNOPHY_DLL_LOCK		(DDR_PHY_OFFSET + 0x104)
#define DDRP_INNOPHY_PLL_LOCK		(DDR_PHY_OFFSET + 0x108)
#define DDRP_INNOPHY_CALIB_DONE		(DDR_PHY_OFFSET + 0x10c)
#define DDRP_INNOPHY_INIT_COMP		(DDR_PHY_OFFSET + 0x110)
#define DDRP_INNOPHY_BIST_RES		(DDR_PHY_OFFSET + 0x118)

#define DDRP_INNOPHY_CALIB_DELAY_AL_RESULT	(DDR_PHY_OFFSET + 0x1d0)//0x74
#define DDRP_INNOPHY_CALIB_DELAY_AH_RESULT	(DDR_PHY_OFFSET + 0x1d4)//0x75
#define DDRP_INNOPHY_CALIB_DELAY_BL_RESULT	(DDR_PHY_OFFSET + 0x290)//0xA4
#define DDRP_INNOPHY_CALIB_DELAY_BH_RESULT	(DDR_PHY_OFFSET + 0x294)//0xA5

#define DDRP_INNOPHY_CALIB_DELAY_AL	(DDR_PHY_OFFSET + 0x158)//0x56
#define DDRP_INNOPHY_CALIB_DELAY_AH	(DDR_PHY_OFFSET + 0x198)//0x66
#define DDRP_INNOPHY_CALIB_BYPASS_AL	(DDR_PHY_OFFSET + 0x17c)//0x5f
#define DDRP_INNOPHY_CALIB_BYPASS_AH	(DDR_PHY_OFFSET + 0x23c)//0x8f
#define DDRP_INNOPHY_CALIB_DELAY_BL	(DDR_PHY_OFFSET + 0x218)//0x86
#define DDRP_INNOPHY_CALIB_DELAY_BH	(DDR_PHY_OFFSET + 0x258)//0x96
#define DDRP_INNOPHY_WL_MODE1		(DDR_PHY_OFFSET + 0x00c)
#define DDRP_INNOPHY_WL_MODE2		(DDR_PHY_OFFSET + 0x010)
#define DDRP_INNOPHY_WL_DONE		(DDR_PHY_OFFSET + 0x100)

#define DDRP_INNOPHY_TRAINING_2c	(DDR_PHY_OFFSET + 0x0b0)
#define DDRP_INNOPHY_TRAINING_3c	(DDR_PHY_OFFSET + 0x0f0)
#define DDRP_INNOPHY_TRAINING_4c	(DDR_PHY_OFFSET + 0x130)
#define DDRP_INNOPHY_TRAINING_5c	(DDR_PHY_OFFSET + 0x170)

#define DDRP_INNOPHY_WRLEVEL_RESULT1	(DDR_PHY_OFFSET + 0x1c0)
#define DDRP_INNOPHY_WRLEVEL_RESULT2	(DDR_PHY_OFFSET + 0x170)
union ddrp_calib {
	/** raw register data */
	uint8_t d8;
	/** register bits */
	struct {
		unsigned dllsel:3;
		unsigned ophsel:1;
		unsigned cyclesel:3;
		unsigned reserved7:1;
	} calib;					/* calib delay/bypass al/ah */
};

/*
 * Name     : soft_phy_calibration()
 * Function : control the RX DQS window delay to the DQS
 *
 * a_low_8bit_delay		= al8_2x * clk_2x + al8_1x * clk_1x;
 * a_high_8bit_delay	= ah8_2x * clk_2x + ah8_1x * clk_1x;
 *
 * */
void soft_phy_calibration(void)
{
  printf("!!! soft_phy_calibration\n");
	ddr_writel(DDRP_TRAINING_CTRL_DSCSE_BP, DDRP_INNOPHY_TRAINING_CTRL);

	int x, y;
	int w, z;
	int c, o, d, e;
	unsigned int addr = 0xa0010000, val;
	unsigned int delay_al = 0, delay_ah = 0, delay_bl = 0, delay_bh = 0, soft_bypass_al = 0, soft_bypass_ah = 0;
	unsigned int i, n, m = 0;
	union ddrp_calib calib_val[8 * 2 * 8];

#if 1
    for(c = 0; c < 8; c++)
        for(o = 0; o < 2; o++) {
            x = c << 4 | o << 3 ;

            for(d = 0; d < 8; d++) {
                y = d << 0;
                ddr_writel(x, DDRP_INNOPHY_CALIB_DELAY_AL);
                ddr_writel(ddr_readl(DDRP_INNOPHY_CALIB_BYPASS_AL) | y, DDRP_INNOPHY_CALIB_BYPASS_AL);
                soft_bypass_al = ddr_readl(DDRP_INNOPHY_CALIB_BYPASS_AL);
                delay_al = ddr_readl(DDRP_INNOPHY_CALIB_DELAY_AL);
                debug("### soft_bypass_al = 0x%x, delay_al = 0x%x\n", soft_bypass_al, delay_al);
                mdelay(10);
                for(i = 0; i < 0xff; i ++) {
                    val = 0;
                    for(n = 0; n < 4; n++ ) {
                        val |= i <<(n * 8);
                        val &= 0x000000ff;
                    }
                    *(volatile unsigned int *)(addr + i * 4) = val;
                    val &= 0x000000ff;
                    if((*(volatile unsigned int *)(addr + i * 4) & 0x000000ff)!= val) {
                        debug(" AL fail VALUE 0x%x  sVALUE 0x%x error \n", val ,(*(volatile unsigned int *)(addr + i * 4) >> 16));
                        break;
                    }
                }
                if(i == 0xff) {
                    calib_val[m].calib.cyclesel = c;
                    calib_val[m].calib.ophsel = o;
                    /*calib_val[m].calib.dllsel = d;*/
                    m++;
                    debug("*************** AL ok VALUE 0x%x ****************\n", x);
                }
            }
        }

    if(!m) {
        debug("AL calib bypass fail\n");
        printf("AL calib bypass fail\n");
        return ;
    }
	/* for(i = 0; i <= m; i++) */
	/* 	debug("byass CALIB_AL: dllsel %x, ophsel %x, cyclesel %x\n", calib_val[i].calib.dllsel, calib_val[i].calib.ophsel, calib_val[i].calib.cyclesel); */
	m /= 2;
	c = calib_val[m].calib.cyclesel;
	o = calib_val[m].calib.ophsel;
	/*d = calib_val[m].calib.dllsel;*/

	/*x = c << 4 | o << 3 | d;*/
    x = 0;
	x = c << 4 | o << 3 | 0;
	ddr_writel(ddr_readl(DDRP_INNOPHY_CALIB_BYPASS_AL) | y, DDRP_INNOPHY_CALIB_BYPASS_AL);
	ddr_writel(x, DDRP_INNOPHY_CALIB_DELAY_AL);
	{
		union ddrp_calib al, ah;
		al.d8 = ddr_readl(DDRP_INNOPHY_CALIB_DELAY_AL);
		debug("bypass :CALIB_AL: dllsel %x, ophsel %x, cyclesel %x\n", al.calib.dllsel, al.calib.ophsel, al.calib.cyclesel);
	}
	debug("DDRP_INNOPHY_CALIB_DELAY_AL 0x%x\n", ddr_readl(DDRP_INNOPHY_CALIB_DELAY_AL));
	printf("DDRP_INNOPHY_CALIB_DELAY_AL 0x%x\n", ddr_readl(DDRP_INNOPHY_CALIB_DELAY_AL));
#endif

#if 1
	m = 0 ;
    x = 0 ;
    y = 0 ;
	for(c = 0; c < 8; c ++)
		for(o = 0; o < 2; o++) {
			x = c << 4 | o << 3 | 0;
			for(d = 0; d < 8; d++) {
		      y = e << 4;
	        ddr_writel(x, DDRP_INNOPHY_CALIB_DELAY_AH);
	            ddr_writel(ddr_readl(DDRP_INNOPHY_CALIB_BYPASS_AL) | y, DDRP_INNOPHY_CALIB_BYPASS_AL);
                /*soft_bypass_al = ddr_readl(DDRP_INNOPHY_CALIB_BYPASS_AL);
                delay_ah = ddr_readl(DDRP_INNOPHY_CALIB_DELAY_AH);
			    debug("### soft_bypass_al = 0x%x, delay_al = 0x%x\n", soft_bypass_al, delay_ah);*/
			mdelay(10);
			for(i = 0; i < 0xff; i ++) {
				val = 0;
				for(n = 0; n < 4; n++ ) {
					val |= i <<(n * 8);
					val &= 0x0000ff00;
				}
				*(volatile unsigned int *)(addr + i * 4) = val;
				val = val >> 8;
				if(((*(volatile unsigned int *)(addr + i * 4) & 0x0000ff00) >> 8)!= val) {
					/*debug(" AH fail VALUE 0x%x  sVALUE 0x%x error \n", val ,((*(volatile unsigned int *)(addr + i * 4) & 0x0000ff00) >> 8));*/
					break;
				}
			}
			if(i == 0xff) {
				calib_val[m].calib.cyclesel = c;
				calib_val[m].calib.ophsel = o;
					/*calib_val[m].calib.dllsel = d;*/
					m++;
					debug("*************** AH  ok VALUE 0x%x ****************\n", y);
				}
            }
		}

	if(!m) {
		debug(" AH  calib bypass fail\n");
		printf(" AH  calib bypass fail\n");
		return ;
	}
	/* for(i = 0; i <= m; i++) */
	/* 	debug("byass CALIB_AL: dllsel %x, ophsel %x, cyclesel %x\n", calib_val[i].calib.dllsel, calib_val[i].calib.ophsel, calib_val[i].calib.cyclesel); */
	m /= 2;
	c = calib_val[m].calib.cyclesel;
	o = calib_val[m].calib.ophsel;
	/*d = calib_val[m].calib.dllsel;*/

	/*y = c << 4 | o << 3 | d;*/
    x = 0;
	x = c << 4 | o << 3 | 0;
	ddr_writel(ddr_readl(DDRP_INNOPHY_CALIB_BYPASS_AL) | y, DDRP_INNOPHY_CALIB_BYPASS_AL);
	ddr_writel(x, DDRP_INNOPHY_CALIB_DELAY_AH);
	{
		union ddrp_calib al, ah;
		ah.d8 = ddr_readl(DDRP_INNOPHY_CALIB_DELAY_AH);
		debug("bypass:CAHIB_AH: dllsel %x, ophsel %x, cyclesel %x\n", ah.calib.dllsel, ah.calib.ophsel, ah.calib.cyclesel);
	}
	debug("DDRP_INNOPHY_CALIB_DELAY_AH 0x%x\n", ddr_readl(DDRP_INNOPHY_CALIB_DELAY_AH));
	printf("DDRP_INNOPHY_CALIB_DELAY_AH 0x%x\n", ddr_readl(DDRP_INNOPHY_CALIB_DELAY_AH));
#endif

#if 1
	m = 0;
    x = 0;
    y = 0;
	for(c = 0; c < 8; c ++)
		for(o = 0; o < 2; o++) {
			x = c << 4 | o << 3 | 0;
			for(d = 0; d < 8; d++) {
				y = d << 0;
				ddr_writel(x, DDRP_INNOPHY_CALIB_DELAY_BL);
	            ddr_writel(ddr_readl(DDRP_INNOPHY_CALIB_BYPASS_AH) | y, DDRP_INNOPHY_CALIB_BYPASS_AH);
                /*soft_bypass_ah = ddr_readl(DDRP_INNOPHY_CALIB_BYPASS_AH);
                delay_bl = ddr_readl(DDRP_INNOPHY_CALIB_DELAY_BL);
			    debug("### soft_bypass_ah = 0x%x , delay_bl = 0x%x\n", soft_bypass_ah, delay_bl);*/
				mdelay(10);
				for(i = 0; i < 0xff; i ++) {
					val = 0;
					for(n = 0; n < 4; n++ ) {
						val |= i <<(n * 8);
						val &=  0x00ff0000;
					}
					*(volatile unsigned int *)(addr + i * 4) = val;
					val = val >> 16;
					if(((*(volatile unsigned int *)(addr + i * 4) & 0x00ff0000 )>> 16) != val) {
						/*debug(" BL fail VALUE 0x%x  sVALUE 0x%x error \n", val ,(*(volatile unsigned int *)(addr + i * 4) >> 16));*/
						break;
					}
				}
				if(i == 0xff) {
					calib_val[m].calib.cyclesel = c;
					calib_val[m].calib.ophsel = o;
					/*calib_val[m].calib.dllsel = d;*/
					m++;
					debug("*************** BL ok VALUE 0x%x ****************\n", x);
				}
			}
        }
	if(!m) {
		debug("BL calib bypass fail\n");
		return ;
	}
	/* for(i = 0; i <= m; i++) */
	/* 	debug("byass CALIB_AL: dllsel %x, ophsel %x, cyclesel %x\n", calib_val[i].calib.dllsel, calib_val[i].calib.ophsel, calib_val[i].calib.cyclesel); */
	m /= 2;
	c = calib_val[m].calib.cyclesel;
	o = calib_val[m].calib.ophsel;
	/*d = calib_val[m].calib.dllsel;*/

    x = 0;
	/*x = c << 4 | o << 3 | d;*/
	x = c << 4 | o << 3 | 0;
	ddr_writel(ddr_readl(DDRP_INNOPHY_CALIB_BYPASS_AH) | y, DDRP_INNOPHY_CALIB_BYPASS_AH);
	ddr_writel(x, DDRP_INNOPHY_CALIB_DELAY_BL);
	{
		union ddrp_calib al, ah;
		al.d8 = ddr_readl(DDRP_INNOPHY_CALIB_DELAY_BL);
		debug("bypass :CALIB_BL: dllsel %x, ophsel %x, cyclesel %x\n", al.calib.dllsel, al.calib.ophsel, al.calib.cyclesel);
	}
	debug("DDRP_INNOPHY_CALIB_DELAY_BL 0x%x\n", ddr_readl(DDRP_INNOPHY_CALIB_DELAY_BL));
#endif

#if 1
	m = 0;
    x = 0;
    y = 0;
    for(c = 0; c < 8; c ++)
		for(o = 0; o < 2; o++)  {
			x = c << 4 | o << 3 | 0;
			for(d = 0; d < 8; d++) {
				y = d << 4;
				ddr_writel(x, DDRP_INNOPHY_CALIB_DELAY_BH);
	            ddr_writel(ddr_readl(DDRP_INNOPHY_CALIB_BYPASS_AH) | y, DDRP_INNOPHY_CALIB_BYPASS_AH);
                /*soft_bypass_ah = ddr_readl(DDRP_INNOPHY_CALIB_BYPASS_AH);
                delay_bh = ddr_readl(DDRP_INNOPHY_CALIB_DELAY_BH);
			    debug("### soft_bypass_ah = 0x%x , delay_bh = 0x%x\n", soft_bypass_ah, delay_bh);*/
				mdelay(10);
				for(i = 0; i < 0xff; i ++) {
					val = 0;
					for(n = 0; n < 4; n++ ) {
						val |= i <<(n * 8);
						val &=  0xff000000;
					}
					*(volatile unsigned int *)(addr + i * 4) = val;
					val = val >> 24;
					if(((*(volatile unsigned int *)(addr + i * 4) &=  0xff000000 ) >> 24 )!= val) {
						/*debug(" BH fail VALUE 0x%x  sVALUE 0x%x error \n", val ,((*(volatile unsigned int *)(addr + i * 4) &=  0xff000000 ) >> 24 ));*/
						break;
					}
				}
				if(i == 0xff) {
					calib_val[m].calib.cyclesel = c;
					calib_val[m].calib.ophsel = o;
					/*calib_val[m].calib.dllsel = d;*/
					m++;
					debug("*************** BH  ok VALUE 0x%x ****************\n", y);
				}
			}
        }
	if(!m) {
		debug(" BH calib bypass fail\n");
		return ;
	}
	/* for(i = 0; i <= m; i++) */
	/* 	debug("byass CALIB_AL: dllsel %x, ophsel %x, cyclesel %x\n", calib_val[i].calib.dllsel, calib_val[i].calib.ophsel, calib_val[i].calib.cyclesel); */
	m /= 2;
	c = calib_val[m].calib.cyclesel;
	o = calib_val[m].calib.ophsel;
	/*d = calib_val[m].calib.dllsel;*/

    x = 0;
	x = c << 4 | o << 3 | 0;
	/*y = c << 4 | o << 3 | d;*/
	ddr_writel(ddr_readl(DDRP_INNOPHY_CALIB_BYPASS_AH) | y, DDRP_INNOPHY_CALIB_BYPASS_AH);
	ddr_writel(x, DDRP_INNOPHY_CALIB_DELAY_BH);
	{
		union ddrp_calib al, ah;
		ah.d8 = ddr_readl(DDRP_INNOPHY_CALIB_DELAY_BH);
		debug("bypass:CAHIB_BH: dllsel %x, ophsel %x, cyclesel %x\n", ah.calib.dllsel, ah.calib.ophsel, ah.calib.cyclesel);
	}
	debug("DDRP_INNOPHY_CALIB_DELAY_BH 0x%x\n", ddr_readl(DDRP_INNOPHY_CALIB_DELAY_BH));
#endif

  printf("!!! soft_phy_calibration end\n");
}

/* DDR sdram init */
void sdram_init(void)
{

	int type = VARIABLE;
	unsigned int mode;
	unsigned int bypass = 0;
	unsigned int rate;
#ifndef CONFIG_DDR4_PHY_INIT
#ifdef CONFIG_DDR_TYPE_DDR3
	type = DDR3;
#endif
#ifdef CONFIG_DDR_TYPE_LPDDR
	type = LPDDR;
#endif
#ifdef CONFIG_DDR_TYPE_LPDDR2
	type = LPDDR2;
#endif

#ifdef CONFIG_DDR_TYPE_DDR2
	type = DDR2;
#endif

#ifndef CONFIG_DDR_HOST_CC
	struct ddrc_reg ddrc;
	struct ddrp_reg ddrp;
#ifndef CONFIG_DDR_TYPE_VARIABLE
	struct ddr_params ddr_params;
	ddr_params_p = &ddr_params;
#else
	ddr_params_p = &gd->arch.gi->ddr_params;
	ddr_params_p->freq = gd->arch.gi->cpufreq / gd->arch.gi->ddr_div;
#endif
	fill_in_params(ddr_params_p, type);
	ddr_params_creator(&ddrc, &ddrp, ddr_params_p);
	ddr_params_assign(&ddrc, &ddrp, ddr_params_p);
#endif /* CONFIG_DDR_HOST_CC */

	dwc_debug("sdram init start\n");
#ifndef CONFIG_FPGA
	clk_set_rate(DDR, gd->arch.gi->ddrfreq);
	reset_dll();
	rate = clk_get_rate(DDR);
#else
	rate = gd->arch.gi->ddrfreq;
#endif
#ifdef CONFIG_M200
	if(rate <= 150000000)
		bypass = 1;
#endif
	reset_controller();

#ifdef CONFIG_DDR_AUTO_SELF_REFRESH
	ddr_writel(0x0 ,DDRC_AUTOSR_EN);
#endif
	ddr_phy_init_new();
	dump_ddrp_register_new();
#if 0
	/*force CKE1 HIGH*/
	ddr_writel(DDRC_CFG_VALUE, DDRC_CFG);
	ddr_writel((1 << 1), DDRC_CTRL);
	mode = (type << 1) | (bypass & 1);
	/* DDR PHY init*/
	ddr_writel(0, DDRC_CTRL);
#endif
	/* DDR Controller init*/
	ddr_controller_init();
	soft_phy_calibration();
	/* DDRC address remap configure*/
	//mem_remap();

	ddr_writel(ddr_readl(DDRC_STATUS) & ~DDRC_DSTATUS_MISS, DDRC_STATUS);
#ifdef CONFIG_DDR_AUTO_SELF_REFRESH
	if(!bypass)
		ddr_writel(0 , DDRC_DLP);
#ifdef CONFIG_FPGA
	//ddr_writel(ddr_readl(DDRP_DSGCR)&~(1<<4),DDRP_DSGCR);
#endif
	ddr_writel(0x1 ,DDRC_AUTOSR_EN);
#endif
	ddr_writel(0 , DDRC_DLP);

	dump_ddrc_register();
	dwc_debug("sdram init finished\n");
#undef DDRTYPE
#endif
#ifdef CONFIG_DDR4_PHY_INIT
	mdelay(1000*3);
	printf("*************DDR4 OK!!!!!!!");
#endif
}

phys_size_t initdram(int board_type)
{
#ifdef CONFIG_DDR_HOST_CC
	/* SDRAM size was calculated when compiling. */
#ifndef EMC_LOW_SDRAM_SPACE_SIZE
#define EMC_LOW_SDRAM_SPACE_SIZE 0x10000000 /* 256M */
#endif /* EMC_LOW_SDRAM_SPACE_SIZE */
	unsigned int ram_size;
	ram_size = (unsigned int)(DDR_CHIP_0_SIZE) + (unsigned int)(DDR_CHIP_1_SIZE);

	if (ram_size > EMC_LOW_SDRAM_SPACE_SIZE)
		ram_size = EMC_LOW_SDRAM_SPACE_SIZE;

	return ram_size;
#elif defined (CONFIG_BURNER)
	/* SDRAM size was defined in global info. */
	ddr_params_p = &gd->arch.gi->ddr_params;
	return ddr_params_p->size.chip0 + ddr_params_p->size.chip1;
#else
	ddr_params_p->dw32 = CONFIG_DDR_DW32;
	ddr_params_p->bank8 = DDR_BANK8;
	ddr_params_p->cs0 = CONFIG_DDR_CS0;
	ddr_params_p->cs1 = CONFIG_DDR_CS1;
	ddr_params_p->row = DDR_ROW;
	ddr_params_p->col = DDR_COL;
#ifdef DDR_ROW1
	ddr_params_p->row1 = DDR_ROW1;
#endif
#ifdef DDR_COL1
	ddr_params_p->col1 = DDR_COL1;
#endif
	return sdram_size(0, ddr_params_p) + sdram_size(1, ddr_params_p);
#endif
}
