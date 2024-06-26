#
# (C) Copyright 2006
# Wolfgang Denk, DENX Software Engineering, wd@denx.de.
#
# SPDX-License-Identifier:	GPL-2.0+
#

include $(TOPDIR)/config.mk

LIB	:= $(obj)libnand.o

ifdef CONFIG_CMD_NAND

ifdef CONFIG_SPL_BUILD

ifdef CONFIG_SPL_NAND_DRIVERS
NORMAL_DRIVERS=y
endif

COBJS-$(CONFIG_SPL_NAND_AM33XX_BCH) += am335x_spl_bch.o
COBJS-$(CONFIG_SPL_NAND_DOCG4) += docg4_spl.o
COBJS-$(CONFIG_SPL_NAND_SIMPLE) += nand_spl_simple.o
COBJS-$(CONFIG_SPL_NAND_LOAD) += nand_spl_load.o
COBJS-$(CONFIG_SPL_NAND_ECC) += nand_ecc.o
COBJS-$(CONFIG_SPL_NAND_BASE) += nand_base.o

else # not spl

NORMAL_DRIVERS=y

COBJS-y += nand.o
COBJS-y += nand_bbt.o
COBJS-y += nand_ids.o
COBJS-y += nand_util.o
COBJS-y += nand_ecc.o
COBJS-y += nand_base.o

endif # not spl

ifdef NORMAL_DRIVERS

COBJS-$(CONFIG_NAND_ECC_BCH) += nand_bch.o

COBJS-$(CONFIG_NAND_ATMEL) += atmel_nand.o
COBJS-$(CONFIG_DRIVER_NAND_BFIN) += bfin_nand.o
COBJS-$(CONFIG_NAND_DAVINCI) += davinci_nand.o
COBJS-$(CONFIG_NAND_FSL_ELBC) += fsl_elbc_nand.o
COBJS-$(CONFIG_NAND_FSL_IFC) += fsl_ifc_nand.o
COBJS-$(CONFIG_NAND_FSL_UPM) += fsl_upm.o
COBJS-$(CONFIG_NAND_FSMC) += fsmc_nand.o
COBJS-$(CONFIG_NAND_JZ4740) += jz4740_nand.o
COBJS-$(CONFIG_NAND_KB9202) += kb9202_nand.o
COBJS-$(CONFIG_NAND_KIRKWOOD) += kirkwood_nand.o
COBJS-$(CONFIG_NAND_KMETER1) += kmeter1_nand.o
COBJS-$(CONFIG_NAND_MPC5121_NFC) += mpc5121_nfc.o
COBJS-$(CONFIG_NAND_MXC) += mxc_nand.o
COBJS-$(CONFIG_NAND_MXS) += mxs_nand.o
COBJS-$(CONFIG_NAND_NDFC) += ndfc.o
COBJS-$(CONFIG_NAND_NOMADIK) += nomadik.o
COBJS-$(CONFIG_NAND_S3C2410) += s3c2410_nand.o
COBJS-$(CONFIG_NAND_SPEAR) += spr_nand.o
COBJS-$(CONFIG_TEGRA_NAND) += tegra_nand.o
COBJS-$(CONFIG_NAND_OMAP_GPMC) += omap_gpmc.o
COBJS-$(CONFIG_NAND_PLAT) += nand_plat.o
COBJS-$(CONFIG_NAND_DOCG4) += docg4.o
COBJS-$(CONFIG_JZ_SPI_NANDFLASH) += jz_spi_nand.o
COBJS-$(CONFIG_CMD_SFC_NAND) += jz_sfc_nand.o jz_sfc.o


else  # minimal SPL drivers

COBJS-$(CONFIG_NAND_FSL_ELBC) += fsl_elbc_spl.o
COBJS-$(CONFIG_NAND_FSL_IFC) += fsl_ifc_spl.o
COBJS-$(CONFIG_NAND_MXC) += mxc_nand_spl.o

endif # drivers
endif # nand

COBJS	:= $(COBJS-y)
SRCS	:= $(COBJS:.o=.c)
OBJS	:= $(addprefix $(obj),$(COBJS))

all:	$(LIB)

$(LIB):	$(obj).depend $(OBJS)
	$(call cmd_link_o_target, $(OBJS))

#########################################################################

# defines $(obj).depend target
include $(SRCTREE)/rules.mk

sinclude $(obj).depend

#########################################################################
