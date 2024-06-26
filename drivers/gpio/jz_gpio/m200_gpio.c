/*
 * M200 GPIO definitions
 *
 * Copyright (c) 2013 Ingenic Semiconductor Co.,Ltd
 * Author: Sonil <ztyan@ingenic.cn>
 * Based on: newxboot/modules/gpio/jz4775_gpio.c|jz4780_gpio.c
 *
 * SPDX-License-Identifier:	GPL-2.0+
 */

static struct jz_gpio_func_def uart_gpio_func[] = {
	[0] = {.port = GPIO_PORT_F, .func = GPIO_FUNC_2, .pins = 0x9},
	[1] = {.port = GPIO_PORT_D, .func = GPIO_FUNC_2, .pins = 0x9 << 26},
	[2] = {.port = GPIO_PORT_D, .func = GPIO_FUNC_1, .pins = 0xf << 4},
	[3] = {.port = GPIO_PORT_A, .func = GPIO_FUNC_1, .pins = 1 << 31},
};

static struct jz_gpio_func_def gpio_func[] = {
#if defined(CONFIG_JZ_MMC_MSC0_PA_4BIT)
	{.port = GPIO_PORT_A, .func = GPIO_FUNC_1, .pins = 0x00fc0000},
#endif
#if defined(CONFIG_JZ_SSI0_PA)
	{.port = GPIO_PORT_A, .func = GPIO_FUNC_2, .pins = 0x00fc0000},
#endif
#if defined(CONFIG_JZ_MMC_MSC0_PA_8BIT)
	{.port = GPIO_PORT_A, .func = GPIO_FUNC_1, .pins = 0x00fc00f0},
#endif
#if defined(CONFIG_JZ_MMC_MSC0_PE)
	{.port = GPIO_PORT_E, .func = GPIO_FUNC_0, .pins = 0x30f00000},
#endif
#if defined(CONFIG_JZ_MMC_MSC1_PD)
	{.port = GPIO_PORT_D, .func = GPIO_FUNC_0, .pins = 0x03f00000},
#endif
#if defined(CONFIG_JZ_MMC_MSC1_PE)
	{.port = GPIO_PORT_E, .func = GPIO_FUNC_2, .pins = 0x30f00000},
#endif
#if defined(CONFIG_JZ_MMC_MSC2_PB)
	{.port = GPIO_PORT_B, .func = GPIO_FUNC_0, .pins = 0xf0300000},
#endif
#if defined(CONFIG_JZ_MMC_MSC2_PE)
	{.port = GPIO_PORT_E, .func = GPIO_FUNC_2, .pins = 0x30f00000},
#endif

#if defined(CONFIG_NAND_LOADER)
#if (CFG_NAND_BW8 == 1)
	{.port = GPIO_PORT_A, .func = GPIO_FUNC_0, .pins = 0x000c00ff,},
	{.port = GPIO_PORT_B, .func = GPIO_FUNC_0, .pins = 0x00000003,},
	{.port = GPIO_PORT_A, .func = GPIO_FUNC_0, .pins = 0x00200000 << ((CONFIG_NAND_CS)-1),},
	{.port = GPIO_PORT_A, .func = GPIO_INPUT,  .pins = 0x00100000,},
#else
	{.port = GPIO_PORT_A, .func = GPIO_FUNC_0, .pins = 0x000c00ff,},
	{.port = GPIO_PORT_F, .func = GPIO_FUNC_1, .pins = 0x0003fc00,},
	{.port = GPIO_PORT_B, .func = GPIO_FUNC_0, .pins = 0x00000003,},
	{.port = GPIO_PORT_A, .func = GPIO_FUNC_0, .pins = 0x00200000 << ((CONFIG_NAND_CS)-1),},
	{.port = GPIO_PORT_A, .func = GPIO_INPUT,  .pins = 0x00100000,},
#endif
#endif

#if defined(CONFIG_LCD_GPIO_FUNC0_16BIT) || defined(CONFIG_LCD_GPIO_FUNC0_24BIT)
	{.port = GPIO_PORT_C, .func = GPIO_FUNC_0, .pins = 0x0fffffff,},
#endif
#ifdef  CONFIG_LCD_GPIO_FUNC2_SLCD
	{.port = GPIO_PORT_C, .func = GPIO_FUNC_2, .pins = 0x0e0ff3fc,}
#endif

#ifdef CONFIG_JZ_PWM_GPIO_E0
	{.port = GPIO_PORT_E, .func = GPIO_FUNC_0, .pins = 1 << 0,},
#endif
#ifdef CONFIG_JZ_PWM_GPIO_E1
	{.port = GPIO_PORT_E, .func = GPIO_FUNC_0, .pins = 1 << 1,},
#endif
#ifdef CONFIG_JZ_PWM_GPIO_E2
	{.port = GPIO_PORT_E, .func = GPIO_FUNC_0, .pins = 1 << 2,},
#endif
#ifdef CONFIG_JZ_PWM_GPIO_E3
	{.port = GPIO_PORT_E, .func = GPIO_FUNC_0, .pins = 1 << 3,},
#endif
#if defined(CONFIG_JZ_SSI1_PE)
	{.port = GPIO_PORT_E, .func = GPIO_FUNC_0, .pins = 0x30f00000},
#endif
};
