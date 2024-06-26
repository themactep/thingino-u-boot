/*
 * Copyright (C) 2011 Andes Technology Corporation
 * Shawn Lin, Andes Technology Corporation <nobuhiro@andestech.com>
 * Macpaul Lin, Andes Technology Corporation <macpaul@andestech.com>
 *
 * SPDX-License-Identifier:	GPL-2.0+
 */

#include <common.h>
#include <netdev.h>
#include <asm/io.h>

#include <faraday/ftsdc010.h>
#include <faraday/ftsmc020.h>

DECLARE_GLOBAL_DATA_PTR;

/*
 * Miscellaneous platform dependent initializations
 */

int board_init(void)
{
	/*
	 * refer to BOOT_PARAMETER_PA_BASE within
	 * "linux/arch/nds32/include/asm/misc_spec.h"
	 */
	gd->bd->bi_arch_number = MACH_TYPE_ADPAG101P;
	gd->bd->bi_boot_params = PHYS_SDRAM_0 + 0x400;

	ftsmc020_init();	/* initialize Flash */
	return 0;
}

int dram_init(void)
{
	unsigned long sdram_base = PHYS_SDRAM_0;
	unsigned long expected_size = PHYS_SDRAM_0_SIZE;
	unsigned long actual_size;

	actual_size = get_ram_size((void *)sdram_base, expected_size);

	gd->ram_size = actual_size;

	if (expected_size != actual_size) {
		printf("Warning: Only %lu of %lu MiB SDRAM is working\n",
				actual_size >> 20, expected_size >> 20);
	}

	return 0;
}

int board_eth_init(bd_t *bd)
{
	return ftmac100_initialize(bd);
}

ulong board_flash_get_legacy(ulong base, int banknum, flash_info_t *info)
{
	if (banknum == 0) {	/* non-CFI boot flash */
		info->portwidth = FLASH_CFI_8BIT;
		info->chipwidth = FLASH_CFI_BY8;
		info->interface = FLASH_CFI_X8;
		return 1;
	} else {
		return 0;
	}
}

int board_mmc_init(bd_t *bis)
{
	ftsdc010_mmc_init(0);
	return 0;
}
