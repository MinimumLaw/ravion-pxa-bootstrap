/*
 * mkntim.c -- Generator for PXA3xx boot image headers
 *
 * Copyright (C) 2010 Marek Vasut <marek.vasut@gmail.com>
 *
 * This file is a part of the OpenPXA project, an Open Source OBM for the
 * Marvell PXA310 and Marvell PXA320 CPU.
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

#include <stdio.h>
#include <stdint.h>
#include <string.h>

#ifndef	NAND_BUS_WIDTH
#warning Define NAND_BUS_WIDTH in your platform file
#endif

struct hmit {
	uint32_t	hdr;
	uint32_t	offset;
	uint32_t	date;
	uint32_t	magic;
	uint32_t	ff[5];
	uint32_t	mtype;
	uint32_t	nparts;
	uint32_t	zero[2];
	uint32_t	ftr;
};

struct sect {
	uint32_t	hdr;
	uint32_t	noffset;
	uint32_t	moffset;
	uint32_t	size;
	uint32_t	zero[10];
	uint32_t	ftr;
};

struct ntim {
	uint32_t	ver;
	struct	hmit	hmit;
	struct	sect	obmi;
	struct	sect	boot;	
};

#define	compose(a, b, c, d)	((a) << 24 | (b) << 16 | (c) << 8 | (d))

int main()
{
	struct ntim file;
	int i;
	unsigned char width;

	if (NAND_BUS_WIDTH == 8)
		width = 0x06;
	else if (NAND_BUS_WIDTH == 16)
		width = 0x04;
	else {
		fprintf(stderr, "NAND_BUS_WIDTH contains invalid value!\n");
		return 1;
	}

	memset(&file, 0, sizeof(file));

	file.ver = 0xea000000;	/* Jump 8 bytes further */

	file.hmit.hdr	= compose('T', 'I', 'M', 'H');
	file.hmit.offset= 0;
	file.hmit.date	= 0x11032006;
	file.hmit.magic	= 0xdeadbeef;
	memset(file.hmit.ff, 0xff, sizeof(file.hmit.ff));
	file.hmit.mtype	= compose('N', 'A', 'N', width);
	file.hmit.nparts= 3;
	memset(file.hmit.zero, 0x00, sizeof(file.hmit.zero));
	file.hmit.ftr	= compose('T', 'I', 'M', 'H') ;

	file.obmi.hdr	= compose('O', 'B', 'M', 'I');
	file.obmi.noffset= 0x00;
	file.obmi.moffset= 0x5c008000;
	file.obmi.size	= 0xff;
	memset(file.obmi.zero, 0x00, sizeof(file.obmi.zero));
	file.obmi.ftr	= compose('O', 'B', 'M', 'I') ;

	file.boot.hdr	= compose('B', 'O', 'O', 'T');
	file.boot.noffset= 0x00000;
	file.boot.moffset= 0x5c014000;
	file.boot.size	= 0x1000;
	memset(file.boot.zero, 0x00, sizeof(file.boot.zero));
	file.boot.ftr	= compose('B', 'O', 'O', 'T') ;

	/* Update the date for PXA320 -- jump past the NTIM header */
	file.hmit.date	= 0xea000000 | ((sizeof(file) >> 2) - 5);
	/* Another weirdness of PXA320 */
#ifdef	SKIP_TIHM
	file.hmit.hdr = 0;
#endif

	for (i = 0; i < sizeof(file); i++)
		printf("%c", *(((uint8_t *)&file) + i));

	return 0;
}
