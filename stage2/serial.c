/**
 * @file	serial.c
 * @brief	uniboot - the universal bootloader project
 * @version	v1.0
 * @author	dogan c. karatas
 * @date	11/02/2020
 **/

/* Tell gcc to emit 16-bit code */
__asm__ (".code16gcc");

#include <stddef.h>
#include <stdint.h>
#include <stdarg.h>
#include "common.h"
#include "cpu.h"
#include "util.h"
#include "stage2/serial.h"

/**
 * @brief there is only 1 tty device atm,
 * @todo: FIXME: make it linked list later.
 */
struct serial_port {
	enum serial_base_addr addr;
	uint16_t baud;
} serial_config;

/* Serial Mini-Info Sheet
0x3F8 = First Serial Port
0x2F8 = Second Serial Port
0x3E8 = Third Serial Port (May be different)
0x2E8 = Fourth Serial Port (May be different)

SBA = Serial Base Address
D   = Data
IE  = Interrupt Enable
TE  = Transmit Queue Empty
BE  = Break/Error
SC  = Status Change
BL  = Baud Rate LSB Bits
BH  = Baud Rate MSB Bits
DL  = Divisor Latch Address Bit
DB  = Data Bits   [00  = 5bit, 01  = 6bit, 10  = 7bit, 11  = 8bit]
SB  = Stop Bit    [0   = 1bit, 1   = 1,5/2 bit ]
PB  = Parity Bits [--0 = N/A, 001 = ODD,  011 = EVEN, 101 = MARK, 111 = SPC]
X   = Unused

        7  6  5  4  3  2  1  0
SBA+0 = D  D  D  D  D  D  D  D   Data Buffer          (DLAB 0)
SBA+1 = X  X  X  X  SC BE TE IE  Interrupt Enable Reg (DLAB 0)
SBA+0 = BL BL BL BL BL BL BL BL  LSB of Baud          (DLAB 1)
SBA+1 = BH BH BH BH BH BH BH BH  MSB of Baud          (DLAB 1)
SBA+2 = .  .  .  .  .  .  .  .   FIFO Control Reg
SBA+3 = DL .  PB PB PB SB DB DB  Line Control Reg
SBA+4 = .  .  .  .  .  .  .  .   Modem Control Reg
SBA+5 = .  .  .  .  .  .  .  .   Line Status Reg
SBA+6 = .  .  .  .  .  .  .  .   Modem Status Reg
SBA+7 = .  .  .  .  .  .  .  .   Scratch Reg
*/

/**
 * @fn         char* serial_addr_to_name(enum serial_base_address sba)
 * @brief      converts given serial base address to string
 * @param[in]  sba base address of serial port
 * @return     string representation of given port base address
 */
char* serial_addr_to_string(enum serial_base_addr sba)
{
	switch (sba) {
		case tty_serial0: return "ttyS0"; break;
		case tty_serial1: return "ttyS1"; break;
		case tty_serial2: return "ttyS2"; break;
		case tty_serial3: return "ttyS3"; break;
		default: return ""; break;
	}
}

/**
 * @fn         void serial_setup(enum serial_base_address sba, uint16_t baud)
 * @brief      serial port initialization function at given baud rate
 * @param[in]  sba base address of serial port
 * @param[in]  baud desired baud rate of serial port
 * @return     void
 */
void serial_setup(enum serial_base_addr sba, uint16_t baud)
{
	serial_config.addr = sba;
	serial_config.baud = baud;
	outb((uint16_t) (serial_config.addr + 1), 0x00); /* clear all ints */
	outb((uint16_t) (serial_config.addr + 3), 0x80); /* enable DLAB */
	outb((uint16_t) (serial_config.addr + 0), (uint8_t) (serial_config.baud & 0xFF)); /* lsb of baud */
	outb((uint16_t) (serial_config.addr + 1), (uint8_t) ((serial_config.baud >> 8) & 0xFF)); /* msb of baud */
	outb((uint16_t) (serial_config.addr + 3), 0x03); /* 8N1 setup */
	outb((uint16_t) (serial_config.addr + 2), 0xC7); /* enable FIFO */
	outb((uint16_t) (serial_config.addr + 4), 0x0B); /* IRQ bits set, Rts & dts set. */
	serial_send("terminal: serial_config\r\n"
		"device: %s (0x%x)\r\n",
		serial_addr_to_string(sba),
		sba);
}

/**
 * @fn         int serial_available()
 * @brief      serial port availability check for sending data
 * @return     0 if not available, 1 if available to send data
 */
int serial_send_available()
{
	return inb((uint16_t) (serial_config.addr + 5)) & 0x20;
}

/**
 * @fn         void serial_putc(const char c)
 * @brief      function for sending 1 char to serial port
 * @param[in]  c char to send
 * @return     void
 */
void serial_putc(const char c)
{
	if (serial_send_available()) {
		outb((uint16_t) (serial_config.addr), c);
	}
}

/**
 * @fn         void serial_puts(const char *str)
 * @brief      function for sending zero terminated string to serial port
 * @param[in]  str string to send
 * @return     void
 */
void serial_puts(const char *str)
{
	int i = 0;
	for (i = 0; i < (int) strlen((char *) str); i++) {
		serial_putc(*(str + i));
	}
}

/**
 * @fn         void serial_send(const char *fmt, ...)
 * @brief      function for sending formatted data to serial port
 * @param[in]  fmt format string for variadic arguments
 * @param[in]  va_args variadic arguments according to format string
 * @return     void
 */
void serial_send(const char *fmt, ...)
{
	va_list args;
	va_start(args, fmt);
	int d = 0;
	int x = 0;
	char c = '\0';
	char *s;
	while (*fmt != '\0') {
		if(*fmt == '%') {
			switch (*(fmt + 1)) {
				case 'd':
					d = va_arg(args, int);
					serial_puts(itoa(d, BASE_10));
					break;
				case 'c':
					c = va_arg(args, int);
					serial_putc((char) c);
					break;
				case 's':
					s = va_arg(args, char *);
					serial_puts(s);
					break;
				case 'x':
					x = va_arg(args, int);
					serial_puts(itoa(x, BASE_16));
					break;
				default:
					++fmt;
					break;
			}
			++fmt;
		} else {
			serial_putc(*fmt);
		}
		++fmt;
	}
	va_end(args);
}

/**
 * @fn         int serial_recv_available()
 * @brief      serial port availability check for receiving data
 * @return     0 if not available, 1 if available to send data
 */
int serial_recv_available()
{
	return inb((uint16_t) (serial_config.addr + 5)) & 0x01;
}

/**
 * @fn         void serial_recv_char(char *c)
 * @brief      function for receiving 1 char from serial port
 * @param[out] c received char
 * @return     void
 */
void serial_recv_char(char *c)
{
	//while (serial_recv_available() == 0);
	if (serial_recv_available()) {
		*c = (char) inb((uint16_t) (serial_config.addr));
	}
}

#if 0 /* to be implemented (irq) */
void serial_recv_str()
{

}

void serial_recv()
{

}
#endif
