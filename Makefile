#
# Copyright (C) 2010 Marek Vasut <marek.vasut@gmail.com>
#
# This file is a part of the OpenPXA project, an Open Source OBM for the
# Marvell PXA3xx CPUs.
#
# This program is free software: you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation, either version 3 of the License, or
# (at your option) any later version.
# 
# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
# GNU General Public License for more details.
# 
# You should have received a copy of the GNU General Public License
# along with this program.  If not, see <http://www.gnu.org/licenses/>.
#

CFLAGS=-Wall -pedantic
ARM_CFLAGS=-Wall -pedantic
ARM_LDFLAGS=

PLAT=littleton
CPU=PXA310

all: iplntim.bin
	@echo "-----------------------------------------------------------------"
	@echo "All done, now flash the files like this:"
	@echo "1) iplntim.bin  to 0x00000 of NAND"
	@echo "2) Your bootloader to the offset specified in your platform file"
	@echo "-----------------------------------------------------------------"
	@echo "RESET the board to boot."

mkntim: mkntim.c
	$(CC) $(HOST_CFLAGS) $^ -DCONFIG_BLOCK -include plat/$(PLAT).c -o $@

ntim.bin: mkntim
	./$< > $@

%.o: %.c
	$(CROSS_COMPILE)gcc $(ARM_CFLAGS) -Os -I. -static -nostdlib -c $^ -o $@

ipl.o: start.o dma.o ddr.o uart.o nand.o timer.o plat/$(PLAT).o
	$(CROSS_COMPILE)ld $(ARM_LDFLAGS) --section-start=.boot=0x5c009000 -Ttext 0x5c009020 -Map=ipl.map -estart -o $@ $^

ipl.bin: ipl.o
	$(CROSS_COMPILE)size $<
	$(CROSS_COMPILE)objcopy -O binary $< $@

iplntim.bin: ntim.bin ipl.bin
	cat ntim.bin ipl.bin > iplntim.bin

clean:
	rm -f *.o plat/*.o *.bin mkntim ipl.map
