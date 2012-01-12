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

#define	DMC_BASE	0x48100000
#define	MDCNFG		0x00
#define	MDREFR		0x04
#define	MDMRS		0x40
#define	DDR_HCAL	0x60
#define	DDR_WCAL	0x68
#define	DMCIER		0x70
#define	DMCISR		0x78
#define	DDR_DLS		0x80
#define	EMPI		0x90
#define	RCOMP		0x100
#define	PAD_MA		0x110
#define	PAD_MDMSB	0x114
#define	PAD_MDLSB	0x118
#define	PAD_SDRAM	0x11c
#define	PAD_SDCLK	0x120
#define	PAD_SDCS	0x124

/*
 * Configure and start the DDR RAM controller
 */
void ddr_configure(struct ddr_config *ddr)
{
	unsigned long mdcnfg = ddr->mdcnfg;

	/* Do not configure DDR DRAM */
	if (!ddr->enable)
		return;

	mdcnfg &= 0x37f;	/* DTC, DRAC, DCAC, DBW, DCSE */
	mdcnfg |= 0x80000400;	/* SETALWAYS bits */

	/* 1.8.1 -- 2. Program DTC, DRAC, DCAC */
	writel(DMC_BASE, MDCNFG, (mdcnfg & ~0x3));
	readl(DMC_BASE, MDCNFG);

	/* 1.8.1 -- 3. Program DCSE */
	writel(DMC_BASE, MDCNFG, mdcnfg);
	readl(DMC_BASE, MDCNFG);

	/* 1.8.1 -- 5a. Enable RCOMP interrupt (we disable it) */
	writel(DMC_BASE, DMCIER, 0);
	readl(DMC_BASE, DMCIER);

	/* 1.8.1 -- 5b. SWEVAL, RCRNG, REI (5c., 5d.) */
	writel(DMC_BASE, RCOMP, 0x81000000 | (ddr->rcrng << 29) | ddr->rei);
	readl(DMC_BASE, RCOMP);

	/* 1.8.1 -- 5e. Wait for RCI, then clear it */
	while (!(readl(DMC_BASE, DMCISR) & 0x80000000));
	writel(DMC_BASE, DMCISR, 0x80000000);

	/* 1.8.1 -- 5f. ??? REVISIT */

	/* 1.8.1 -- 5g. RCOMP UPDATE */
	writel(DMC_BASE, RCOMP, readl(DMC_BASE, RCOMP) | 0x40000000);

	/* 1.8.1 -- 5h. Wait for RCOMP UPDATE cleared */
	while (readl(DMC_BASE, RCOMP) & 0x40000000);

	/* 1.8.1 -- 6b. Clear HCRNG, HCOFF (6c.) */
	writel(DMC_BASE, DDR_HCAL, readl(DMC_BASE, DDR_HCAL) & ~0x3fdf);
	readl(DMC_BASE, DDR_HCAL);

	/* 1.8.1 -- 6d. Clear WCEN, WCOFF (6e.) */
	writel(DMC_BASE, DDR_WCAL, readl(DMC_BASE, DDR_WCAL) & ~0x80000f00);
	readl(DMC_BASE, DDR_WCAL);

	/* 1.8.1 -- 6f. Set HCEN */
	writel(DMC_BASE, DDR_HCAL, readl(DMC_BASE, DDR_HCAL) | 0x80000000);
	readl(DMC_BASE, DDR_HCAL);

	/* 1.8.1 -- 6g. Set HCPROG */
	writel(DMC_BASE, DDR_HCAL, readl(DMC_BASE, DDR_HCAL) | 0x10000000);
	readl(DMC_BASE, DDR_HCAL);

	/* 1.8.1 -- 6h. Set HWFREQ */
	writel(DMC_BASE, MDCNFG, readl(DMC_BASE, MDCNFG) | 0x20000000);
	readl(DMC_BASE, MDCNFG);

	/* 1.8.1 -- 7. Write MDMRS */
	writel(DMC_BASE, MDMRS, 0x10000000 | (mdcnfg << 30) | ddr->mdmrs);
	readl(DMC_BASE, MDMRS);

	/* 1.8.1 -- 8. Write MDREFR */
	writel(DMC_BASE, MDREFR, 0x1e);
	readl(DMC_BASE, MDREFR);

	/* 1.8.1 -- 9. Program HCRNG > 0 */
	writel(DMC_BASE, DDR_HCAL, readl(DMC_BASE, DDR_HCAL) | 0x08000002);
	readl(DMC_BASE, DDR_HCAL);

	/* 1.8.1 -- 10. Program DMCEN, Enable DMC */
	writel(DMC_BASE, MDCNFG, readl(DMC_BASE, MDCNFG) | 0x40000000);
	readl(DMC_BASE, MDCNFG);
}
