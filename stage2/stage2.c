/**
 * @file	stage2.c
 * @brief	uniboot - the universal bootloader project
 * @version	v1.0
 * @author	dogan c. karatas
 * @date	11/02/2020
 **/

/* Tell gcc to emit 16-bit code */
__asm__ (".code16gcc");

#include "stdtypes.h"
#include "common.h"
#include "cpu.h"
#include "util.h"
#include "stage2/tty.h"
#include "stage2/memory.h"
#include "stage2/pmode.h"

/* TODO 
 * Move stage 1 and stage 2 into arch/x86 folder.
 * All prints must go inside some special buffer (kmesg).
 * Set IRQ handler, load GDT IDT ...
 * Enable protected mode (32)
 * Implement Disk IO
 * ...
 * Load ELF Binary w/ GRUB Multiboot support
 * Jump Stage 3 (platform independent ELF)
 * ...
*/

/* Disk layout sample for x86
 *  Sector 0                           Sector 1                    Sector n
 *  ____________________________________________________________________________________
 * |Stage 1 + 2 + 3 (/boot partition) | powerkernel (/ partition) | EFI partition | ... |
 *  ------------------------------------------------------------------------------------
*/

void stage2() __attribute__((section (".stage2")));

void stage2()
{
	initialize_tty(TTY_DEVICE_VGA);
	reclaim_stage1_memory_area();
	print("[+] Stage 2 loaded at 0x%x.\r\n", 0x700);
	uint32_t fls = get_flags();
	load_memory_map(0x7c00);
	print("PF=%d\r\n", fls & FLAG_PF);
	disable_a20(); /* For debugging purposes */
	enable_a20();
	// load_gdt();
	// enter_protected_mode();
	hang();
}
