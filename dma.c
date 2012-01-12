/*
 * Copyright (C) 2011 Marek Vasut <marek.vasut@gmail.com>
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
#include <dma.h>

#define	DMA_BASE	0x40000000
#define	DCSR0		0x000
#define	DALGN		0x0a0
#define	DRCMR0		0x100
#define	DDADR0		0x200
#define	DSADR0		0x204
#define	DTADR0		0x208
#define	DCMD0		0x20c

/* DCMD */
#define	INCSRCADDR	(1 << 31)
#define	INCTRGADDR	(1 << 30)
#define	FLOWSRC		(1 << 29)
#define	FLOWTRG		(1 << 28)
#define	CMPEN		(1 << 25)
#define	ADDRMODE	(1 << 23)
#define	STARTIRQEN	(1 << 22)
#define	ENDIRQEN	(1 << 21)
#define	SIZE(n)		(((n) & 3) << 16)
#define	WIDTH(n)	(((n) & 3) << 14)
#define	LEN(n)		((n) & 0x1fff)

/* DCSR */
#define	RUN		(1 << 31)
#define	NODESCFETCH	(1 << 30)
#define	STOPIRQEN	(1 << 29)
#define	EORIRQEN	(1 << 28)
#define	EORJMPEN	(1 << 27)
#define	EORSTOPEN	(1 << 26)
#define	SETCMPST	(1 << 25)
#define	CLRCMPST	(1 << 24)
#define	RASIRQEN	(1 << 23)
#define	MASKRUN		(1 << 22)
#define	CMPST		(1 << 10)
#define	EORINTR		(1 << 9)
#define	REQPEND		(1 << 8)
#define	RASINTR		(1 << 4)
#define	STOPINTR	(1 << 3)
#define	ENDINTR		(1 << 2)
#define	STARTINTR	(1 << 1)
#define	BUSERRINTR	(1 << 0)

void mem2mem_dma(unsigned long from, unsigned long to, unsigned long cnt)
{

	writel(DMA_BASE, DALGN, 0);
	writel(DMA_BASE, DCSR0, NODESCFETCH);
	writel(DMA_BASE, DSADR0, from);
	writel(DMA_BASE, DTADR0, to);
	writel(DMA_BASE, DDADR0, 1);
	writel(DMA_BASE, DCMD0,
		INCSRCADDR | INCTRGADDR | SIZE(3) | LEN(cnt));
	writel(DMA_BASE, DCSR0, RUN | NODESCFETCH);
	while (!(readl(DMA_BASE, DCSR0) & STOPINTR));
	writel(DMA_BASE, DCSR0, NODESCFETCH);
}
