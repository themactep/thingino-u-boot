/*
 * Copyright 2013 Freescale Semiconductor, Inc.
 *
 * Authors:  Roy Zang <tie-fei.zang@freescale.com>
 *	     Chunhe Lan <Chunhe.Lan@freescale.com>
 *
 * SPDX-License-Identifier:	GPL-2.0+
 */

#ifndef __CONFIG_H
#define __CONFIG_H

#ifndef CONFIG_SYS_TEXT_BASE
#define CONFIG_SYS_TEXT_BASE	0xeff80000
#endif

#ifndef CONFIG_SYS_MONITOR_BASE
#define CONFIG_SYS_MONITOR_BASE	CONFIG_SYS_TEXT_BASE	/* start of monitor */
#endif

#ifndef CONFIG_RESET_VECTOR_ADDRESS
#define CONFIG_RESET_VECTOR_ADDRESS	0xeffffffc
#endif

/* High Level Configuration Options */
#define CONFIG_BOOKE		/* BOOKE */
#define CONFIG_E500		/* BOOKE e500 family */
#define CONFIG_MPC85xx
#define CONFIG_P1023
#define CONFIG_MP		/* support multiple processors */

#define CONFIG_FSL_ELBC		/* Has Enhanced localbus controller */
#define CONFIG_PCI		/* Enable PCI/PCIE */
#define CONFIG_PCI_INDIRECT_BRIDGE     /* indirect PCI bridge support */
#define CONFIG_PCIE1		/* PCIE controler 1 (slot 1) */
#define CONFIG_PCIE2		/* PCIE controler 2 (slot 2) */
#define CONFIG_PCIE3		/* PCIE controler 3 (slot 3) */
#define CONFIG_FSL_PCI_INIT	/* Use common FSL init code */
#define CONFIG_FSL_PCIE_RESET	/* need PCIe reset errata */
#define CONFIG_SYS_PCI_64BIT	/* enable 64-bit PCI resources */
#define CONFIG_FSL_LAW		/* Use common FSL init code */

#ifndef __ASSEMBLY__
extern unsigned long get_clock_freq(void);
#endif

#define CONFIG_SYS_CLK_FREQ	66666666
#define CONFIG_DDR_CLK_FREQ	CONFIG_SYS_CLK_FREQ

/*
 * These can be toggled for performance analysis, otherwise use default.
 */
#define CONFIG_L2_CACHE			/* toggle L2 cache */
#define CONFIG_BTB			/* toggle branch predition */
#define CONFIG_HWCONFIG

#define CONFIG_ENABLE_36BIT_PHYS

#define CONFIG_SYS_MEMTEST_START	0x01000000	/* memtest works on */
#define CONFIG_SYS_MEMTEST_END		0x02000000

#define CONFIG_PANIC_HANG	/* do not reset board on panic */

/* Implement conversion of addresses in the LBC */
#define CONFIG_SYS_LBC_LBCR		0x00000000
#define CONFIG_SYS_LBC_LCRR		LCRR_CLKDIV_8

/* DDR Setup */
#define CONFIG_VERY_BIG_RAM
#define CONFIG_SYS_DDR_SDRAM_BASE	0x00000000
#define CONFIG_SYS_SDRAM_BASE		CONFIG_SYS_DDR_SDRAM_BASE

#define CONFIG_DIMM_SLOTS_PER_CTLR	1
#define CONFIG_CHIP_SELECTS_PER_CTRL	1

#define CONFIG_DDR_SPD
#define CONFIG_FSL_DDR3
#define CONFIG_FSL_DDR_INTERACTIVE
#define CONFIG_SYS_SDRAM_SIZE		512u	/* DDR is 512M */
#define CONFIG_SYS_SPD_BUS_NUM          0
#define SPD_EEPROM_ADDRESS              0x50
#define CONFIG_SYS_DDR_RAW_TIMING

/*
 * Memory map
 *
 * 0x0000_0000	0x1fff_ffff	DDR			512M cacheable
 * 0x8000_0000	0xbfff_ffff	PCI Express Mem		1G non-cacheable
 * 0xc000_0000	0xdfff_ffff	PCI			512M non-cacheable
 * 0xe100_0000	0xe3ff_ffff	PCI IO range		4M non-cacheable
 * 0xff00_0000	0xff3f_ffff	DPAA_QBMAN		4M cacheable
 * 0xff60_0000	0xff7f_ffff	CCSR			2M non-cacheable
 * 0xffd0_0000	0xffd0_3fff	L1 for stack		16K cacheable TLB0
 *
 * Localbus non-cacheable
 *
 * 0xec00_0000	0xefff_ffff	NOR flash		64M non-cacheable
 * 0xffa0_0000	0xffaf_ffff	NAND			1M non-cacheable
 */

/*
 * Local Bus Definitions
 */
#define CONFIG_SYS_FLASH_BASE		0xec000000 /* start of FLASH 64M */
#define CONFIG_SYS_FLASH_BASE_PHYS	CONFIG_SYS_FLASH_BASE

#define CONFIG_FLASH_BR_PRELIM  (BR_PHYS_ADDR(CONFIG_SYS_FLASH_BASE_PHYS) \
				| BR_PS_16 | BR_V)
#define CONFIG_FLASH_OR_PRELIM	0xfc000ff7

#define CONFIG_FLASH_CFI_DRIVER
#define CONFIG_SYS_FLASH_CFI
#define CONFIG_SYS_FLASH_EMPTY_INFO
#define CONFIG_SYS_MAX_FLASH_BANKS	1	/* number of banks */
#define CONFIG_SYS_MAX_FLASH_SECT	512	/* sectors per device */
#define CONFIG_SYS_FLASH_ERASE_TOUT	60000	/* Flash Erase Timeout (ms) */
#define CONFIG_SYS_FLASH_WRITE_TOUT	500	/* Flash Write Timeout (ms) */

#define CONFIG_BOARD_EARLY_INIT_F	/* call board_early_init_f function */
#define CONFIG_BOARD_EARLY_INIT_R	/* call board_early_init_r function */

#define CONFIG_SYS_INIT_RAM_LOCK
#define CONFIG_SYS_INIT_RAM_ADDR	0xffd00000	/* Initial L1 address */
#define CONFIG_SYS_INIT_RAM_SIZE	0x00004000/* Size of used area in RAM */
#define CONFIG_SYS_GBL_DATA_OFFSET	(CONFIG_SYS_INIT_RAM_SIZE - \
					GENERATED_GBL_DATA_SIZE)
#define CONFIG_SYS_INIT_SP_OFFSET	CONFIG_SYS_GBL_DATA_OFFSET

#define CONFIG_SYS_MONITOR_LEN	(512 * 1024)	  /* Reserve 512 kB for Mon */
#define CONFIG_SYS_MALLOC_LEN	(6 * 1024 * 1024) /* Reserved for malloc */

#define CONFIG_SYS_NAND_BASE		0xffa00000
#define CONFIG_SYS_NAND_BASE_PHYS	CONFIG_SYS_NAND_BASE

#define CONFIG_SYS_NAND_BASE_LIST	{ CONFIG_SYS_NAND_BASE }
#define CONFIG_SYS_MAX_NAND_DEVICE	1
#define CONFIG_MTD_NAND_VERIFY_WRITE
#define CONFIG_CMD_NAND
#define CONFIG_NAND_FSL_ELBC
#define CONFIG_SYS_NAND_BLOCK_SIZE	(128 * 1024)

/* NAND flash config */
#define CONFIG_SYS_NAND_BR_PRELIM  (BR_PHYS_ADDR(CONFIG_SYS_NAND_BASE_PHYS) \
				| (2<<BR_DECC_SHIFT)	/* Use HW ECC */ \
				| BR_PS_8		/* Port Size = 8bit */ \
				| BR_MS_FCM		/* MSEL = FCM */ \
				| BR_V)			/* valid */
#define CONFIG_SYS_NAND_OR_PRELIM  (OR_AM_256KB		/* length 256K */ \
				| OR_FCM_PGS \
				| OR_FCM_CSCT \
				| OR_FCM_CST \
				| OR_FCM_CHT \
				| OR_FCM_SCY_1 \
				| OR_FCM_TRLX \
				| OR_FCM_EHTR)

#define CONFIG_SYS_BR0_PRELIM	CONFIG_FLASH_BR_PRELIM	/* NOR Base Address */
#define CONFIG_SYS_OR0_PRELIM	CONFIG_FLASH_OR_PRELIM	/* NOR Options */
#define CONFIG_SYS_BR1_PRELIM	CONFIG_SYS_NAND_BR_PRELIM
#define CONFIG_SYS_OR1_PRELIM	CONFIG_SYS_NAND_OR_PRELIM /* NAND Options */

/* Serial Port */
#define CONFIG_CONS_INDEX		1
#undef	CONFIG_SERIAL_SOFTWARE_FIFO
#define CONFIG_SYS_NS16550
#define CONFIG_SYS_NS16550_SERIAL
#define CONFIG_SYS_NS16550_REG_SIZE	1
#define CONFIG_SYS_NS16550_CLK		get_bus_freq(0)

#define CONFIG_SYS_BAUDRATE_TABLE	\
	{300, 600, 1200, 2400, 4800, 9600, 19200, 38400, 57600, 115200}

#define CONFIG_SYS_NS16550_COM1	(CONFIG_SYS_CCSRBAR + 0x4500)
#define CONFIG_SYS_NS16550_COM2	(CONFIG_SYS_CCSRBAR + 0x4600)

/* Use the HUSH parser */
#define CONFIG_SYS_HUSH_PARSER

/*
 * Pass open firmware flat tree
 */
#define CONFIG_OF_LIBFDT
#define CONFIG_OF_BOARD_SETUP
#define CONFIG_OF_STDOUT_VIA_ALIAS

/* new uImage format support */
#define CONFIG_FIT
#define CONFIG_FIT_VERBOSE	/* enable fit_format_{error,warning}() */

/* I2C */
#define CONFIG_FSL_I2C		/* Use FSL common I2C driver */
#define CONFIG_HARD_I2C		/* I2C with hardware support */
#define CONFIG_I2C_MULTI_BUS
#define CONFIG_SYS_I2C_SPEED	400000	/* I2C speed and slave address */
#define CONFIG_SYS_I2C_SLAVE		0x7F
#define CONFIG_SYS_I2C_OFFSET		0x3000
#define CONFIG_SYS_I2C2_OFFSET		0x3100

/*
 * I2C2 EEPROM
 */
#define CONFIG_ID_EEPROM
#ifdef CONFIG_ID_EEPROM
#define CONFIG_SYS_I2C_EEPROM_NXID
#endif
#define CONFIG_SYS_I2C_EEPROM_ADDR		0x50
#define CONFIG_SYS_I2C_EEPROM_ADDR_LEN		1
#define CONFIG_SYS_EEPROM_BUS_NUM		0

#define CONFIG_CMD_I2C

/*
 * General PCI
 * Memory space is mapped 1-1, but I/O space must start from 0.
 */

/* controller 3, Slot 1, tgtid 3, Base address b000 */
#define CONFIG_SYS_PCIE3_NAME		"Slot 3"
#define CONFIG_SYS_PCIE3_MEM_VIRT	0x80000000
#define CONFIG_SYS_PCIE3_MEM_BUS	0x80000000
#define CONFIG_SYS_PCIE3_MEM_PHYS	0x80000000
#define CONFIG_SYS_PCIE3_MEM_SIZE	0x20000000	/* 512M */
#define CONFIG_SYS_PCIE3_IO_VIRT	0xffc00000
#define CONFIG_SYS_PCIE3_IO_BUS		0x00000000
#define CONFIG_SYS_PCIE3_IO_PHYS	0xffc00000
#define CONFIG_SYS_PCIE3_IO_SIZE	0x00010000	/* 64k */

/* controller 2, direct to uli, tgtid 2, Base address 9000 */
#define CONFIG_SYS_PCIE2_NAME		"Slot 2"
#define CONFIG_SYS_PCIE2_MEM_VIRT	0xa0000000
#define CONFIG_SYS_PCIE2_MEM_BUS	0xa0000000
#define CONFIG_SYS_PCIE2_MEM_PHYS	0xa0000000
#define CONFIG_SYS_PCIE2_MEM_SIZE	0x20000000	/* 512M */
#define CONFIG_SYS_PCIE2_IO_VIRT	0xffc10000
#define CONFIG_SYS_PCIE2_IO_BUS		0x00000000
#define CONFIG_SYS_PCIE2_IO_PHYS	0xffc10000
#define CONFIG_SYS_PCIE2_IO_SIZE	0x00010000	/* 64k */

/* controller 1, Slot 2, tgtid 1, Base address a000 */
#define CONFIG_SYS_PCIE1_NAME		"Slot 1"
#define CONFIG_SYS_PCIE1_MEM_VIRT	0xc0000000
#define CONFIG_SYS_PCIE1_MEM_BUS	0xc0000000
#define CONFIG_SYS_PCIE1_MEM_PHYS	0xc0000000
#define CONFIG_SYS_PCIE1_MEM_SIZE	0x20000000	/* 512M */
#define CONFIG_SYS_PCIE1_IO_VIRT	0xffc20000
#define CONFIG_SYS_PCIE1_IO_BUS		0x00000000
#define CONFIG_SYS_PCIE1_IO_PHYS	0xffc20000
#define CONFIG_SYS_PCIE1_IO_SIZE	0x00010000	/* 64k */

#if defined(CONFIG_PCI)
#define CONFIG_E1000		/* Defind e1000 pci Ethernet card */
#define CONFIG_PCI_PNP		/* do pci plug-and-play */
#define CONFIG_PCI_SCAN_SHOW	/* show pci devices on startup */
#endif	/* CONFIG_PCI */

/*
 * Environment
 */
#define CONFIG_ENV_OVERWRITE

#define CONFIG_ENV_IS_IN_FLASH
#if CONFIG_SYS_MONITOR_BASE > 0xfff80000
#define CONFIG_ENV_ADDR		0xfff80000
#else
#define CONFIG_ENV_ADDR		(CONFIG_SYS_MONITOR_BASE - CONFIG_ENV_SECT_SIZE)
#endif
#define CONFIG_ENV_SIZE		0x2000
#define CONFIG_ENV_SECT_SIZE	0x20000 /* 128K (one sector) */

#define CONFIG_LOADS_ECHO		/* echo on for serial download */
#define CONFIG_SYS_LOADS_BAUD_CHANGE	/* allow baudrate change */

/*
 * Command line configuration.
 */
#include <config_cmd_default.h>

#define CONFIG_CMD_IRQ
#define CONFIG_CMD_PING
#define CONFIG_CMD_MII
#define CONFIG_CMD_SETEXPR
#define CONFIG_CMD_REGINFO

#if defined(CONFIG_PCI)
#define CONFIG_CMD_PCI
#define CONFIG_CMD_NET
#endif

/*
 * USB
 */
#define CONFIG_HAS_FSL_DR_USB
#ifdef CONFIG_HAS_FSL_DR_USB
#define CONFIG_USB_EHCI

#ifdef CONFIG_USB_EHCI
#define CONFIG_CMD_USB
#define CONFIG_EHCI_HCD_INIT_AFTER_RESET
#define CONFIG_USB_EHCI_FSL
#define CONFIG_USB_STORAGE
#define CONFIG_CMD_FAT
#define CONFIG_CMD_EXT2
#define CONFIG_CMD_FAT
#define CONFIG_DOS_PARTITION
#endif
#endif

/*
 * Miscellaneous configurable options
 */
#define CONFIG_SYS_LONGHELP			/* undef to save memory	*/
#define CONFIG_CMDLINE_EDITING		/* Command-line editing */
#define CONFIG_SYS_LOAD_ADDR	0x2000000	/* default load address */
#define CONFIG_SYS_PROMPT	"=> "		/* Monitor Command Prompt */
#if defined(CONFIG_CMD_KGDB)
#define CONFIG_SYS_CBSIZE	1024		/* Console I/O Buffer Size */
#else
#define CONFIG_SYS_CBSIZE	256		/* Console I/O Buffer Size */
#endif
/* Print Buffer Size */
#define CONFIG_SYS_PBSIZE (CONFIG_SYS_CBSIZE + sizeof(CONFIG_SYS_PROMPT)+16)
#define CONFIG_SYS_MAXARGS	16		/* max number of command args */
/* Boot Argument Buffer Size */
#define CONFIG_SYS_BARGSIZE	CONFIG_SYS_CBSIZE
#define CONFIG_SYS_HZ	1000		/* decrementer freq: 1ms ticks */

/*
 * For booting Linux, the board info and command line data
 * have to be in the first 64 MB of memory, since this is
 * the maximum mapped by the Linux kernel during initialization.
 */
#define CONFIG_SYS_BOOTMAPSZ	(64 << 20)   /* Initial Memory map for Linux*/
#define CONFIG_SYS_BOOTM_LEN	(64 << 20)   /* Increase max gunzip size */

/*
 * Environment Configuration
 */
#define CONFIG_BOOTFILE		"uImage"
#define CONFIG_UBOOTPATH	(u-boot.bin) /* U-Boot image on TFTP server */

/* default location for tftp and bootm */
#define CONFIG_LOADADDR		1000000

#define CONFIG_BOOTDELAY -1	/* -1 disables auto-boot */

#define CONFIG_BAUDRATE	115200

/* Qman/Bman */
#define CONFIG_SYS_DPAA_QBMAN		/* support Q/Bman */
#define CONFIG_SYS_QMAN_MEM_BASE	0xff000000
#define CONFIG_SYS_QMAN_MEM_PHYS	CONFIG_SYS_QMAN_MEM_BASE
#define CONFIG_SYS_QMAN_MEM_SIZE	0x00200000
#define CONFIG_SYS_BMAN_MEM_BASE	0xff200000
#define CONFIG_SYS_BMAN_MEM_PHYS	CONFIG_SYS_BMAN_MEM_BASE
#define CONFIG_SYS_BMAN_MEM_SIZE	0x00200000

/* For FM */
#define CONFIG_SYS_DPAA_FMAN
#define CONFIG_PHY_GIGE		/* Include GbE speed/duplex detection */

#ifdef CONFIG_SYS_DPAA_FMAN
#define CONFIG_FMAN_ENET
#define CONFIG_PHY_ATHEROS
#endif

/* Default address of microcode for the Linux Fman driver */
/* QE microcode/firmware address */
#define CONFIG_SYS_QE_FMAN_FW_IN_NOR
#define CONFIG_SYS_QE_FMAN_FW_ADDR	0xeff40000
#define CONFIG_SYS_QE_FMAN_FW_LENGTH	0x10000
#define CONFIG_SYS_FDT_PAD		(0x3000 + CONFIG_SYS_QE_FMAN_FW_LENGTH)

#ifdef CONFIG_FMAN_ENET
#define CONFIG_SYS_FM1_DTSEC1_PHY_ADDR	0x1
#define CONFIG_SYS_FM1_DTSEC2_PHY_ADDR	0x2

#define CONFIG_SYS_TBIPA_VALUE	8
#define CONFIG_MII		/* MII PHY management */
#define CONFIG_ETHPRIME		"FM1@DTSEC1"
#endif

#define CONFIG_EXTRA_ENV_SETTINGS	\
	"hwconfig=usb1:dr_mode=host,phy_type=ulpi\0"

#endif	/* __CONFIG_H */
