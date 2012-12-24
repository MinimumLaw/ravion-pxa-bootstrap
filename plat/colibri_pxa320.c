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

#ifdef	CONFIG_BLOCK

#define	NAND_BUS_WIDTH	8
#define	SKIP_TIHM

#else

#include <common.h>
#include <ddr.h>
#include <nand.h>
#include <uart.h>

#define	GPIO_BASE	0x40e10000

inline void plat_init(void)
{
	/* UART1 GPIO setup */
	writel(GPIO_BASE, 0x54c, 0x801);
	writel(GPIO_BASE, 0x550, 0x801);
	writel(GPIO_BASE, 0x600, 0x801);
	writel(GPIO_BASE, 0x604, 0x801);
	writel(GPIO_BASE, 0x608, 0x801);
	writel(GPIO_BASE, 0x60c, 0x801);
	writel(GPIO_BASE, 0x610, 0x801);
	writel(GPIO_BASE, 0x614, 0x801);

	/* CPU speed settings */
        writel(0x40F40000, 0x00, 0x00002000); /* Clear RDH, set MTS to XN=2 in ASCR */
        while (((readl(0x40F40000, 0x00) >> 8) & 0x3) != 0x2); /* wait MTS_S bits */
        writel(0x41340000, 0x00, 0x028fb21f); /* set freq and turbo mult */
        asm volatile("mcr p14, 0, %0, c6, c0, 0"::"r"(3)); /* change freq + turbo on*/
        while (((readl(0x41340000, 0x04) >> 28) & 0x3) != 0x3); /* wait PLL for ready*/
}

void plat_uart_setup(struct uart_config *uart)
{
	uart->enabled = UART1_EN;
}

void plat_nand_setup(struct nand_layout *nand)
{
	nand->copy_start= 0x20000;
	nand->copy_size	= 0x80000;
	nand->copy_dest	= 0xa7800000; /* move u-boot top of SDRAM */
}

void plat_ddr_setup(struct ddr_config *ddr)
{
	ddr->mdcnfg	= 0x329;
	ddr->rcrng	= 0x2;
	ddr->rei	= 0x50;
	ddr->mdmrs	= 0x33;
	ddr->enable	= 1;
}

#endif	/* CONFIG_BLOCK */
