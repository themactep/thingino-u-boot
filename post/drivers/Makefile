#
# (C) Copyright 2002-2006
# Wolfgang Denk, DENX Software Engineering, wd@denx.de.
#
# See file CREDITS for list of people who contributed to this
# project.
#
# SPDX-License-Identifier:	GPL-2.0+
include $(TOPDIR)/config.mk

LIB	= libpostdrivers.o

COBJS-$(CONFIG_HAS_POST)	+= flash.o i2c.o memory.o rtc.o

include $(TOPDIR)/post/rules.mk
