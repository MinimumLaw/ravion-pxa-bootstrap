/*
 * Copyright (C) 2010 Marek Vasut <marek.vasut@gmail.com>
 *
 * This file is a part of the OpenPXA project, an Open Source OBM for the
 * Marvell PXA3xx CPUs.
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 * 
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include <common.h>
#include <ddr.h>
#include <nand.h>
#include <plat.h>
#include <uart.h>
#include <dma.h>
#include <cache.h>

#define	RELOC_DEST	0x5c009000

static void relocate(void);
void obm_start(void);

void __attribute__ ((naked, section(".boot"))) start(void)
{
	/*
	 * 1) Use PSPR to store base address of bootloader
	 * 2) Setup temporary stack
	 */
	asm volatile(
		"adr	r0, start\n"
		"ldr	r1, =0x40f50008\n"
		"str	r0, [r1]\n"

		"ldr	sp, =0x5c009000\n"
	);

	obm_start();
}

void obm_start(void)
{
	struct nand_layout nand;
	struct ddr_config ddr;
	struct uart_config uart;

	/* Enable instruction cache and branch trace buffer to speed boot up */
	enable_icache_and_btb();

	/* Platform-specific setup */
	plat_init();

	/* Relocate to RELOC_DEST */
	relocate();

	/* Preconfigure UARTs */
	plat_uart_setup(&uart);

	uart_init(&uart);
	uart_puts(&uart, "\nOpenPXA OBM 2, booting ... ");

	/* Setup DDR DRAM */
	plat_ddr_setup(&ddr);
	ddr_configure(&ddr);

	/* Configure NAND */
	plat_nand_setup(&nand);
	nand_configure(&nand);

	/* Copy the bootloader */
	nand_copy(&nand);

	uart_puts(&uart, "OK\n\n");

	/* Jump to the bootloader */
	asm volatile("ldr	pc, %0;" : "=m"(nand.copy_dest));
}

/*
 * Relocate the IPL Code
 *
 * Memory map:
 * 0x5c008000-0x5c009000	Stack
 * 0x5c009000			Code
 */
static void relocate(void)
{
	int base = *(volatile unsigned int *)0x40f50008;
	int i;

	/* If we already relocated. do nothing */
	if (base == RELOC_DEST)
		return;

	/* Copy the data via DMA */
//	mem2mem_dma(base, RELOC_DEST, 0x800);
	/* Do the relocation */
	for (i = 0; i < 0x800; i+=4)
		writel(RELOC_DEST, i, readl(base, i));

	/* And branch to the new location */
	asm volatile("mov	pc, %0" :: "r"(RELOC_DEST));
}
