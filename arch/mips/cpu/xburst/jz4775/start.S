/*
 * Startup Code for MIPS32 XBURST JZ4775 CPU-core
 *
 * Copyright (c) 2013 Ingenic Semiconductor Co.,Ltd
 *
 * SPDX-License-Identifier:	GPL-2.0+
 */

#include <config.h>
#include <version.h>
#include <asm/regdef.h>
#include <asm/mipsregs.h>
#include <asm/addrspace.h>
#include <asm/cacheops.h>
#include <asm/arch/base.h>

	.set noreorder

	.globl _start
	.section .start_section

_start:
#ifdef CONFIG_SPL_MMC_SUPPORT
	/* magic value ("MSPL") */
	.word 0x4d53504c
#endif

#if defined(CONFIG_SPL_NAND_SUPPORT) || defined(CONFIG_JZ_NAND_MGR)
	/*
	 * NAND parameters are stored with plenty of redundancy in the
	 * first 192 bytes of the first page of the SPL image.
	 */

	.space 256, 0x00
	/* simulation use 8bit common 3-byte row cycles 8k page nand*/
	/*.space 128, 0x55
	.space 96, 0xaa
	.space 32, 0x55*/

#endif /* CONFIG_SPL_NAND_SUPPORT */

#ifdef CONFIG_SPL_NOR_SUPPORT
	.word 0
#endif /* CONFIG_SPL_NOR_SUPPORT */

#ifdef CONFIG_SPL_SPI_SUPPORT
#define SSI_GR_BOOT (CONFIG_SYS_EXTAL / (CONFIG_SYS_SPI_BOOT_FREQ * 2) - 1)
	.word 0x55020304
	.word (0x00aa55aa | (SSI_GR_BOOT << 24))
	.space 8, 0
#endif /* CONFIG_SPL_SPI_SUPPORT */

#if 0
loop:
	la      v0, 0xb0010400
	li      t0, 0x400
	sw      t0, 0x74(v0)
	sw      t0, 0x18(v0)
	sw      t0, 0x24(v0)
	sw      t0, 0x38(v0)
	sw      t0, 0x48(v0)
	li      t1, 0x7fffff
loop2:
	subu    t1, t1, 1
	nop
	bgtz    t1, loop2
	nop
	sw      t0, 0x44(v0)
	li      t1, 0x7fffff
loop3:
	subu    t1, t1, 1
	nop
	bgtz    t1, loop3
	nop
	b       loop
	nop
#endif

	/* Invalidate BTB */
	mfc0	v0, CP0_CONFIG, 7
	nop
	ori	v0, v0, 2
	mtc0	v0, CP0_CONFIG, 7
	nop

	/*
	 * CU0=UM=EXL=IE=0, BEV=ERL=1, IP2~7=1
	 */
	li	t0, 0x0040FC04
	mtc0	t0, CP0_STATUS

	/* CAUSE register */
	/* IV=1, use the specical interrupt vector (0x200) */
	li	t1, 0x00800000
	mtc0	t1, CP0_CAUSE

	/* enable bridge radical mode */
	la	t0, CPM_BASE
	lw	t1, 0x24(t0)
	ori	t1, t1, 0x22
	sw	t1, 0x24(t0)

init_caches:
	li	t0, CONF_CM_CACHABLE_NONCOHERENT
	mtc0	t0, CP0_CONFIG
	nop

	/* enable idx-store-data cache insn */
	li      t0, 0x20000000
	mtc0    t0, CP0_ECC

	li	t1, KSEG0		/* Start address */
#define CACHE_ALLOC_END (CONFIG_SYS_DCACHE_SIZE - CONFIG_SYS_CACHELINE_SIZE)

	ori     t2, t1, CACHE_ALLOC_END	/* End address */
	mtc0	zero, CP0_TAGLO, 0
	mtc0	zero, CP0_TAGLO, 1
cache_clear_a_line:
	cache   INDEX_STORE_TAG_I, 0(t1)
	cache   INDEX_STORE_TAG_D, 0(t1)
	bne     t1, t2, cache_clear_a_line
	addiu   t1, t1, CONFIG_SYS_CACHELINE_SIZE

	li	t1, KSEG0		/* Start address */
	ori     t2, t1, CACHE_ALLOC_END	/* End address */
	la      t3, 0x1ffff000		/* Physical address and 4KB page mask */
cache_alloc_a_line:
	and     t4, t1, t3
	ori     t4, t4, 1		/* V bit of the physical tag */
	mtc0    t4, CP0_TAGLO, 0
	cache   INDEX_STORE_TAG_I, 0(t1)
	cache   INDEX_STORE_TAG_D, 0(t1)
	bne     t1, t2, cache_alloc_a_line
	addiu   t1, t1, CONFIG_SYS_CACHELINE_SIZE

#ifdef CONFIG_SPL_NOR_SUPPORT
	.extern __data_start
	.extern __data_end
	/*Init data section, nor spl*/
	la	t0, __data_start
	la	t1, __data_end
	la	t2, 0xF4000800
1:
	lw	t3, 0(t0)
	addiu	t0, t0, 4
	sw	t3, 0(t2)
	addiu	t2, t2, 4
	bne	t0, t1, 1b
	nop
#endif

	/* Set up stack */
#ifdef CONFIG_SPL_STACK
	li	sp, CONFIG_SPL_STACK
#endif
#ifndef CONFIG_BURNER
	j	board_init_f
	nop
#else
	jal	board_init_f
	nop
	lui     k0 ,0xbfc0
	ori     k0, k0,0x2b94
	li ra, 0xbfc03258
        jr ra
        addiu sp, sp, 8
#endif

	.globl	validate_cache
	.type	validate_cache, @function
validate_cache:
	li	t1, KSEG0		/* Start address */
	ori     t2, t1, CACHE_ALLOC_END	/* End address */
	la      t3, 0x1ffff000		/* Physical address and 4KB page mask */
cache_alloc_a_line2:
	and     t4, t1, t3
	mtc0    t4, CP0_TAGLO, 0
	cache   INDEX_STORE_TAG_I, 0(t1)
	cache   INDEX_STORE_TAG_D, 0(t1)
	bne     t1, t2, cache_alloc_a_line2
	addiu   t1, t1, CONFIG_SYS_CACHELINE_SIZE
	jr	ra
	nop
