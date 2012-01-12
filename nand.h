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

#ifndef	__OPENPXA_NAND__
#define	__OPENPXA_NAND__

struct nand_layout {
/* internal bootloader variables, do not modify */
	unsigned long	page_sz;	/* NAND page size */

/* configuration options, these should be modified in the platform file */
	unsigned long	copy_start;	/* address of first page to be copied */
	unsigned long	copy_size;	/* size of data to be copied */
	unsigned long	copy_dest;	/* destination address */
};

void nand_configure(struct nand_layout *nand);
void nand_copy(struct nand_layout *nand);

#endif	/* __OPENPXA_NAND__ */
