#
# (C) Copyright 2000-2007
# Wolfgang Denk, DENX Software Engineering, wd@denx.de.
#
# SPDX-License-Identifier:	GPL-2.0+
#

include $(TOPDIR)/config.mk

LIB	:= $(obj)libi2c.o

COBJS-$(CONFIG_BFIN_TWI_I2C) += bfin-twi_i2c.o
COBJS-$(CONFIG_DRIVER_DAVINCI_I2C) += davinci_i2c.o
COBJS-$(CONFIG_DW_I2C) += designware_i2c.o
COBJS-$(CONFIG_FSL_I2C) += fsl_i2c.o
COBJS-$(CONFIG_I2C_MVTWSI) += mvtwsi.o
COBJS-$(CONFIG_I2C_MV) += mv_i2c.o
COBJS-$(CONFIG_I2C_MXC) += mxc_i2c.o
COBJS-$(CONFIG_I2C_MXS) += mxs_i2c.o
COBJS-$(CONFIG_DRIVER_OMAP1510_I2C) += omap1510_i2c.o
COBJS-$(CONFIG_DRIVER_OMAP24XX_I2C) += omap24xx_i2c.o
COBJS-$(CONFIG_DRIVER_OMAP34XX_I2C) += omap24xx_i2c.o
COBJS-$(CONFIG_PCA9564_I2C) += pca9564_i2c.o
COBJS-$(CONFIG_PPC4XX_I2C) += ppc4xx_i2c.o
COBJS-$(CONFIG_DRIVER_S3C24X0_I2C) += s3c24x0_i2c.o
COBJS-$(CONFIG_S3C44B0_I2C) += s3c44b0_i2c.o
COBJS-$(CONFIG_SOFT_I2C) += soft_i2c.o
COBJS-$(CONFIG_INGENIC_SOFT_I2C) += ingenic_soft_i2c.o
COBJS-$(CONFIG_TEGRA_I2C) += tegra_i2c.o
COBJS-$(CONFIG_TSI108_I2C) += tsi108_i2c.o
COBJS-$(CONFIG_U8500_I2C) += u8500_i2c.o
COBJS-$(CONFIG_SH_I2C) += sh_i2c.o
COBJS-$(CONFIG_SH_SH7734_I2C) += sh_sh7734_i2c.o
COBJS-$(CONFIG_ZYNQ_I2C) += zynq_i2c.o

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
