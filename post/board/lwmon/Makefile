#
# (C) Copyright 2002-2006
# Wolfgang Denk, DENX Software Engineering, wd@denx.de.
#
# See file CREDITS for list of people who contributed to this
# project.
#
# SPDX-License-Identifier:	GPL-2.0+
include $(OBJTREE)/include/autoconf.mk

LIB	= libpostlwmon.o

COBJS-$(CONFIG_HAS_POST)	+= sysmon.o

include $(TOPDIR)/post/rules.mk
