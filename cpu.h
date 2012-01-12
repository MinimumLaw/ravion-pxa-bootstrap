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

#ifndef	__OPENPXA_CPU__
#define	__OPENPXA_CPU__

#define	PXA300	0x80
#define	PXA310	0x90
#define	PXA320	0x20

static inline unsigned long cpu_type(unsigned long cpu_type)
{
	unsigned long cpuid;
	asm volatile("mrc p15, 0, %0, c0, c0, 0" : "=r"(cpuid));
	return ((cpuid & 0x3f0) == cpu_type);
}

#endif
