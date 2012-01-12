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

#include <common.h>
#include <uart.h>

#define	RBR	0x00
#define	THR	0x00
#define	DLL	0x00
#define	DLH	0x04
#define	IER	0x04
#define	IIR	0x08
#define	FCR	0x08
#define	LCR	0x0c
#define	MCR	0x10
#define	LSR	0x14
#define	MSR	0x18
#define	SCR	0x1c
#define	ISR	0x20
#define	FOR	0x24
#define	ABR	0x28
#define	ACR	0x2c

#define	UART1	0x40100000
#define	UART2	0x40200000
#define	UART3	0x40700000

static const unsigned long uarts[] = { UART1, UART2, UART3 };

/*
 * Initialize UARTs
 */
void uart_init(struct uart_config *uart)
{
	int i;
	for (i = 0; i < sizeof(uarts); i++)
		if (uart->enabled & (1 << i)) {
			writel(uarts[i], IER, 0x00);	/* IER = 0x00 */
			writel(uarts[i], FCR, 0x00);	/* FCR = 0x00 */
			writel(uarts[i], LCR, 0x83);	/* LCR = 0x83 */
			writel(uarts[i], DLL, 0x08);	/* DLL = 0x08 */
			writel(uarts[i], DLH, 0x00);	/* DLH = 0x00 */
			writel(uarts[i], LCR, 0x03);	/* LCR = 0x03 */
			writel(uarts[i], IER, 0x40);	/* IER = 0x40 */
		}
}

/*
 * Send byte over UART
 */
void uart_putc(struct uart_config *uart, volatile char c)
{
	int i;
	for (i = 0; i < sizeof(uarts); i++)
		if (uart->enabled & (1 << i)) {
			while (!(readl(uarts[i], LSR) & 0x40));
			writel(uarts[i], THR, c);
		}
}

/*
 * Send NULL-terminated string over UART
 */
void uart_puts(struct uart_config *uart, char *c)
{
	while (*c != '\0')
		uart_putc(uart, *c++);
}

/*
 * Send long number as a string over UART
 */
void uart_putl(struct uart_config *uart, long l)
{
	int i;
	char c;
	uart_puts(uart, "0x");

	for (i = 28; i >=0; i -= 4) {
		c = (l >> i) & 0xf;
		uart_putc(uart, c + ((c < 0xa) ? 0x30 : 0x57));
	}
}
