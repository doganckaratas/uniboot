/**
 * @file	vga.c
 * @brief	uniboot - the universal bootloader project
 * @version	v1.0
 * @author	dogan c. karatas
 * @date	11/02/2020
 **/

/* Tell gcc to emit 16-bit code */
__asm__ (".code16gcc");

#include <stdarg.h>
#include <stdint.h>
#include "common.h"
#include "stage2/vga.h"

void vga_putc(const char c)
{
	__asm__ ("int $0x10" : : "a"(0x0e00 | c));
}

void vga_puts(const char *s)
{
	for (; *s; ++s)
		vga_putc(*s);
}