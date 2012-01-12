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

#ifndef	__OPENPXA_CACHE__
#define	__OPENPXA_CACHE__

#define	enable_icache_and_btb()						\
		do {							\
			asm volatile(					\
				"mrc	p15, 0, r0, c1, c0, 0\n"	\
				"orr	r0, #0x1800\n"			\
				"mcr	p15, 0, r0, c1, c0, 0\n"	\
				/* CPWAIT */				\
				"mrc	p15, 0, r0, c2, c0, 0\n"	\
				"mov	r0, r0\n"			\
				"sub	pc, pc, #4\n"			\
			:::"r0");					\
		} while (0)

#define	disable_icache_and_btb()					\
		do {							\
			asm volatile(					\
				"mrc	p15, 0, r0, c1, c0, 0\n"	\
				"bic	r0, #0x1800\n"			\
				"mcr	p15, 0, r0, c1, c0, 0\n"	\
				/* CPWAIT */				\
				"mrc	p15, 0, r0, c2, c0, 0\n"	\
				"mov	r0, r0\n"			\
				"sub	pc, pc, #4\n"			\
			:::"r0");					\
		} while (0)

#endif	/* __OPENPXA_CACHE__ */
