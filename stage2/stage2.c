/**
 * @file	stage2.c
 * @brief	uniboot - the universal bootloader project
 * @version	v1.0
 * @author	dogan c. karatas
 * @date	29/01/2019
 **/

/* Tell gcc to emit 16-bit code */
asm (".code16gcc");

#include "stdtypes.h"
#include "common.h"
#include "util.h"
#include "memory.h"

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
void seek_file(void);

void stage2()
{
	print("uniboot "VERSION", written by Dogan C. Karatas\r\n"
		"[+] Stage 1 loaded.\r\n"
		"[+] Stage 2 loaded.\r\n");
	memory_report_e820(0x1000, 0x2000);
	print("test: %d\r\n", atoi("123") + 25 );

	while (1);
}

void seek_file()
{
	;
}
