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

#else

#include <common.h>
#include <ddr.h>
#include <nand.h>
#include <cpu.h>
#include <uart.h>

#define	GPIO_BASE	0x40e10000

inline void plat_init(void)
{
	unsigned long accr;

	/* CPU speed settings */
	accr = 0x028fd200;
	if (cpu_type(PXA300))
		accr |= 0x08;
	else
		accr |= 0x18;
	writel(0x41340000, 0x00, accr);
	asm volatile("mcr p14, 0, %0, c6, c0, 0"::"r"(2));
	while (((readl(0x41340000, 0x04) >> 28) & 0x3) != 0x3);

	/* UART1 GPIO setup */
	writel(GPIO_BASE, 0x600, 0x801);
	writel(GPIO_BASE, 0x604, 0x801);
	writel(GPIO_BASE, 0x608, 0x801);
	writel(GPIO_BASE, 0x60c, 0x801);
	writel(GPIO_BASE, 0x610, 0x806);
	writel(GPIO_BASE, 0x614, 0x801);
	writel(GPIO_BASE, 0x618, 0x806);
	writel(GPIO_BASE, 0x61c, 0x801);

	/* DFC setup */
	writel(GPIO_BASE, 0xb4, 0xc2c1);	/* GPIO0 */
	writel(GPIO_BASE, 0xb8, 0x02c1);	/* GPIO1 */
	writel(GPIO_BASE, 0xbc, 0x02c1);	/* GPIO2 */

	writel(GPIO_BASE, 0xc0, 0x8287);	/* nCS1 */
	writel(GPIO_BASE, 0xc4, 0x8287);	/* nCS0 */
	writel(GPIO_BASE, 0xc8, 0xd5c0);	/* INT_RnB */
	writel(GPIO_BASE, 0xcc, 0xd481);	/* nWE */

	writel(GPIO_BASE, 0x200, 0xd481);	/* nRE */
	writel(GPIO_BASE, 0x204, 0xd480);	/* nBE0 */
	writel(GPIO_BASE, 0x208, 0xd480);	/* nBE1 */
	writel(GPIO_BASE, 0x20c, 0xd581);	/* ALE_nWE */
	writel(GPIO_BASE, 0x240, 0xd580);	/* CLE_nOE */
	writel(GPIO_BASE, 0x210, 0xb480);	/* ADDR0 */
	writel(GPIO_BASE, 0x214, 0xb480);	/* ADDR1 */
	writel(GPIO_BASE, 0x218, 0xb480);	/* ADDR2 */
	writel(GPIO_BASE, 0x21c, 0xb480);	/* ADDR3 */
	writel(GPIO_BASE, 0x244, 0xd480);	/* nLUA */
	writel(GPIO_BASE, 0x254, 0xd480);	/* nLLA */
	writel(GPIO_BASE, 0x248, 0xd481);	/* NCS0 */
	writel(GPIO_BASE, 0x278, 0xd481);	/* NCS1 */

	writel(GPIO_BASE, 0x220, 0xb881);	/* IO0 */
	writel(GPIO_BASE, 0x228, 0xb881);	/* IO1 */
	writel(GPIO_BASE, 0x230, 0xb881);	/* IO2 */
	writel(GPIO_BASE, 0x238, 0xb881);	/* IO3 */
	writel(GPIO_BASE, 0x258, 0xb881);	/* IO4 */
	writel(GPIO_BASE, 0x260, 0xb881);	/* IO5 */
	writel(GPIO_BASE, 0x268, 0xb881);	/* IO6 */
	writel(GPIO_BASE, 0x270, 0xb881);	/* IO7 */
	writel(GPIO_BASE, 0x224, 0xb881);	/* IO8 */
	writel(GPIO_BASE, 0x22c, 0xb881);	/* IO9 */
	writel(GPIO_BASE, 0x234, 0xb881);	/* IO10 */
	writel(GPIO_BASE, 0x23c, 0xb881);	/* IO11 */
	writel(GPIO_BASE, 0x25c, 0xb881);	/* IO12 */
	writel(GPIO_BASE, 0x264, 0xb881);	/* IO13 */
	writel(GPIO_BASE, 0x26c, 0xb881);	/* IO14 */
	writel(GPIO_BASE, 0x274, 0xb881);	/* IO15 */
}

void plat_uart_setup(struct uart_config *uart)
{
	uart->enabled = UART1_EN;
}

void plat_nand_setup(struct nand_layout *nand)
{
	nand->copy_start= 0x20000;
	nand->copy_size	= 0x40000;
	nand->copy_dest	= 0x5c00a000;
}

void plat_ddr_setup(struct ddr_config *ddr)
{
	unsigned long pad_xx, i;

	if (cpu_type(PXA300)) {
		ddr->mdcnfg	= 0x32d;
		pad_xx		= 0x181b0505;
	} else {
		ddr->mdcnfg	= 0x34d;
		pad_xx		= 0x19250904;
	}

	ddr->rcrng	= 0x2;
	ddr->rei	= 0xd0;
	ddr->mdmrs	= 0x33;
	ddr->enable	= 1;

	for (i = 0x110; i <= 0x12c; i += 4)
		writel(0x48100000, i, pad_xx);
}

#endif	/* CONFIG_BLOCK */
