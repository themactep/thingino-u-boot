#
# (C) Copyright 2000-2007
# Wolfgang Denk, DENX Software Engineering, wd@denx.de.
#
# SPDX-License-Identifier:	GPL-2.0+
#

include $(TOPDIR)/config.mk

LIB	:= $(obj)libvideo.o

COBJS-$(CONFIG_ATI_RADEON_FB) += ati_radeon_fb.o videomodes.o
COBJS-$(CONFIG_ATMEL_HLCD) += atmel_hlcdfb.o
COBJS-$(CONFIG_ATMEL_LCD) += atmel_lcdfb.o
COBJS-$(CONFIG_CFB_CONSOLE) += cfb_console.o
COBJS-$(CONFIG_EXYNOS_DP) += exynos_dp.o exynos_dp_lowlevel.o
COBJS-$(CONFIG_EXYNOS_FB) += exynos_fb.o exynos_fimd.o
COBJS-$(CONFIG_EXYNOS_MIPI_DSIM) += exynos_mipi_dsi.o exynos_mipi_dsi_common.o \
				exynos_mipi_dsi_lowlevel.o
COBJS-$(CONFIG_EXYNOS_PWM_BL) += exynos_pwm_bl.o
COBJS-$(CONFIG_FSL_DIU_FB) += fsl_diu_fb.o videomodes.o
COBJS-$(CONFIG_MPC8XX_LCD) += mpc8xx_lcd.o
COBJS-$(CONFIG_PXA_LCD) += pxa_lcd.o
COBJS-$(CONFIG_S6E8AX0) += s6e8ax0.o
COBJS-$(CONFIG_S6E63D6) += s6e63d6.o
COBJS-$(CONFIG_LD9040) += ld9040.o
COBJS-$(CONFIG_SED156X) += sed156x.o
COBJS-$(CONFIG_VIDEO_BCM2835) += bcm2835.o
COBJS-$(CONFIG_VIDEO_COREBOOT) += coreboot_fb.o
COBJS-$(CONFIG_VIDEO_CT69000) += ct69000.o videomodes.o
COBJS-$(CONFIG_VIDEO_DA8XX) += da8xx-fb.o videomodes.o
COBJS-$(CONFIG_VIDEO_MB862xx) += mb862xx.o videomodes.o
COBJS-$(CONFIG_VIDEO_MB86R0xGDC) += mb86r0xgdc.o videomodes.o
COBJS-$(CONFIG_VIDEO_MX3) += mx3fb.o videomodes.o
COBJS-$(CONFIG_VIDEO_IPUV3) += mxc_ipuv3_fb.o ipu_common.o ipu_disp.o
COBJS-$(CONFIG_VIDEO_MXS) += mxsfb.o videomodes.o
COBJS-$(CONFIG_VIDEO_OMAP3) += omap3_dss.o
COBJS-$(CONFIG_VIDEO_SED13806) += sed13806.o
COBJS-$(CONFIG_VIDEO_SM501) += sm501.o
COBJS-$(CONFIG_VIDEO_SMI_LYNXEM) += smiLynxEM.o videomodes.o
COBJS-$(CONFIG_VIDEO_TEGRA) += tegra.o
COBJS-$(CONFIG_VIDEO_VCXK) += bus_vcxk.o
COBJS-$(CONFIG_VIDEO_BYD_BM8766U) += jz_lcd/lcd_panel/byd_bm8766u.o
COBJS-$(CONFIG_VIDEO_BM347WV_F_8991FTGF) += jz_lcd/lcd_panel/byd_8991.o \
	jz_lcd/lcd_panel/byd_8991_init.o
COBJS-$(CONFIG_VIDEO_ILI_6122) += jz_lcd/lcd_panel/ili_6122.o

COBJS-$(CONFIG_VIDEO_X163) += jz_lcd/lcd_panel/auo_x163.o \
				jz_lcd/lcd_panel/write_command_auo_x163.o

COBJS-$(CONFIG_VIDEO_BYD_9177AA) += jz_lcd/lcd_panel/byd_9177aa.o
COBJS-$(CONFIG_VIDEO_TRULY_TFT240240_2_E) += jz_lcd/lcd_panel/truly_tft240240_2_e.o
COBJS-$(CONFIG_VIDEO_LH155) += jz_lcd/lcd_panel/lh155.o
COBJS-$(CONFIG_JZ_MIPI_DSI) += jz_lcd/jz_mipi_dsi/jz_mipi_dsi.o jz_lcd/jz_mipi_dsi/jz_mipi_dsi_lowlevel.o jz_lcd/jz_mipi_dsi/jz_mipi_dsih_hal.o \
				jz_lcd/jz_mipi_dsi/write_command_to_mipi.o
COBJS-$(CONFIG_VIDEO_JZ4775) += jz_lcd/backlight_pwm.o jz_lcd/jz4775_lcd.o
COBJS-$(CONFIG_VIDEO_JZ4780) += jz_lcd/backlight_pwm.o jz_lcd/jz4780_lcd.o
COBJS-$(CONFIG_JZ_LCD_V12) += jz_lcd/backlight_pwm.o jz_lcd/jz_lcd_v1_2.o

COBJS	:= $(sort $(COBJS-y))
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
