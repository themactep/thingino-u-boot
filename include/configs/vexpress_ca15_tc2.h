/*
 * (C) Copyright 2013 Linaro
 * Andre Przywara, <andre.przywara@linaro.org>
 *
 * Configuration for Versatile Express. Parts were derived from other ARM
 *   configurations.
 *
 * SPDX-License-Identifier:	GPL-2.0+
 */

#ifndef __VEXPRESS_CA15X2_TC2_h
#define __VEXPRESS_CA15X2_TC2_h

#define CONFIG_VEXPRESS_EXTENDED_MEMORY_MAP
#include "vexpress_common.h"
#define CONFIG_BOOTP_VCI_STRING     "U-boot.armv7.vexpress_ca15x2_tc2"

#define CONFIG_SYS_CLK_FREQ 24000000

#endif
