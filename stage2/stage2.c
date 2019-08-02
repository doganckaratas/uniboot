/**
 * @file	stage2.c
 * @brief	uniboot - the universal bootloader project
 * @version	v1.0
 * @author	dogan c. karatas
 * @date	29/01/2019
 **/

/* Tell gcc to emit 16-bit code */
asm (".code16gcc");
#include "common.h"

/* TODO 
 * Implement Disk IO
 * Load ELF Binary w/ GRUB Multiboot support
 * ...
*/

/* Disk layout sample
 *  Sector 0                       Sector 1                     ...
 *  ________________________________________________________________
 * |Stage 1 + 2 (/boot partition) | powerkernel (/ partition) | ... |
 *  ----------------------------------------------------------------
*/

void stage2() __attribute__((section (".stage2")));
void putchar(char c);
void puts(const char *s);

void putchar(char c)
{
	asm("int $0x0010" : : "a"(0x0e00 | c));
}

void puts(const char *s)
{
	for (; *s; ++s)
		putchar(*s);
}

void stage2()
{
	puts("uniboot "VERSION"\r\n");
	puts("[+] Stage 1 loaded.\r\n");
	puts("[+] Stage 2 loaded.\r\n");
	while (1);
}
