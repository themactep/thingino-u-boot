#
# (C) Copyright 2010 DENX Software Engineering
# Anatolij Gustschin, agust@denx.de.
#
# See file CREDITS for list of people who contributed to this
# project.
#
# SPDX-License-Identifier:	GPL-2.0+
include $(OBJTREE)/include/autoconf.mk

LIB	= libpostpdm360ng.o

COBJS-$(CONFIG_HAS_POST)	+= coproc_com.o

include $(TOPDIR)/post/rules.mk
