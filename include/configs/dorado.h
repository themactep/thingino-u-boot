/*
 * Ingenic dorado configuration
 *
 * Copyright (c) 2014 Ingenic Semiconductor Co.,Ltd
 * Author: Zoro <ykli@ingenic.cn>
 * Based on: include/configs/urboard.h
 *           Written by Paul Burton <paul.burton@imgtec.com>
 *
 * SPDX-License-Identifier:	GPL-2.0+
 */

#ifndef __CONFIG_DORAOD_H__
#define __CONFIG_DORAOD_H__

/**
 * Basic configuration(SOC, Cache, UART, DDR).
 */
#define CONFIG_MIPS32		/* MIPS32 CPU core */
#define CONFIG_CPU_XBURST
#define CONFIG_SYS_LITTLE_ENDIAN
#define CONFIG_M200		/* M200 SoC */
#ifndef CONFIG_DORADO_V30 // no def
#define CONFIG_DDR_AUTO_SELF_REFRESH
#endif //endif CONFIG_DORADO_V30
#define CONFIG_SPL_DDR_SOFT_TRAINING

#ifndef CONFIG_RVMS
#define CONFIG_SYS_APLL_FREQ		1200000000	/*If APLL not use mast be set 0*/
#define CONFIG_SYS_MPLL_FREQ		600000000	/*If MPLL not use mast be set 0*/
#define CONFIG_CPU_SEL_PLL		APLL
#define CONFIG_DDR_SEL_PLL		MPLL
#define CONFIG_SYS_CPU_FREQ		1200000000
#define CONFIG_SYS_MEM_FREQ		300000000

#else /* defined CONFIG_RVMS */
#define CONFIG_SYS_APLL_FREQ		800000000	/*If APLL not use mast be set 0*/
#define CONFIG_SYS_MPLL_FREQ		600000000	/*If MPLL not use mast be set 0*/
#define CONFIG_CPU_SEL_PLL		APLL
#define CONFIG_DDR_SEL_PLL		MPLL
#define CONFIG_SYS_CPU_FREQ		800000000
#define CONFIG_SYS_MEM_FREQ		300000000
#endif

#define CONFIG_SYS_EXTAL		24000000	/* EXTAL freq: 24 MHz */
#define CONFIG_SYS_HZ			1000		/* incrementer freq */


#define CONFIG_SYS_DCACHE_SIZE		32768
#define CONFIG_SYS_ICACHE_SIZE		32768
#define CONFIG_SYS_CACHELINE_SIZE	32

#define CONFIG_SYS_UART_INDEX		1
#ifndef CONFIG_RVMS
#define CONFIG_BAUDRATE			57600
#else /* defined CONFIG_RVMS */
#define CONFIG_BAUDRATE			115200
#endif

/*#define CONFIG_DDR_TEST_CPU
#define CONFIG_DDR_TEST*/
#define CONFIG_DDR_PARAMS_CREATOR
#define CONFIG_DDR_HOST_CC
#define CONFIG_DDR_FORCE_SELECT_CS1

#ifdef CONFIG_DORADO_V30
#define CONFIG_DDR_TYPE_DDR3
#define CONFIG_DDR3_H5TQ1G83DFR_H9C
#else
#define CONFIG_DDR_TYPE_LPDDR2
/*#define CONFIG_MCP_H9TP32A8JDMC_PRKGM_LPDDR2*/
#define CONFIG_MCP_SAMSUNG_KMN5X000ZM_LPDDR2
#endif // CONFIG_DORADO_V30
#ifdef CONFIG_DORADO_V20
#define CONFIG_DDR_CS0          1   /* 1-connected, 0-disconnected */
#define CONFIG_DDR_CS1          1   /* 1-connected, 0-disconnected */
#define CONFIG_DDR_DW32         1   /* 1-32bit-width, 0-16bit-width */
#else
#define CONFIG_DDR_CS0          1   /* 1-connected, 0-disconnected */
#define CONFIG_DDR_CS1          0   /* 1-connected, 0-disconnected */
#define CONFIG_DDR_DW32         1   /* 1-32bit-width, 0-16bit-width */
#endif

/* #define CONFIG_DDR_DLL_OFF */
/*
 * #define CONFIG_DDR_CHIP_ODT
 * #define CONFIG_DDR_PHY_ODT
 * #define CONFIG_DDR_PHY_DQ_ODT
 * #define CONFIG_DDR_PHY_DQS_ODT
 * #define CONFIG_DDR_PHY_IMPED_PULLUP		0xe
 * #define CONFIG_DDR_PHY_IMPED_PULLDOWN	0xe
 */

/**
 * Boot arguments definitions.
 *
 * console=ttyS1: kernel boot time (7s)
 * console=null:  kernel boot time (4s), for product board
 */
#ifndef CONFIG_RVMS
#if defined(CONFIG_DORADO_V21) || defined(CONFIG_DORADO_V22)
#define BOOTARGS_COMMON "console=ttyS1,57600n8 mem=255M@0x0 mem=256M@0x30000000"
/*#define BOOTARGS_COMMON "console=null, mem=255M@0x0 mem=256M@0x30000000"*/
#else
#define BOOTARGS_COMMON "console=ttyS1,57600n8 mem=255M@0x0 mem=768M@0x30000000"
#endif
#else
#ifdef CONFIG_DORADO_V21
#define BOOTARGS_COMMON "console=ttyS1,115200n8 mem=255M@0x0 mem=256M@0x30000000"
#else
#define BOOTARGS_COMMON "console=ttyS1,115200n8 mem=255M@0x0 mem=768M@0x30000000"
#endif
#endif

#ifdef CONFIG_BOOT_ANDROID
  #define CONFIG_BOOTARGS BOOTARGS_COMMON " ip=off root=/dev/ram0 rw rdinit=/init"
#else
  #ifdef CONFIG_SPL_MMC_SUPPORT
/*    #define CONFIG_BOOTARGS BOOTARGS_COMMON " ip=192.168.10.205:192.168.10.1:192.168.10.1:255.255.255.0 nfsroot=192.168.8.3:/home/nfsroot/bliu/buildroot rw" */
/*	#define CONFIG_BOOTARGS BOOTARGS_COMMON " ip=off root=/dev/ram0 rw rdinit=/linuxrc" */
	#define CONFIG_BOOTARGS BOOTARGS_COMMON " rootdelay=2 init=/linuxrc root=/dev/mmcblk0p7 rw"
  #else
    /*#define CONFIG_BOOTARGS BOOTARGS_COMMON " root=/dev/ndsystem rw"*/
    #define CONFIG_BOOTARGS BOOTARGS_COMMON " root=/dev/ram0 rw rdinit=/linuxrc"
  #endif
#endif

/**
 * Boot command definitions.
 */
#ifdef CONFIG_DORADO_V21
#define CONFIG_BOOTDELAY 1
#else
#define CONFIG_BOOTDELAY 1
#endif

#ifdef CONFIG_BOOT_ANDROID
  #ifdef CONFIG_SPL_MMC_SUPPORT
    #define CONFIG_BOOTCOMMAND	\
	  "batterydet; cls; boota mmc 0 0x80f00000 6144"
    #define CONFIG_NORMAL_BOOT CONFIG_BOOTCOMMAND
    #define CONFIG_RECOVERY_BOOT "boota mmc 0 0x80f00000 24576"
  #else
    /*#define CONFIG_BOOTCOMMAND "boota nand 0 0x80f00000 6144"*/
		#define CONFIG_BOOTCOMMAND  "nand_zm read ndboot 0 0x400000 0x80f00000;boota mem 0x80f00000"
    #define CONFIG_NORMAL_BOOT CONFIG_BOOTCOMMAND
    #define CONFIG_RECOVERY_BOOT "boota nand 0 0x80f00000 24576"
  #endif
#else  /* CONFIG_BOOT_ANDROID */
  #ifdef CONFIG_SPL_MMC_SUPPORT
/*    #define CONFIG_BOOTCOMMAND "tftpboot 0x80600000 bliu/85/uImage.new; bootm" */
	#define CONFIG_BOOTCOMMAND "mmc read 0x80600000 0x1800 0x3000; bootm 0x80600000"
  #else
	#ifdef CONFIG_JZ_NAND_MGR
		#define CONFIG_BOOTCOMMAND  "nand_zm read ndboot 0 0x600000 0x80600000;bootm"
                                                            /*order ops pt offset len dst */
		/*#define CONFIG_BOOTCOMMAND        "nand_zm read ndboot;bootm"*/
	#else
    #define CONFIG_BOOTCOMMAND						\
	"mtdparts default; ubi part system; ubifsmount ubi:boot; "	\
	"ubifsload 0x80f00000 vmlinux.ub; bootm 0x80f00000"
	#endif /* endif CONFIG_JZ_NAND_MGR*/
  #endif
#endif /* CONFIG_BOOT_ANDROID */

/**
 * Drivers configuration.
 */

/* LCD */
#ifndef CONFIG_RVMS
#define CONFIG_LCD
#endif

#ifdef CONFIG_LCD
#define CONFIG_LCD_FORMAT_X8B8G8R8
#ifdef CONFIG_DORADO_V21
#define CONFIG_JZ_MIPI_DSI
#endif
#define LCD_BPP				5
#define CONFIG_GPIO_LCD_PWM	 	GPIO_PE(1)

#define CONFIG_LCD_LOGO
#define CONFIG_RLE_LCD_LOGO
/*#define CONFIG_LCD_INFO_BELOW_LOGO      *//*display the console info on lcd panel for debugg */
#define CONFIG_SYS_WHITE_ON_BLACK
#define CONFIG_SYS_PWM_PERIOD		10000 /* Pwm period in ns */
#define CONFIG_SYS_PWM_CHN		1  /* Pwm channel ok*/
#define CONFIG_SYS_PWM_FULL		256
#define CONFIG_SYS_BACKLIGHT_LEVEL	80 /* Backlight brightness is (80 / 256) */
#define CONFIG_JZ_LCD_V12
#define CONFIG_JZ_PWM

#ifdef CONFIG_JZ_MIPI_DSI
#define CONFIG_VIDEO_BYD_9177AA
/* #define CONFIG_VIDEO_LH155 */
#define CONFIG_DATALANE_BPS_MHZ		320
#else
#define CONFIG_LCD_GPIO_FUNC0_24BIT
/*#define CONFIG_LCD_GPIO_FUNC2_SLCD*/
#define CONFIG_VIDEO_BM347WV_F_8991FTGF
/*#define CONFIG_VIDEO_TRULY_TFT240240_2_E*/
#endif

#ifdef CONFIG_RLE_LCD_LOGO
#define CONFIG_CMD_BATTERYDET   	/* detect battery and show charge logo */
#define CONFIG_CMD_LOGO_RLE	/*display the logo using rle command*/
#endif

#endif /* CONFIG_LCD */

/* SPI */
#if defined(CONFIG_SPL_SPI_SUPPORT)
#ifdef CONFIG_SOFT_SPI
#define SPI_DELAY
#define SPI_SDA(val)                    gpio_direction_output(GPIO_PE(21), val)
#define SPI_SCL(val)                    gpio_direction_output(GPIO_PE(28), val)
#define SPI_READ                        gpio_get_value(GPIO_PE(20))
#define CONFIG_SF_DEFAULT_CS            1
#else
#define CONFIG_JZ_SSI0_PA		/* gpio */
#define CONFIG_SSI_BASE			SSI0_BASE
#endif
#define CONFIG_JZ_SPI			/* compile drivers/spi/jz_spi.c */
#define CONFIG_CMD_SF			/* compile commom/cmd_sf.c */
#define CONFIG_SPI_FLASH		/* compile drivers/mtd/spi/spi_flash.c */
#define CONFIG_SPI_FLASH_INGENIC
#define CONFIG_UBOOT_OFFSET             (26  * 1024)

#define CONFIG_SPL_SERIAL_SUPPORT
#define CONFIG_SPI_SPL_CHECK
#define CONFIG_SYS_SPI_BOOT_FREQ	1000000

#define CONFIG_SYS_MONITOR_LEN		(230 * 1024)
#define CONFIG_ENV_SIZE                 (256 * 1024)
#define CONFIG_ENV_OFFSET		(256 * 1024)
#define CONFIG_ENV_IS_IN_SPI_FLASH
#define CONFIG_ENV_SECT_SIZE		(4 * 1024)
#endif
/* END SPI */

#ifdef CONFIG_COMMAND_SPI
#define CONFIG_CMD_SF			/* compile commom/cmd_sf.c */
#define CONFIG_CMD_SPI_BURNER		/* compile commom/cmd_spi_burner.c */
#define CONFIG_SPI_FLASH		/* compile drivers/mtd/spi/spi_flash.c */
#define CONFIG_JZ_SPI			/* compile drivers/spi/jz_spi.c */
#define CONFIG_SPI_WRITE_CHECK		/* nand write check */
#define CONFIG_JZ_SSI1_PE		/* gpio */
#define CONFIG_SSI_BASE			SSI1_BASE
#define CONFIG_SPI_FLASH_INGENIC
#define CONFIG_SPI_FLASH_INGENIC_NAND
#endif

/* MMC */
#define CONFIG_GENERIC_MMC		1
#define CONFIG_MMC			1
#define CONFIG_JZ_MMC 1

#ifdef CONFIG_JZ_MMC_MSC0
#define CONFIG_JZ_MMC_SPLMSC 0
#define CONFIG_JZ_MMC_MSC0_PA_8BIT 1
#endif
#ifdef CONFIG_JZ_MMC_MSC1
#define CONFIG_JZ_MMC_SPLMSC 1
#define CONFIG_JZ_MMC_MSC1_PE 1
#endif

/* I2C */
#define CONFIG_INGENIC_SOFT_I2C

#ifdef CONFIG_DORADO_V20
#define CONFIG_RICOH61X_I2C_SCL	GPIO_PE(31)
#define CONFIG_RICOH61X_I2C_SDA	GPIO_PE(30)
#define CONFIG_SOFT_I2C_READ_REPEATED_START
/*#define CONFIG_SPL_CORE_VOLTAGE*/
/*#define CONFIG_SPL_MEM_VOLTAGE*/
#define CONFIG_PMU_RICOH6x
#endif

#ifdef CONFIG_DORADO_V21
#define CONFIG_RICOH61X_I2C_SCL	GPIO_PA(13)
#define CONFIG_RICOH61X_I2C_SDA	GPIO_PA(12)
#define CONFIG_SOFT_I2C_READ_REPEATED_START
#define CONFIG_PMU_RICOH6x
#endif

#ifdef CONFIG_DORADO_V22
#define CONFIG_RICOH61X_I2C_SCL	GPIO_PE(31)
#define CONFIG_RICOH61X_I2C_SDA	GPIO_PE(30)
#define CONFIG_SOFT_I2C_READ_REPEATED_START
#define CONFIG_PMU_RICOH6x
#define CONFIG_JZ_PCA953X
#endif

#ifdef CONFIG_DORADO_V30
#define CONFIG_RICOH61X_I2C_SCL	GPIO_PE(31)
#define CONFIG_RICOH61X_I2C_SDA	GPIO_PE(30)
#define CONFIG_SOFT_I2C_READ_REPEATED_START
#define CONFIG_PMU_RICOH6x
#endif

/* PMU */
#define CONFIG_REGULATOR

/* DEBUG ETHERNET */
/*
#define CONFIG_SERVERIP		192.168.8.3
#define CONFIG_IPADDR		192.168.10.206
#define CONFIG_GATEWAYIP        192.168.10.1
#define CONFIG_NETMASK          255.255.255.0
#define CONFIG_ETHADDR          00:11:22:33:44:55
*/
/* GPIO */
#define CONFIG_JZ_GPIO

/**
 * Command configuration.
 */
#define CONFIG_CMD_BOOTD	/* bootd			*/
#define CONFIG_CMD_CONSOLE	/* coninfo			*/
#define CONFIG_CMD_DHCP 	/* DHCP support			*/
#define CONFIG_CMD_ECHO		/* echo arguments		*/
#define CONFIG_CMD_EXT4 	/* ext4 support			*/
#define CONFIG_CMD_FAT		/* FAT support			*/
#define CONFIG_CMD_LOADB	/* loadb			*/
#define CONFIG_CMD_LOADS	/* loads			*/
#define CONFIG_CMD_MEMORY	/* md mm nm mw cp cmp crc base loop mtest */
#define CONFIG_CMD_MISC		/* Misc functions like sleep etc*/
#define CONFIG_CMD_MMC		/* MMC/SD support			*/
#define CONFIG_CMD_NET		/* networking support			*/
#define CONFIG_CMD_PING
#define CONFIG_CMD_RUN		/* run command in env variable	*/
#define CONFIG_CMD_SETGETDCR	/* DCR support on 4xx		*/
#define CONFIG_CMD_SOURCE	/* "source" command support	*/
#define CONFIG_CMD_GETTIME
#define CONFIG_CMD_EEPROM
#define CONFIG_CMD_SAVEENV	/* saveenv			*/
/*#define CONFIG_CMD_I2C*/

/*eeprom*/
#ifdef CONFIG_CMD_EEPROM
#define CONFIG_SYS_I2C_EEPROM_ADDR  0x50
/*#define CONFIG_ENV_EEPROM_IS_ON_I2C*/
#define CONFIG_SYS_I2C_EEPROM_ADDR_LEN	1
#endif

#define PARTITION_NUM 10

/* USB */
#define CONFIG_CMD_FASTBOOT
#define CONFIG_USB_GADGET
#define CONFIG_USB_GADGET_DUALSPEED
#define CONFIG_USB_JZ_DWC2_UDC_V1_1
#define CONFIG_FASTBOOT_GADGET
#define CONFIG_FASTBOOT_FUNCTION
#define CONFIG_G_FASTBOOT_VENDOR_NUM	(0x18d1)
#define CONFIG_G_FASTBOOT_PRODUCT_NUM	(0xdddd)
#define CONFIG_USB_GADGET_VBUS_DRAW 500

/**
 * Serial download configuration
 */
#define CONFIG_LOADS_ECHO	1	/* echo on for serial download */

/**
 * Miscellaneous configurable options
 */
#define CONFIG_DOS_PARTITION

#define CONFIG_LZO
#define CONFIG_RBTREE

#define CONFIG_SKIP_LOWLEVEL_INIT
#define CONFIG_BOARD_EARLY_INIT_F
#define CONFIG_SYS_NO_FLASH
#define CONFIG_SYS_FLASH_BASE	0 /* init flash_base as 0 */
#define CONFIG_ENV_OVERWRITE
#define CONFIG_MISC_INIT_R 1

#define CONFIG_BOOTP_MASK	(CONFIG_BOOTP_DEFAUL)

#define CONFIG_SYS_MAXARGS 16
#define CONFIG_SYS_LONGHELP
#define CONFIG_SYS_PROMPT CONFIG_SYS_BOARD "# "
#define CONFIG_SYS_CBSIZE 1024 /* Console I/O Buffer Size */
#define CONFIG_SYS_PBSIZE (CONFIG_SYS_CBSIZE + sizeof(CONFIG_SYS_PROMPT) + 16)

#define CONFIG_SYS_MONITOR_LEN		(768 * 1024)
#define CONFIG_SYS_MALLOC_LEN		(64 * 1024 * 1024)
#define CONFIG_SYS_BOOTPARAMS_LEN	(128 * 1024)

#define CONFIG_SYS_SDRAM_BASE		0x80000000 /* cached (KSEG0) address */
#define CONFIG_SYS_SDRAM_MAX_TOP	0x90000000 /* don't run into IO space */
#define CONFIG_SYS_INIT_SP_OFFSET	0x400000
#define CONFIG_SYS_LOAD_ADDR		0x88000000
#define CONFIG_SYS_MEMTEST_START	0x80000000
#define CONFIG_SYS_MEMTEST_END		0x88000000

#define CONFIG_SYS_TEXT_BASE		0x80100000
#define CONFIG_SYS_MONITOR_BASE		CONFIG_SYS_TEXT_BASE

/**
 * Environment
 */
#ifdef CONFIG_ENV_IS_IN_MMC
#define CONFIG_SYS_MMC_ENV_DEV		0
#define CONFIG_ENV_SIZE			(32 << 10)
#define CONFIG_ENV_OFFSET		(CONFIG_SYS_MONITOR_LEN + CONFIG_SYS_MMCSD_RAW_MODE_U_BOOT_SECTOR * 512)
#endif /* endif CONFIG_ENV_IS_IN_MMC */

#ifdef CONFIG_JZ_NAND_MGR

/* environment  */
#define CONFIG_ENV_IS_IN_NAND_ZM
#define CMDLINE_PARTITION	"ndcmdline"
#define CONFIG_ENV_SIZE			(32 << 10)
#define CONFIG_ENV_OFFSET		0


#define CONFIG_SPL_NAND_BASE
#define CONFIG_SPL_NAND_DRIVERS
#define CONFIG_SPL_NAND_SIMPLE
#define CONFIG_SPL_NAND_LOAD


/* nand gpio init */
#define CONFIG_NAND_LOADER
#define CFG_NAND_BW8    1
#define CONFIG_NAND_CS  1

#define CONFIG_SPL_SERIAL_SUPPORT

#define CONFIG_CMD_ZM_NAND  /* nand zone manager support */

#endif /* endif CONFIG_JZ_NAND_MGR */

/**
 * SPL configuration
 */
#define CONFIG_SPL_FRAMEWORK

#define CONFIG_SPL_NO_CPU_SUPPORT_CODE
#define CONFIG_SPL_START_S_PATH		"$(CPUDIR)/$(SOC)"
#ifdef CONFIG_SPL_NOR_SUPPORT
#define CONFIG_SPL_LDSCRIPT		"$(CPUDIR)/$(SOC)/u-boot-nor-spl.lds"
#else
#define CONFIG_SPL_LDSCRIPT		"$(CPUDIR)/$(SOC)/u-boot-spl.lds"
#endif
#ifdef CONFIG_JZ_NAND_MGR
#define CONFIG_SPL_PAD_TO		32768
#else
#define CONFIG_SPL_PAD_TO		26624 /* equal to spl max size in M200 */
#endif /* endif CONFIG_JZ_NAND_MGR*/


#define CONFIG_SYS_MMCSD_RAW_MODE_U_BOOT_SECTOR	86//0x5A //wli changed 0x20 /* 16KB offset */
#define CONFIG_SYS_U_BOOT_MAX_SIZE_SECTORS	0x400 /* 512 KB */
#define CONFIG_SYS_NAND_U_BOOT_OFFS	(CONFIG_SYS_NAND_BLOCK_SIZE * 4)
#define CONFIG_SYS_NAND_U_BOOT_DST	CONFIG_SYS_TEXT_BASE
#define CONFIG_SYS_NAND_U_BOOT_START	CONFIG_SYS_NAND_U_BOOT_DST
#define CONFIG_SYS_NAND_U_BOOT_SIZE	(512 * 1024)

#define CONFIG_SPL_BOARD_INIT
#define CONFIG_SPL_LIBGENERIC_SUPPORT
#define CONFIG_SPL_GPIO_SUPPORT
#define CONFIG_SPL_I2C_SUPPORT
#define CONFIG_SPL_REGULATOR_SUPPORT
#define CONFIG_SPL_CORE_VOLTAGE		1100

#ifdef CONFIG_SPL_NOR_SUPPORT
#define CONFIG_SPL_TEXT_BASE		0xba000000
#else
#define CONFIG_SPL_TEXT_BASE		0x80001000
#endif	/*CONFIG_SPL_NOR_SUPPORT*/
#define CONFIG_SPL_MAX_SIZE		(26 * 1024)

#ifdef CONFIG_SPL_MMC_SUPPORT
#define CONFIG_SPL_SERIAL_SUPPORT

#endif /* CONFIG_SPL_MMC_SUPPORT */

#ifdef CONFIG_SPL_NAND_SUPPORT


/* the NAND SPL is small enough to enable serial */
#define CONFIG_SPL_SERIAL_SUPPORT
#define CONFIG_SPL_LIBCOMMON_SUPPORT

#endif /* CONFIG_SPL_NAND_SUPPORT */

#ifdef CONFIG_SPL_NOR_SUPPORT
#define CONFIG_SPL_SERIAL_SUPPORT
#define CONFIG_SYS_UBOOT_BASE		(CONFIG_SPL_TEXT_BASE + CONFIG_SPL_PAD_TO - 0x40)	//0x40 = sizeof (image_header)
#define CONFIG_SYS_OS_BASE		0
#define CONFIG_SYS_SPL_ARGS_ADDR	0
#define CONFIG_SYS_FDT_BASE		0
#endif

/**
 * GPT configuration
 */
#ifdef CONFIG_GPT_CREATOR
#define CONFIG_GPT_TABLE_PATH	"$(TOPDIR)/board/$(BOARDDIR)"
#else
/* USE MBR + zero-GPT-table instead if no gpt table defined*/
#define CONFIG_MBR_P0_OFF	64mb
#define CONFIG_MBR_P0_END	556mb
#define CONFIG_MBR_P0_TYPE 	linux

#define CONFIG_MBR_P1_OFF	580mb
#define CONFIG_MBR_P1_END 	1604mb
#define CONFIG_MBR_P1_TYPE 	linux

#define CONFIG_MBR_P2_OFF	28mb
#define CONFIG_MBR_P2_END	58mb
#define CONFIG_MBR_P2_TYPE 	linux

#define CONFIG_MBR_P3_OFF	1609mb
#define CONFIG_MBR_P3_END	7800mb
#define CONFIG_MBR_P3_TYPE 	fat
#endif

/**
 * Keys.
 */
#define CONFIG_GPIO_USB_DETECT		GPIO_PA(28)
#define CONFIG_GPIO_USB_DETECT_ENLEVEL	1

/* Wrong keys. */
#define CONFIG_GPIO_RECOVERY		GPIO_PA(30)	/* SW7 */
#define CONFIG_GPIO_RECOVERY_ENLEVEL	0

/* Wrong keys. */
#define CONFIG_GPIO_FASTBOOT		GPIO_PA(30)	/* SW2 */
#define CONFIG_GPIO_FASTBOOT_ENLEVEL	0

/*
#define CONFIG_GPIO_MENU		CONFIG_GPIO_FASTBOOT
#define CONFIG_GPIO_MENU_ENLEVEL	CONFIG_GPIO_FASTBOOT_ENLEVEL
*/

/*#define CONFIG_GPIO_VOL_SUB		GPIO_PD(17)*/	/* SW9 */
/*#define CONFIG_GPIO_VOL_SUB_ENLEVEL	1

#define CONFIG_GPIO_VOL_ADD		GPIO_PD(18)*/	/* SW8 */
/*#define CONFIG_GPIO_VOL_ADD_ENLEVEL	1

#define CONFIG_GPIO_BACK		GPIO_PD(19)	*//* SW7 */
/*#define CONFIG_GPIO_BACK_ENLEVEL	0*/

#define CONFIG_GPIO_PWR_WAKE		GPIO_PA(30)
#define CONFIG_GPIO_PWR_WAKE_ENLEVEL	0

/*#define CONFIG_GPIO_DC_DETECT           GPIO_PB(1)
#define CONFIG_GPIO_DC_DETECT_ENLEVEL   0
*/
/* TEST
#define CONFIG_GPIO_DC_DETECT           GPIO_PG(10)
#define CONFIG_GPIO_DC_DETECT_ENLEVEL   1

#define CONFIG_GPIO_CHARGE_DETECT               GPIO_PG(12)
#define CONFIG_GPIO_CHARGE_DETECT_ENLEVEL       0
*/

/* EXT GPIO */
#ifdef CONFIG_JZ_PCA953X
#define CONFIG_SYS_I2C_PCA953X_ADDR     (0x74)
#define CONFIG_CMD_PCA953X
#define CONFIG_CMD_PCA953X_INFO
#define CONFIG_SYS_I2C_PCA953X_WIDTH    {{0x74, 16}}
#define CONFIG_PCA953X_GPIO_BASE        (GPIO_NR_PORTS * 32)
#define CONFIG_PCA953X_I2C_SCL  GPIO_PA(13)
#define CONFIG_PCA953X_I2C_SDA  GPIO_PA(12)
#endif

/* ddr DEBUG */
/*#define CONFIG_DDR_PARAM_DEBUG*/


#endif /* __CONFIG_DORAOD_H__ */
