/*
 * Copyright (C) 2010-2011 Marek Vasut <marek.vasut@gmail.com>
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

#ifndef	__OPENPXA_UART__
#define	__OPENPXA_UART__

#define	UART1_EN	0x1
#define	UART2_EN	0x2
#define	UART3_EN	0x4

struct uart_config {
	unsigned long	enabled;
};

void plat_uart_setup(struct uart_config *uart);
void uart_init(struct uart_config *uart);
void uart_putc(struct uart_config *uart, volatile char c);
void uart_puts(struct uart_config *uart, char *c);
void uart_putl(struct uart_config *uart, long l);

#endif	/* __OPENPXA_UART__ */
