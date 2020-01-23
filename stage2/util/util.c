/**
 * @file	util.c
 * @brief	uniboot - the universal bootloader project
 * @version	v1.0
 * @author	dogan c. karatas
 * @date	29/01/2019
 **/

/* Tell gcc to emit 16-bit code */
asm (".code16gcc");

#include <stdarg.h>
#include "util.h"
#include "itoa.h"
#include "stdtypes.h"
#include "common.h"

void putchar(char c)
{
	asm("int $0x0010" : : "a"(0x0e00 | c));
}

void puts(const char *s)
{
	for (; *s; ++s)
		putchar(*s);
}

// PORT PDCLIB !!

const char* itoa(int val, int base)
{
#if 0
	if (val == 0)
		return "0";
	static char buf[32] = {0};
	int i = 0;
	for (; val && i < 32; i++, val /= base) {
		buf[31 - i] = "0123456789abcdef"[val % base];
	}
	return (char *) &buf[32 - i];
#endif
	char buf[32] = {0};
	return _itoa(val, buf, base);
}

int abs(int val)
{
	if (val < 0) 
		return -val;
	else
		return val;
}

int atoi(char* str)
{
	int res = 0;
	int sign = 1;
	int i = 0;

	if (str[0] == '-') { 
		sign = -1; 
		i++;
	} 
  
	for (; str[i] != '\0'; ++i) 
		res = res * 10 + str[i] - '0'; 

	return sign * res; 
}

void print(const char *fmt, ...)
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
					puts(itoa(d, 10));
					break;
				case 'c':
					c = va_arg(args, int);
					putchar((char) c);
					break;
				case 's':
					s = va_arg(args, char *);
					puts(s);
					break;
				case 'x':
					x = va_arg(args, int);
					puts(itoa(x, 16));
					break;
				default:
					++fmt;
					break;
			}
			++fmt;
		} else {
			putchar(*fmt);
		}
		++fmt;
	}
	va_end(args);
}
