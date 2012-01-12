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
#include <nand.h>
#include <timer.h>

#define	NAND_BASE	0x43100000
#define	NDCR		0x00
#define	NDTR0CS0	0x04
#define	NDTR1CS0	0x0c
#define	NDSR		0x14
#define	NDPCR		0x18
#define	NDBBDR0		0x1c
#define	NDBBDR1		0x20
#define	NDREDEL		0x24
#define	NDDB		0x40
#define	NDCB0		0x48
#define	NDCB1		0x4c
#define	NDCB2		0x50

#define	nand_write_command(a, b, c) \
	{ \
		writel(NAND_BASE, NDCB0, (a)); \
		writel(NAND_BASE, NDCB0, (b)); \
		writel(NAND_BASE, NDCB0, (c)); \
	}

/*
 * Configure the NAND controller
 */
void nand_configure(struct nand_layout *nand)
{
	unsigned long ndcr;
	unsigned long page_bs;

	/* Clear DMA, ECC, RUN, Interrupts */
	ndcr = readl(NAND_BASE, NDCR);
	writel(NAND_BASE, NDCR, ndcr & ~0x70000000);

	nand->page_sz = (ndcr & 0x03000000) ? 0x800 : 0x200;
	page_bs = (ndcr & 0x03000000) ? 11 : 9;

	/* Calculate how many pages we have to copy */
	nand->copy_start >>= page_bs;
	nand->copy_size >>= page_bs;
}

/*
 * Start new command
 */
static void nand_cmd_new(void)
{
	/* Clear status bits */
	writel(NAND_BASE, NDSR, 0xfff);

	/* Start NAND controller */
	writel(NAND_BASE, NDCR, readl(NAND_BASE, NDCR) | 0x10000000);

	/* Wait for WRCMDDREQ */
	while (!(readl(NAND_BASE, NDSR) & 0x001));
}

/*
 * Issues CMD_RESET to the NAND flash
 */
static void nand_cmd_reset(void)
{
	int timeout = 2;

	/* Start new command */
	nand_cmd_new();

	/* Write the CMD_RESET */
	nand_write_command(0x00a000ff, 0x00, 0x00);

	/* Wait for CS0_CMDD */
	while (!(readl(NAND_BASE, NDSR) & 0x100));

	/* Wait for RDY. This may not happen on some devices, hence
	 * the timeout. */
	while (timeout--) {
		if (readl(NAND_BASE, NDSR) & 0x800)
			break;
		mdelay(1);
	}

	/* Stop NAND controller */
	writel(NAND_BASE, NDCR, readl(NAND_BASE, NDCR) & ~0x10000000);
}

#if 0
/*
 * Issues CMD_READID to the NAND flash
 */
static void nand_cmd_readid(unsigned long *data)
{
	/* Start new command */
	nand_cmd_new();

	/* Write the CMD_RESET */
	nand_write_command(0x00600090, 0x00, 0x00);

	/* Wait for RDDREQ */
	while (!(readl(NAND_BASE, NDSR) & 0x002));

	/* Read the Chip ID */
	data[0] = readl(NAND_BASE, NDDB);
	data[1] = readl(NAND_BASE, NDDB);

	/* Stop NAND controller */
	writel(NAND_BASE, NDCR, readl(NAND_BASE, NDCR) & ~0x10000000);
}
#endif

/*
 * Copies one page of data from NAND to destination address
 */
static void nand_cmd_read0(struct nand_layout *nand, unsigned long page)
{
	unsigned long daddr;
	int i;

	/* Start new command */
	nand_cmd_new();

	if (nand->page_sz == 0x800) {
		/* Large pages : Write the CMD_READ0 and READSTART */
		nand_write_command(0x000d3000, page << 16, page >> 16);
	} else {
		/* Small pages : Write the CMD_READ0 */
		nand_write_command(0x00040000, page << 8, 0);
	}

	/* Wait for RDDREQ */
	while (!(readl(NAND_BASE, NDSR) & 0x002));

	daddr = nand->copy_dest + ((page - nand->copy_start) * nand->page_sz);

	/* Copy the page to the destination address */
	for (i = 0; i < nand->page_sz; i += 4)
		writel(daddr, i, readl(NAND_BASE, NDDB));

	/* Read the ECC data and drop them */
	for (i = 0; i < (nand->page_sz >> 7); i += 4)
		readl(NAND_BASE, NDDB);

	/* Stop NAND controller */
	writel(NAND_BASE, NDCR, readl(NAND_BASE, NDCR) & ~0x10000000);
}

/*
 * Copies the bootloader from NAND
 */
void nand_copy(struct nand_layout *nand)
{
	int i;

	/* Reset the NAND */
	nand_cmd_reset();

	/* Copy the data from NAND */
	for (i = nand->copy_start; i < nand->copy_start + nand->copy_size; i++)
		nand_cmd_read0(nand, i);
}
