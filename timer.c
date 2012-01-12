/*
 * Copyright (C) 2010 David Hunter <hunterd42@gmail.com>
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
#include <timer.h>

#define OST_BASE	0x40A00000
#define OSCR0		0x10

#define OSCR0_FREQ_KHZ	3250

void mdelay(unsigned int msec)
{
	unsigned long ticks = msec * OSCR0_FREQ_KHZ;

	writel(OST_BASE, OSCR0, 0);

	while (readl(OST_BASE, OSCR0) < ticks)
		;
}
