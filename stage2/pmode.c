/**
 * @file	pmode.c
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

static bool check_a20();
static bool enable_a20_8042();
static bool enable_a20_fast();
static bool enable_a20_bios();

void enable_a20()
{
	if (check_a20() == true) {
		print("[i] A20 gate is already enabled.\r\n");
		return;
	}

	if (enable_a20_bios() == true) {
		print("[i] A20 gate is enabled using BIOS.\r\n");
		return;
	}

	if (enable_a20_fast() == true) {
		print("[i] A20 gate is enabled using Fast method.\r\n");
		return;
	}

	if (enable_a20_8042() == true) {
		print("[i] A20 gate is enabled using 8042 Keyboard Controller.\r\n");
		return;
	}

	print("[!] A20 did not enabled, can not proceed to protected mode.\r\n");
	hang(); /* stay in 16 bit mode */
}

static bool check_a20()
{
	/* TODO: implement */
	push(REG_SI | REG_DI | REG_ES | REG_DS | REG_FLAGS);
	cli();

	clr(REG_AX);
	mov_reg(REG_AX, REG_ES);
	dec(REG_AX);
	mov_reg(REG_AX, REG_DS);
	mov_val(0x500, REG_DI);
	mov_val(0x510, REG_SI);
	__asm__ volatile("mov %es:(%di), %al");
	push(REG_AX);
	__asm__ volatile("mov %ds:(%si), %al");
	push(REG_AX);
	__asm__ volatile("movw $0x00,%es:(%di)");
	__asm__ volatile("movw $0xFF, %ds:(%si)");
	__asm__ volatile("cmpw $0xff, %es:(%di)");
	pop(REG_AX);
	__asm__ volatile("mov %al, %ds:(%si)");
	pop(REG_AX);
	__asm__ volatile("mov %al, %es:(%di)");
	mov_val(0x00, REG_AX);
	// __asm__ volatile("je check"); //if (get_flags() & FLAG_ZF == true) { success } else { fail }


	sti();
	pop(REG_SI | REG_DI | REG_ES | REG_DS | REG_FLAGS);
	return true;
}

static bool enable_a20_8042()
{
	/* TODO: implement */
	return false;
}

static bool enable_a20_fast()
{
	/* TODO: implement */

	return false;
}

static bool enable_a20_bios()
{
	/* TODO: test if int 15 is supported? */
	__asm__ volatile("pusha");
	__asm__ volatile("movl $0x2401, %eax");
	__asm__ volatile("int $0x15");
	__asm__ volatile("popa");
	return true;
}

void disable_a20()
{
	/* implemented for debugging purposes */
	__asm__ volatile("pusha");
	__asm__ volatile("movl $0x2400, %eax");
	__asm__ volatile("int $0x15");
	__asm__ volatile("popa");
}