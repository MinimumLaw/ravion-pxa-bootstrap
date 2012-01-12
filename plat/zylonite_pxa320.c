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
#include <uart.h>

#define	GPIO_BASE	0x40e10000

inline void plat_init(void)
{
	/* UART1 GPIO setup */
	writel(GPIO_BASE, 0x438, 0x802);
	writel(GPIO_BASE, 0x43c, 0x802);
	writel(GPIO_BASE, 0x440, 0x802);
	writel(GPIO_BASE, 0x444, 0x802);
	writel(GPIO_BASE, 0x448, 0x802);
	writel(GPIO_BASE, 0x44c, 0x802);
	writel(GPIO_BASE, 0x450, 0x802);
	writel(GPIO_BASE, 0x454, 0x802);
}

void plat_uart_setup(struct uart_config *uart)
{
	uart->enabled = UART3_EN;
}

void plat_nand_setup(struct nand_layout *nand)
{
	nand->copy_start= 0x20000;
	nand->copy_size	= 0x40000;
	nand->copy_dest	= 0x5c00a000;
}

void plat_ddr_setup(struct ddr_config *ddr)
{
	ddr->mdcnfg	= 0x355;
	ddr->rcrng	= 0x2;
	ddr->rei	= 0x2;
	ddr->mdmrs	= 0x33;
	ddr->enable	= 1;
}

#endif	/* CONFIG_BLOCK */
