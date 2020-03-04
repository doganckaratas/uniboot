/**
 * @file	tty.c
 * @brief	uniboot - the universal bootloader project
 * @version	v1.0
 * @author	dogan c. karatas
 * @date	11/02/2020
 **/

/* Tell gcc to emit 16-bit code */
__asm__ (".code16gcc");

#include <stdarg.h>
#include "stdtypes.h"
#include "common.h"
#include "util.h"
#include "stage2/tty.h"

struct tty *tty;
struct tty tty1 = {.dev = TTY_DEVICE_VGA, .putc = vga_putc, .puts = vga_puts};
struct tty ttyS1 = {.dev = TTY_DEVICE_SERIAL, .putc = serial_putc, .puts = serial_puts};

void initialize_tty(enum tty_device dev)
{
	switch (dev){
		case TTY_DEVICE_SERIAL:
			tty = &ttyS1;
			break;
		case TTY_DEVICE_VGA:
			tty = &tty1;
			break;
		case TTY_DEVICE_NONE:
		default:
			tty = NULL;
			break;
	}
}

void print(const char *fmt, ...)
{
	va_list args;
	va_start(args, fmt);
	int d = 0;
	unsigned int x = 0;
	char c = '\0';
	char *s;
	while (*fmt != '\0') {
		if(*fmt == '%') {
			switch (*(fmt + 1)) {
				case 'd':
					d = va_arg(args, int);
					tty->puts(itoa(d, 10));
					break;
				case 'c':
					c = va_arg(args, int);
					tty->putc((char) c);
					break;
				case 's':
					s = va_arg(args, char *);
					tty->puts(s);
					break;
				case 'x':
					x = va_arg(args, int);
					tty->puts(itoa(x, 16));
					break;
				default:
					++fmt;
					break;
			}
			++fmt;
		} else {
			tty->putc(*fmt);
		}
		++fmt;
	}
	va_end(args);
}

void sprint(char *buf, const char *fmt, ...)
{
	va_list args;
	va_start(args, fmt);
	int d = 0;
	unsigned int x = 0;
	char c = '\0';
	char *s;
	while (*fmt != '\0') {
		if(*fmt == '%') {
			switch (*(fmt + 1)) {
				case 'd':
					d = va_arg(args, int);
					memcpy((void *) buf, (void *) itoa(d, 10), strlen(itoa(d,10)));
					buf += strlen(itoa(d,10));
					break;
				case 'c':
					c = va_arg(args, int);
					*(buf) = c;
					++buf;
					break;
				case 's':
					s = va_arg(args, char *);
					memcpy((void *) buf, (void *) s, strlen(s));
					buf += strlen(s);
					break;
				case 'x':
					x = va_arg(args, int);
					memcpy((void *) buf, (void *) itoa(x, 16), strlen(itoa(x,16)));
					buf += strlen(itoa(x,16));
					break;
				default:
					++fmt;
					break;
			}
			++fmt;
		} else {
			*(buf) = *(fmt);
			++buf;
		}
		++fmt;
	}
	va_end(args);
}
