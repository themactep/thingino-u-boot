/*
 * Copyright 2013 Freescale Semiconductor, Inc.
 *
 * SPDX-License-Identifier:	GPL-2.0+
 */

/*
 * BSC9132 QDS board configuration file
 */

#ifndef __CONFIG_H
#define __CONFIG_H

#ifdef CONFIG_BSC9132QDS
#define CONFIG_BSC9132
#endif

#define CONFIG_MISC_INIT_R

#ifdef CONFIG_SDCARD
#define CONFIG_RAMBOOT_SDCARD
#define CONFIG_SYS_RAMBOOT
#define CONFIG_SYS_EXTRA_ENV_RELOC
#define CONFIG_SYS_TEXT_BASE		0x11000000
#define CONFIG_RESET_VECTOR_ADDRESS	0x1107fffc
#endif
#define CONFIG_SYS_FSL_ERRATUM_IFC_A002769	1
#ifdef CONFIG_SPIFLASH
#define CONFIG_RAMBOOT_SPIFLASH
#define CONFIG_SYS_RAMBOOT
#define CONFIG_SYS_EXTRA_ENV_RELOC
#define CONFIG_SYS_TEXT_BASE		0x11000000
#define CONFIG_RESET_VECTOR_ADDRESS	0x1107fffc
#endif

#ifdef CONFIG_NAND
#define CONFIG_SPL
#define CONFIG_SPL_INIT_MINIMAL
#define CONFIG_SPL_SERIAL_SUPPORT
#define CONFIG_SPL_NAND_SUPPORT
#define CONFIG_SPL_NAND_MINIMAL
#define CONFIG_SPL_FLUSH_IMAGE
#define CONFIG_SPL_TARGET		"u-boot-with-spl.bin"

#define CONFIG_SYS_TEXT_BASE		0x00201000
#define CONFIG_SPL_TEXT_BASE		0xFFFFE000
#define CONFIG_SPL_MAX_SIZE		8192
#define CONFIG_SPL_RELOC_TEXT_BASE	0x00100000
#define CONFIG_SPL_RELOC_STACK		0x00100000
#define CONFIG_SYS_NAND_U_BOOT_SIZE	((512 << 10) - 0x2000)
#define CONFIG_SYS_NAND_U_BOOT_DST	(0x00200000 - CONFIG_SPL_MAX_SIZE)
#define CONFIG_SYS_NAND_U_BOOT_START	0x00200000
#define CONFIG_SYS_NAND_U_BOOT_OFFS	0
#define CONFIG_SYS_LDSCRIPT	"arch/powerpc/cpu/mpc85xx/u-boot-nand.lds"
#endif

#ifndef CONFIG_SYS_TEXT_BASE
#define CONFIG_SYS_TEXT_BASE		0x8ff80000
#endif

#ifndef CONFIG_RESET_VECTOR_ADDRESS
#define CONFIG_RESET_VECTOR_ADDRESS	0x8ffffffc
#endif

#ifdef CONFIG_SPL_BUILD
#define CONFIG_SYS_MONITOR_BASE	CONFIG_SPL_TEXT_BASE
#else
#define CONFIG_SYS_MONITOR_BASE	CONFIG_SYS_TEXT_BASE	/* start of monitor */
#endif

/* High Level Configuration Options */
#define CONFIG_BOOKE			/* BOOKE */
#define CONFIG_E500			/* BOOKE e500 family */
#define CONFIG_MPC85xx
#define CONFIG_FSL_IFC			/* Enable IFC Support */
#define CONFIG_SYS_HAS_SERDES		/* common SERDES init code */

#define CONFIG_PCI			/* Enable PCI/PCIE */
#if defined(CONFIG_PCI)
#define CONFIG_PCIE1			/* PCIE controler 1 (slot 1) */
#define CONFIG_FSL_PCI_INIT		/* Use common FSL init code */
#define CONFIG_PCI_INDIRECT_BRIDGE	/* indirect PCI bridge support */
#define CONFIG_FSL_PCIE_RESET		/* need PCIe reset errata */
#define CONFIG_SYS_PCI_64BIT		/* enable 64-bit PCI resources */

#define CONFIG_CMD_NET
#define CONFIG_CMD_PCI

#define CONFIG_E1000			/*  E1000 pci Ethernet card*/

/*
 * PCI Windows
 * Memory space is mapped 1-1, but I/O space must start from 0.
 */
/* controller 1, Slot 1, tgtid 1, Base address a000 */
#define CONFIG_SYS_PCIE1_NAME		"PCIe Slot"
#define CONFIG_SYS_PCIE1_MEM_VIRT	0x90000000
#define CONFIG_SYS_PCIE1_MEM_BUS	0x90000000
#define CONFIG_SYS_PCIE1_MEM_PHYS	0x90000000
#define CONFIG_SYS_PCIE1_MEM_SIZE	0x10000000	/* 256M */
#define CONFIG_SYS_PCIE1_IO_VIRT	0xC0010000
#define CONFIG_SYS_PCIE1_IO_BUS		0x00000000
#define CONFIG_SYS_PCIE1_IO_SIZE	0x00010000	/* 64k */
#define CONFIG_SYS_PCIE1_IO_PHYS	0xC0010000

#define CONFIG_PCI_PNP			/* do pci plug-and-play */

#define CONFIG_PCI_SCAN_SHOW		/* show pci devices on startup */
#define CONFIG_DOS_PARTITION
#endif

#define CONFIG_FSL_LAW			/* Use common FSL init code */
#define CONFIG_ENV_OVERWRITE
#define CONFIG_TSEC_ENET /* ethernet */

#if defined(CONFIG_SYS_CLK_100_DDR_100)
#define CONFIG_SYS_CLK_FREQ	100000000
#define CONFIG_DDR_CLK_FREQ	100000000
#elif defined(CONFIG_SYS_CLK_100_DDR_133)
#define CONFIG_SYS_CLK_FREQ	100000000
#define CONFIG_DDR_CLK_FREQ	133000000
#endif

#define CONFIG_MP

#define CONFIG_HWCONFIG
/*
 * These can be toggled for performance analysis, otherwise use default.
 */
#define CONFIG_L2_CACHE			/* toggle L2 cache */
#define CONFIG_BTB			/* enable branch predition */

#define CONFIG_SYS_MEMTEST_START	0x01000000	/* memtest works on */
#define CONFIG_SYS_MEMTEST_END		0x01ffffff

/* DDR Setup */
#define CONFIG_FSL_DDR3
#define CONFIG_SYS_SPD_BUS_NUM		0
#define SPD_EEPROM_ADDRESS1		0x54 /* I2C access */
#define SPD_EEPROM_ADDRESS2		0x56 /* I2C access */
#define CONFIG_FSL_DDR_INTERACTIVE

#define CONFIG_MEM_INIT_VALUE		0xDeadBeef

#define CONFIG_SYS_SDRAM_SIZE		(1024)
#define CONFIG_SYS_DDR_SDRAM_BASE	0x00000000
#define CONFIG_SYS_SDRAM_BASE		CONFIG_SYS_DDR_SDRAM_BASE

#define CONFIG_DIMM_SLOTS_PER_CTLR	1

/* DDR3 Controller Settings */
#define CONFIG_CHIP_SELECTS_PER_CTRL	1
#define CONFIG_SYS_DDR_CS0_BNDS		0x0000003F
#define CONFIG_SYS_DDR_CS0_CONFIG_1333	0x80004302
#define CONFIG_SYS_DDR_CS0_CONFIG_800	0x80014302
#define CONFIG_SYS_DDR_CS0_CONFIG_2	0x00000000
#define CONFIG_SYS_DDR_DATA_INIT	0xdeadbeef
#define CONFIG_SYS_DDR_INIT_ADDR	0x00000000
#define CONFIG_SYS_DDR_INIT_EXT_ADDR	0x00000000
#define CONFIG_SYS_DDR_MODE_CONTROL	0x00000000
#define CONFIG_SYS_DDR1_CS0_BNDS       0x0040007F

#define CONFIG_SYS_DDR_ZQ_CONTROL	0x89080600
#define CONFIG_SYS_DDR_SR_CNTR		0x00000000
#define CONFIG_SYS_DDR_RCW_1		0x00000000
#define CONFIG_SYS_DDR_RCW_2		0x00000000
#define CONFIG_SYS_DDR_CONTROL_800		0x470C0000
#define CONFIG_SYS_DDR_CONTROL_2_800	0x04401050
#define CONFIG_SYS_DDR_TIMING_4_800		0x00220001
#define CONFIG_SYS_DDR_TIMING_5_800		0x03402400

#define CONFIG_SYS_DDR_CONTROL_1333		0x470C0008
#define CONFIG_SYS_DDR_CONTROL_2_1333	0x24401010
#define CONFIG_SYS_DDR_TIMING_4_1333		0x00000001
#define CONFIG_SYS_DDR_TIMING_5_1333		0x03401400

#define CONFIG_SYS_DDR_TIMING_3_800		0x00020000
#define CONFIG_SYS_DDR_TIMING_0_800		0x00330004
#define CONFIG_SYS_DDR_TIMING_1_800		0x6f6B4846
#define CONFIG_SYS_DDR_TIMING_2_800		0x0FA8C8CF
#define CONFIG_SYS_DDR_CLK_CTRL_800		0x03000000
#define CONFIG_SYS_DDR_MODE_1_800		0x40461520
#define CONFIG_SYS_DDR_MODE_2_800		0x8000c000
#define CONFIG_SYS_DDR_INTERVAL_800		0x0C300000
#define CONFIG_SYS_DDR_WRLVL_CONTROL_800	0x8655A608

#define CONFIG_SYS_DDR_TIMING_3_1333		0x01061000
#define CONFIG_SYS_DDR_TIMING_0_1333		0x00440104
#define CONFIG_SYS_DDR_TIMING_1_1333		0x98913A45
#define CONFIG_SYS_DDR_TIMING_2_1333		0x0FB8B114
#define CONFIG_SYS_DDR_CLK_CTRL_1333		0x02800000
#define CONFIG_SYS_DDR_MODE_1_1333		0x00061A50
#define CONFIG_SYS_DDR_MODE_2_1333		0x00100000
#define CONFIG_SYS_DDR_INTERVAL_1333		0x144E0513
#define CONFIG_SYS_DDR_WRLVL_CONTROL_1333	0x8655F607

/*FIXME: the following params are constant w.r.t diff freq
combinations. this should be removed later
*/
#if CONFIG_DDR_CLK_FREQ == 100000000
#define CONFIG_SYS_DDR_CS0_CONFIG CONFIG_SYS_DDR_CS0_CONFIG_800
#define CONFIG_SYS_DDR_CONTROL		CONFIG_SYS_DDR_CONTROL_800
#define CONFIG_SYS_DDR_CONTROL_2 CONFIG_SYS_DDR_CONTROL_2_800
#define CONFIG_SYS_DDR_TIMING_4	CONFIG_SYS_DDR_TIMING_4_800
#define CONFIG_SYS_DDR_TIMING_5	CONFIG_SYS_DDR_TIMING_5_800
#elif CONFIG_DDR_CLK_FREQ == 133000000
#define CONFIG_SYS_DDR_CS0_CONFIG CONFIG_SYS_DDR_CS0_CONFIG_1333
#define CONFIG_SYS_DDR_CONTROL		CONFIG_SYS_DDR_CONTROL_1333
#define CONFIG_SYS_DDR_CONTROL_2	CONFIG_SYS_DDR_CONTROL_2_1333
#define CONFIG_SYS_DDR_TIMING_4	CONFIG_SYS_DDR_TIMING_4_1333
#define CONFIG_SYS_DDR_TIMING_5	CONFIG_SYS_DDR_TIMING_5_1333
#else
#define CONFIG_SYS_DDR_CS0_CONFIG CONFIG_SYS_DDR_CS0_CONFIG_800
#define CONFIG_SYS_DDR_CONTROL		CONFIG_SYS_DDR_CONTROL_800
#define CONFIG_SYS_DDR_CONTROL_2	CONFIG_SYS_DDR_CONTROL_2_800
#define CONFIG_SYS_DDR_TIMING_4	CONFIG_SYS_DDR_TIMING_4_800
#define CONFIG_SYS_DDR_TIMING_5	CONFIG_SYS_DDR_TIMING_5_800
#endif


/* relocated CCSRBAR */
#define CONFIG_SYS_CCSRBAR	CONFIG_SYS_CCSRBAR_DEFAULT
#define CONFIG_SYS_CCSRBAR_PHYS_LOW	CONFIG_SYS_CCSRBAR_DEFAULT

#define CONFIG_SYS_IMMR		CONFIG_SYS_CCSRBAR

/*
 * IFC Definitions
 */
/* NOR Flash on IFC */

#ifdef CONFIG_SPL_BUILD
#define CONFIG_SYS_NO_FLASH
#endif
#define CONFIG_SYS_FLASH_BASE		0x88000000
#define CONFIG_SYS_MAX_FLASH_SECT	1024	/* Max number of sector: 32M */

#define CONFIG_SYS_FLASH_BASE_PHYS	CONFIG_SYS_FLASH_BASE

#define CONFIG_SYS_NOR_CSPR	0x88000101
#define CONFIG_SYS_NOR_AMASK	IFC_AMASK(128*1024*1024)
#define CONFIG_SYS_NOR_CSOR	CSOR_NOR_ADM_SHIFT(5)
/* NOR Flash Timing Params */

#define CONFIG_SYS_NOR_FTIM0	(FTIM0_NOR_TACSE(0x01) \
				| FTIM0_NOR_TEADC(0x03) \
				| FTIM0_NOR_TAVDS(0x00) \
				| FTIM0_NOR_TEAHC(0x0f))
#define CONFIG_SYS_NOR_FTIM1	(FTIM1_NOR_TACO(0x1d) \
				| FTIM1_NOR_TRAD_NOR(0x09) \
				| FTIM1_NOR_TSEQRAD_NOR(0x09))
#define CONFIG_SYS_NOR_FTIM2	(FTIM2_NOR_TCS(0x1) \
				| FTIM2_NOR_TCH(0x4) \
				| FTIM2_NOR_TWPH(0x7) \
				| FTIM2_NOR_TWP(0x1e))
#define CONFIG_SYS_NOR_FTIM3	0x0

#define CONFIG_SYS_FLASH_BANKS_LIST	{CONFIG_SYS_FLASH_BASE_PHYS}
#define CONFIG_SYS_FLASH_QUIET_TEST
#define CONFIG_FLASH_SHOW_PROGRESS	45	/* count down from 45/5: 9..1 */
#define CONFIG_SYS_MAX_FLASH_BANKS	1	/* number of banks */

#undef CONFIG_SYS_FLASH_CHECKSUM
#define CONFIG_SYS_FLASH_ERASE_TOUT	60000	/* Flash Erase Timeout (ms) */
#define CONFIG_SYS_FLASH_WRITE_TOUT	500	/* Flash Write Timeout (ms) */

/* CFI for NOR Flash */
#define CONFIG_FLASH_CFI_DRIVER
#define CONFIG_SYS_FLASH_CFI
#define CONFIG_SYS_FLASH_EMPTY_INFO
#define CONFIG_SYS_FLASH_USE_BUFFER_WRITE

/* NAND Flash on IFC */
#define CONFIG_SYS_NAND_BASE		0xff800000
#define CONFIG_SYS_NAND_BASE_PHYS	CONFIG_SYS_NAND_BASE

#define CONFIG_SYS_NAND_CSPR	(CSPR_PHYS_ADDR(CONFIG_SYS_NAND_BASE_PHYS) \
				| CSPR_PORT_SIZE_8 /* Port Size = 8 bit */ \
				| CSPR_MSEL_NAND	/* MSEL = NAND */ \
				| CSPR_V)
#define CONFIG_SYS_NAND_AMASK	IFC_AMASK(64*1024)

#define CONFIG_SYS_NAND_CSOR    (CSOR_NAND_ECC_ENC_EN   /* ECC on encode */ \
				| CSOR_NAND_ECC_DEC_EN  /* ECC on decode */ \
				| CSOR_NAND_ECC_MODE_4  /* 4-bit ECC */ \
				| CSOR_NAND_RAL_2	/* RAL = 2Byes */ \
				| CSOR_NAND_PGS_2K	/* Page Size = 2K */ \
				| CSOR_NAND_SPRZ_64/* Spare size = 64 */ \
				| CSOR_NAND_PB(64))	/*Pages Per Block = 64*/

/* NAND Flash Timing Params */
#define CONFIG_SYS_NAND_FTIM0		(FTIM0_NAND_TCCST(0x03) \
					| FTIM0_NAND_TWP(0x05) \
					| FTIM0_NAND_TWCHT(0x02) \
					| FTIM0_NAND_TWH(0x04))
#define CONFIG_SYS_NAND_FTIM1		(FTIM1_NAND_TADLE(0x1c) \
					| FTIM1_NAND_TWBE(0x1e) \
					| FTIM1_NAND_TRR(0x07) \
					| FTIM1_NAND_TRP(0x05))
#define CONFIG_SYS_NAND_FTIM2		(FTIM2_NAND_TRAD(0x08) \
					| FTIM2_NAND_TREH(0x04) \
					| FTIM2_NAND_TWHRE(0x11))
#define CONFIG_SYS_NAND_FTIM3		FTIM3_NAND_TWW(0x04)

#define CONFIG_SYS_NAND_DDR_LAW		11

/* NAND */
#define CONFIG_SYS_NAND_BASE_LIST	{ CONFIG_SYS_NAND_BASE }
#define CONFIG_SYS_MAX_NAND_DEVICE	1
#define CONFIG_MTD_NAND_VERIFY_WRITE
#define CONFIG_CMD_NAND

#define CONFIG_SYS_NAND_BLOCK_SIZE	(128 * 1024)

#ifndef CONFIG_SPL_BUILD
#define CONFIG_FSL_QIXIS
#endif
#ifdef CONFIG_FSL_QIXIS
#define CONFIG_SYS_FPGA_BASE	0xffb00000
#define CONFIG_SYS_I2C_FPGA_ADDR	0x66
#define QIXIS_BASE	CONFIG_SYS_FPGA_BASE
#define QIXIS_LBMAP_SWITCH	9
#define QIXIS_LBMAP_MASK	0x07
#define QIXIS_LBMAP_SHIFT	0
#define QIXIS_LBMAP_DFLTBANK		0x00
#define QIXIS_LBMAP_ALTBANK		0x04
#define QIXIS_RST_CTL_RESET		0x83
#define QIXIS_RCFG_CTL_RECONFIG_IDLE	0x20
#define QIXIS_RCFG_CTL_RECONFIG_START	0x21
#define QIXIS_RCFG_CTL_WATCHDOG_ENBLE	0x08

#define CONFIG_SYS_FPGA_BASE_PHYS	CONFIG_SYS_FPGA_BASE

#define CONFIG_SYS_CSPR2		(CSPR_PHYS_ADDR(CONFIG_SYS_FPGA_BASE) \
					| CSPR_PORT_SIZE_8 \
					| CSPR_MSEL_GPCM \
					| CSPR_V)
#define CONFIG_SYS_AMASK2		IFC_AMASK(64*1024)
#define CONFIG_SYS_CSOR2		0x0
/* CPLD Timing parameters for IFC CS3 */
#define CONFIG_SYS_CS2_FTIM0		(FTIM0_GPCM_TACSE(0x0e) | \
					FTIM0_GPCM_TEADC(0x0e) | \
					FTIM0_GPCM_TEAHC(0x0e))
#define CONFIG_SYS_CS2_FTIM1		(FTIM1_GPCM_TACO(0x0e) | \
					FTIM1_GPCM_TRAD(0x1f))
#define CONFIG_SYS_CS2_FTIM2		(FTIM2_GPCM_TCS(0x0e) | \
					FTIM2_GPCM_TCH(0x0) | \
					FTIM2_GPCM_TWP(0x1f))
#define CONFIG_SYS_CS2_FTIM3		0x0
#endif

/* Set up IFC registers for boot location NOR/NAND */
#if defined(CONFIG_NAND)
#define CONFIG_SYS_CSPR0		CONFIG_SYS_NAND_CSPR
#define CONFIG_SYS_AMASK0		CONFIG_SYS_NAND_AMASK
#define CONFIG_SYS_CSOR0		CONFIG_SYS_NAND_CSOR
#define CONFIG_SYS_CS0_FTIM0		CONFIG_SYS_NAND_FTIM0
#define CONFIG_SYS_CS0_FTIM1		CONFIG_SYS_NAND_FTIM1
#define CONFIG_SYS_CS0_FTIM2		CONFIG_SYS_NAND_FTIM2
#define CONFIG_SYS_CS0_FTIM3		CONFIG_SYS_NAND_FTIM3
#define CONFIG_SYS_CSPR1		CONFIG_SYS_NOR_CSPR
#define CONFIG_SYS_AMASK1		CONFIG_SYS_NOR_AMASK
#define CONFIG_SYS_CSOR1		CONFIG_SYS_NOR_CSOR
#define CONFIG_SYS_CS1_FTIM0		CONFIG_SYS_NOR_FTIM0
#define CONFIG_SYS_CS1_FTIM1		CONFIG_SYS_NOR_FTIM1
#define CONFIG_SYS_CS1_FTIM2		CONFIG_SYS_NOR_FTIM2
#define CONFIG_SYS_CS1_FTIM3		CONFIG_SYS_NOR_FTIM3
#else
#define CONFIG_SYS_CSPR0		CONFIG_SYS_NOR_CSPR
#define CONFIG_SYS_AMASK0		CONFIG_SYS_NOR_AMASK
#define CONFIG_SYS_CSOR0		CONFIG_SYS_NOR_CSOR
#define CONFIG_SYS_CS0_FTIM0		CONFIG_SYS_NOR_FTIM0
#define CONFIG_SYS_CS0_FTIM1		CONFIG_SYS_NOR_FTIM1
#define CONFIG_SYS_CS0_FTIM2		CONFIG_SYS_NOR_FTIM2
#define CONFIG_SYS_CS0_FTIM3		CONFIG_SYS_NOR_FTIM3
#define CONFIG_SYS_CSPR1		CONFIG_SYS_NAND_CSPR
#define CONFIG_SYS_AMASK1		CONFIG_SYS_NAND_AMASK
#define CONFIG_SYS_CSOR1		CONFIG_SYS_NAND_CSOR
#define CONFIG_SYS_CS1_FTIM0		CONFIG_SYS_NAND_FTIM0
#define CONFIG_SYS_CS1_FTIM1		CONFIG_SYS_NAND_FTIM1
#define CONFIG_SYS_CS1_FTIM2		CONFIG_SYS_NAND_FTIM2
#define CONFIG_SYS_CS1_FTIM3		CONFIG_SYS_NAND_FTIM3
#endif

#define CONFIG_BOARD_EARLY_INIT_F	/* Call board_pre_init */
#define CONFIG_BOARD_EARLY_INIT_R

#define CONFIG_SYS_INIT_RAM_LOCK
#define CONFIG_SYS_INIT_RAM_ADDR	0xffd00000	/* stack in RAM */
#define CONFIG_SYS_INIT_RAM_END		0x00004000 /* End of used area in RAM */

#define CONFIG_SYS_GBL_DATA_OFFSET	(CONFIG_SYS_INIT_RAM_END \
						- GENERATED_GBL_DATA_SIZE)
#define CONFIG_SYS_INIT_SP_OFFSET	CONFIG_SYS_GBL_DATA_OFFSET

#define CONFIG_SYS_MONITOR_LEN		(256 * 1024) /* Reserve 256 kB for Mon*/
#define CONFIG_SYS_MALLOC_LEN		(1024 * 1024)	/* Reserved for malloc*/

/* Serial Port */
#define CONFIG_CONS_INDEX	1
#undef	CONFIG_SERIAL_SOFTWARE_FIFO
#define CONFIG_SYS_NS16550
#define CONFIG_SYS_NS16550_SERIAL
#define CONFIG_SYS_NS16550_REG_SIZE	1
#define CONFIG_SYS_NS16550_CLK		get_bus_freq(0)
#ifdef CONFIG_SPL_BUILD
#define CONFIG_NS16550_MIN_FUNCTIONS
#endif

#define CONFIG_SERIAL_MULTI	1 /* Enable both serial ports */
#define CONFIG_SYS_CONSOLE_IS_IN_ENV	/* determine from environment */

#define CONFIG_SYS_BAUDRATE_TABLE	\
	{300, 600, 1200, 2400, 4800, 9600, 19200, 38400, 57600, 115200}

#define CONFIG_SYS_NS16550_COM1	(CONFIG_SYS_CCSRBAR + 0x4500)
#define CONFIG_SYS_NS16550_COM2	(CONFIG_SYS_CCSRBAR + 0x4600)
#define CONFIG_SYS_NS16550_COM3	(CONFIG_SYS_CCSRBAR + 0x4700)
#define CONFIG_SYS_NS16550_COM4	(CONFIG_SYS_CCSRBAR + 0x4800)

/* Use the HUSH parser */
#define CONFIG_SYS_HUSH_PARSER    /* hush parser */
#ifdef	CONFIG_SYS_HUSH_PARSER
#define CONFIG_SYS_PROMPT_HUSH_PS2 "> "
#endif

/*
 * Pass open firmware flat tree
 */
#define CONFIG_OF_LIBFDT
#define CONFIG_OF_BOARD_SETUP
#define CONFIG_OF_STDOUT_VIA_ALIAS

/* new uImage format support */
#define CONFIG_FIT
#define CONFIG_FIT_VERBOSE	/* enable fit_format_{error,warning}() */

#define CONFIG_FSL_I2C			/* Use FSL common I2C driver */
#define CONFIG_HARD_I2C			/* I2C with hardware support */
#undef CONFIG_SOFT_I2C			/* I2C bit-banged */
#define CONFIG_I2C_MULTI_BUS
#define CONFIG_I2C_CMD_TREE
#define CONFIG_SYS_I2C_SPEED		400800 /* I2C speed and slave address*/
#define CONFIG_SYS_I2C_SLAVE		0x7F
#define CONFIG_SYS_I2C_OFFSET		0x3000
#define CONFIG_SYS_I2C2_OFFSET		0x3100

/* I2C EEPROM */
#define CONFIG_ID_EEPROM
#ifdef CONFIG_ID_EEPROM
#define CONFIG_SYS_I2C_EEPROM_NXID
#endif
#define CONFIG_SYS_I2C_EEPROM_ADDR	0x57
#define CONFIG_SYS_I2C_EEPROM_ADDR_LEN 1
#define CONFIG_SYS_EEPROM_BUS_NUM	0

/* enable read and write access to EEPROM */
#define CONFIG_CMD_EEPROM
#define CONFIG_SYS_I2C_MULTI_EEPROMS
#define CONFIG_SYS_I2C_EEPROM_ADDR_LEN 1
#define CONFIG_SYS_EEPROM_PAGE_WRITE_BITS 3
#define CONFIG_SYS_EEPROM_PAGE_WRITE_DELAY_MS 5

/* I2C FPGA */
#define CONFIG_I2C_FPGA
#define CONFIG_SYS_I2C_FPGA_ADDR	0x66

#define CONFIG_RTC_DS3231
#define CONFIG_SYS_I2C_RTC_ADDR		0x68

/*
 * SPI interface will not be available in case of NAND boot SPI CS0 will be
 * used for SLIC
 */
/* eSPI - Enhanced SPI */
#define CONFIG_FSL_ESPI  /* SPI */
#ifdef CONFIG_FSL_ESPI
#define CONFIG_SPI_FLASH
#define CONFIG_SPI_FLASH_SPANSION
#define CONFIG_CMD_SF
#define CONFIG_SF_DEFAULT_SPEED		10000000
#define CONFIG_SF_DEFAULT_MODE		SPI_MODE_0
#endif

#if defined(CONFIG_TSEC_ENET)

#define CONFIG_MII			/* MII PHY management */
#define CONFIG_MII_DEFAULT_TSEC	1	/* Allow unregistered phys */
#define CONFIG_TSEC1	1
#define CONFIG_TSEC1_NAME	"eTSEC1"
#define CONFIG_TSEC2	1
#define CONFIG_TSEC2_NAME	"eTSEC2"

#define TSEC1_PHY_ADDR		0
#define TSEC2_PHY_ADDR		1

#define TSEC1_FLAGS		(TSEC_GIGABIT | TSEC_REDUCED)
#define TSEC2_FLAGS		(TSEC_GIGABIT | TSEC_REDUCED)

#define TSEC1_PHYIDX		0
#define TSEC2_PHYIDX		0

#define CONFIG_ETHPRIME		"eTSEC1"

#define CONFIG_PHY_GIGE		/* Include GbE speed/duplex detection */

/* TBI PHY configuration for SGMII mode */
#define CONFIG_TSEC_TBICR_SETTINGS ( \
		TBICR_PHY_RESET \
		| TBICR_ANEG_ENABLE \
		| TBICR_FULL_DUPLEX \
		| TBICR_SPEED1_SET \
		)

#endif	/* CONFIG_TSEC_ENET */

#define CONFIG_MMC
#ifdef CONFIG_MMC
#define CONFIG_CMD_MMC
#define CONFIG_DOS_PARTITION
#define CONFIG_FSL_ESDHC
#define CONFIG_GENERIC_MMC
#define CONFIG_SYS_FSL_ESDHC_ADDR	CONFIG_SYS_MPC85xx_ESDHC_ADDR
#endif

#define CONFIG_USB_EHCI  /* USB */
#ifdef CONFIG_USB_EHCI
#define CONFIG_CMD_USB
#define CONFIG_EHCI_HCD_INIT_AFTER_RESET
#define CONFIG_USB_EHCI_FSL
#define CONFIG_USB_STORAGE
#define CONFIG_HAS_FSL_DR_USB
#endif

/*
 * Environment
 */
#if defined(CONFIG_RAMBOOT_SDCARD)
#define CONFIG_ENV_IS_IN_MMC
#define CONFIG_SYS_MMC_ENV_DEV		0
#define CONFIG_ENV_SIZE			0x2000
#elif defined(CONFIG_RAMBOOT_SPIFLASH)
#define CONFIG_ENV_IS_IN_SPI_FLASH
#define CONFIG_ENV_SPI_BUS	0
#define CONFIG_ENV_SPI_CS	0
#define CONFIG_ENV_SPI_MAX_HZ	10000000
#define CONFIG_ENV_SPI_MODE	0
#define CONFIG_ENV_OFFSET	0x100000	/* 1MB */
#define CONFIG_ENV_SECT_SIZE	0x10000
#define CONFIG_ENV_SIZE		0x2000
#elif defined(CONFIG_NAND)
#define CONFIG_ENV_IS_IN_NAND
#define CONFIG_ENV_SIZE		CONFIG_SYS_NAND_BLOCK_SIZE
#define CONFIG_ENV_OFFSET	((512 * 1024) + CONFIG_SYS_NAND_BLOCK_SIZE)
#define CONFIG_ENV_RANGE	(3 * CONFIG_ENV_SIZE)
#elif defined(CONFIG_SYS_RAMBOOT)
#define CONFIG_ENV_IS_NOWHERE		/* Store ENV in memory only */
#define CONFIG_ENV_ADDR			(CONFIG_SYS_MONITOR_BASE - 0x1000)
#define CONFIG_ENV_SIZE			0x2000
#else
#define CONFIG_ENV_IS_IN_FLASH
#if CONFIG_SYS_MONITOR_BASE > 0xfff80000
#define CONFIG_ENV_ADDR	0xfff80000
#else
#define CONFIG_ENV_ADDR	(CONFIG_SYS_MONITOR_BASE - CONFIG_ENV_SECT_SIZE)
#endif
#define CONFIG_ENV_SIZE		0x2000
#define CONFIG_ENV_SECT_SIZE	0x20000 /* 128K (one sector) */
#endif

#define CONFIG_LOADS_ECHO		/* echo on for serial download */
#define CONFIG_SYS_LOADS_BAUD_CHANGE	/* allow baudrate change */

/*
 * Command line configuration.
 */
#include <config_cmd_default.h>

#define CONFIG_CMD_DATE
#define CONFIG_CMD_DHCP
#define CONFIG_CMD_ELF
#define CONFIG_CMD_ERRATA
#define CONFIG_CMD_I2C
#define CONFIG_CMD_IRQ
#define CONFIG_CMD_MII
#define CONFIG_CMD_PING
#define CONFIG_CMD_SETEXPR
#define CONFIG_CMD_REGINFO

#if defined(CONFIG_MMC) || defined(CONFIG_USB_EHCI)
#define CONFIG_CMD_EXT2
#define CONFIG_CMD_FAT
#define CONFIG_DOS_PARTITION
#endif

/*
 * Miscellaneous configurable options
 */
#define CONFIG_SYS_LONGHELP			/* undef to save memory	*/
#define CONFIG_CMDLINE_EDITING			/* Command-line editing */
#define CONFIG_AUTO_COMPLETE			/* add autocompletion support */
#define CONFIG_SYS_LOAD_ADDR	0x2000000	/* default load address */
#define CONFIG_SYS_PROMPT	"=> "		/* Monitor Command Prompt */

#if defined(CONFIG_CMD_KGDB)
#define CONFIG_SYS_CBSIZE	1024		/* Console I/O Buffer Size */
#else
#define CONFIG_SYS_CBSIZE	256		/* Console I/O Buffer Size */
#endif
#define CONFIG_SYS_PBSIZE (CONFIG_SYS_CBSIZE+sizeof(CONFIG_SYS_PROMPT)+16)
						/* Print Buffer Size */
#define CONFIG_SYS_MAXARGS	16		/* max number of command args */
#define CONFIG_SYS_BARGSIZE	CONFIG_SYS_CBSIZE/* Boot Argument Buffer Size */
#define CONFIG_SYS_HZ		1000		/* decrementer freq:1ms ticks */


/*
 * For booting Linux, the board info and command line data
 * have to be in the first 64 MB of memory, since this is
 * the maximum mapped by the Linux kernel during initialization.
 */
#define CONFIG_SYS_BOOTMAPSZ	(64 << 20) /* Initial Memory map for Linux */
#define CONFIG_SYS_BOOTM_LEN	(64 << 20) /* Increase max gunzip size */

#if defined(CONFIG_CMD_KGDB)
#define CONFIG_KGDB_BAUDRATE	230400	/* speed to run kgdb serial port */
#define CONFIG_KGDB_SER_INDEX	2	/* which serial port to use */
#endif

/*
 * Environment Configuration
 */

#if defined(CONFIG_TSEC_ENET)
#define CONFIG_HAS_ETH0
#define CONFIG_HAS_ETH1
#endif

#define CONFIG_HOSTNAME		BSC9132qds
#define CONFIG_ROOTPATH		"/opt/nfsroot"
#define CONFIG_BOOTFILE		"uImage"
#define CONFIG_UBOOTPATH	"u-boot.bin"

#define CONFIG_BAUDRATE		115200

#ifdef CONFIG_SDCARD
#define CONFIG_DEF_HWCONFIG	"hwconfig=usb1:dr_mode=host,phy_type=ulpi\0"
#else
#define CONFIG_DEF_HWCONFIG	"hwconfig=sim;usb1:dr_mode=host,phy_type=ulpi\0"
#endif

#define	CONFIG_EXTRA_ENV_SETTINGS				\
	"netdev=eth0\0"						\
	"uboot=" CONFIG_UBOOTPATH "\0"				\
	"loadaddr=1000000\0"			\
	"bootfile=uImage\0"	\
	"consoledev=ttyS0\0"				\
	"ramdiskaddr=2000000\0"			\
	"ramdiskfile=rootfs.ext2.gz.uboot\0"		\
	"fdtaddr=c00000\0"				\
	"fdtfile=bsc9132qds.dtb\0"		\
	"bdev=sda1\0"	\
	CONFIG_DEF_HWCONFIG\
	"othbootargs=mem=880M ramdisk_size=600000 " \
		"default_hugepagesz=256m hugepagesz=256m hugepages=1 " \
		"isolcpus=0\0" \
	"usbext2boot=setenv bootargs root=/dev/ram rw "	\
		"console=$consoledev,$baudrate $othbootargs; "	\
		"usb start;"			\
		"ext2load usb 0:4 $loadaddr $bootfile;"		\
		"ext2load usb 0:4 $fdtaddr $fdtfile;"	\
		"ext2load usb 0:4 $ramdiskaddr $ramdiskfile;"	\
		"bootm $loadaddr $ramdiskaddr $fdtaddr\0"	\
	"debug_halt_off=mw ff7e0e30 0xf0000000;"

#define CONFIG_NFSBOOTCOMMAND	\
	"setenv bootargs root=/dev/nfs rw "	\
	"nfsroot=$serverip:$rootpath "	\
	"ip=$ipaddr:$serverip:$gatewayip:$netmask:$hostname:$netdev:off " \
	"console=$consoledev,$baudrate $othbootargs;" \
	"tftp $loadaddr $bootfile;"	\
	"tftp $fdtaddr $fdtfile;"	\
	"bootm $loadaddr - $fdtaddr"

#define CONFIG_HDBOOT	\
	"setenv bootargs root=/dev/$bdev rw rootdelay=30 "	\
	"console=$consoledev,$baudrate $othbootargs;" \
	"usb start;"	\
	"ext2load usb 0:1 $loadaddr /boot/$bootfile;"	\
	"ext2load usb 0:1 $fdtaddr /boot/$fdtfile;"	\
	"bootm $loadaddr - $fdtaddr"

#define CONFIG_RAMBOOTCOMMAND		\
	"setenv bootargs root=/dev/ram rw "	\
	"console=$consoledev,$baudrate $othbootargs; "	\
	"tftp $ramdiskaddr $ramdiskfile;"	\
	"tftp $loadaddr $bootfile;"		\
	"tftp $fdtaddr $fdtfile;"		\
	"bootm $loadaddr $ramdiskaddr $fdtaddr"

#define CONFIG_BOOTCOMMAND CONFIG_RAMBOOTCOMMAND

#endif	/* __CONFIG_H */
