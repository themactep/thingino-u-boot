#
# (C) Copyright 2002-2007
# Wolfgang Denk, DENX Software Engineering, wd@denx.de.
#
# See file CREDITS for list of people who contributed to this
# project.
#
# SPDX-License-Identifier:	GPL-2.0+
include $(OBJTREE)/include/autoconf.mk

LIB	= libpostmpc83xx.o

AOBJS-$(CONFIG_HAS_POST)	+=
COBJS-$(CONFIG_HAS_POST)	+= ecc.o

include $(TOPDIR)/post/rules.mk
