/**
 * @file	serial.c
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
#include "stage2/serial.h"

/* TODO: implement serial communication */

void serial_putc(char c)
{
	__asm__ ("int $0x10" : : "a"(0x0e00 | c));
}

void serial_puts(char *s)
{
	for (; *s; ++s)
		serial_putc(*s);
}