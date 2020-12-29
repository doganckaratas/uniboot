/**
 * @file	pmode.c
 * @brief	uniboot - the universal bootloader project
 * @version	v1.0
 * @author	dogan c. karatas
 * @date	11/02/2020
 **/

/* Tell gcc to emit 16-bit code */
__asm__ (".code16gcc");

#include <stdint.h>
#include "common.h"
#include "cpu.h"
#include "util.h"
#include "stage2/tty.h"
#include "stage2/mem.h"
#include "stage2/pmode.h"
#include "stage2/gdt.h"
#include "stage2/idt.h"

static bool check_a20();
static bool enable_a20_8042();
static bool enable_a20_fast();
static bool enable_a20_bios();

void enter_pmode()
{
	print("[pmode]: entering protected mode\r\n");
	disable_nmi();
	enable_a20();
	load_gdt();
	load_idt();
	return;
}

void enable_nmi()
{
	print("[pmode]: enabling nmi\r\n");
	outb(0x70, (uint8_t) (inb(0x70) & 0x7F));
}

void disable_nmi()
{
	print("[pmode]: disabling nmi\r\n");
	outb(0x70, (uint8_t) (inb(0x70) | 0x80));
}

void enable_a20()
{
	print("[mmu]: checking a20\r\n");
	if (check_a20() == true) {
		print("[mmu]: a20 enabled\r\n");
		return;
	}

	if (enable_a20_bios() == true) {
		print("[mmu]: a20 enabled via bios\r\n");
		return;
	}

	if (enable_a20_fast() == true) {
		print("[mmu]: a20 enabled via fasta20\r\n");
		return;
	}

	if (enable_a20_8042() == true) {
		print("[mmu]: a20 eanbled via 8042\r\n");
		return;
	}

	print("[mmu]: a20 disabled, protected mode fail\r\n");
	hang(); /* stay in 16 bit mode */
}

inline bool check_a20()
{
	bool status = false;
	pusha();
	cli();

	mov_val(0x00, REG_AX);
	mov_reg(REG_AX, REG_ES); // ES = 0x0000
	dec(REG_AX);
	mov_reg(REG_AX, REG_DS); // DS = 0xFFFF
	mov_val(0x00, REG_AX);
	mov_val(0x500, REG_DI); // ES:DI = 0x0000:0x0500
	mov_val(0x510, REG_SI); // DS:SI = 0xFFFF:0x0510
	mov_reg((REG_ES | REG_DI), REG_AL);
	push(REG_AX);
	mov_reg((REG_DS | REG_SI), REG_AL);
	push(REG_AX);
	mov_val(0x00, (REG_ES | REG_DI));
	mov_val(0xFF, (REG_DS | REG_SI));
	__asm__ volatile("cmpb $0xFF, %es:(%di)\n"); //bug?

	pop(REG_AX);
	mov_reg(REG_AL, (REG_DS | REG_SI));
	pop(REG_AX);
	mov_reg(REG_AL, (REG_ES | REG_DI));
	mov_val(0x00, REG_AX);

	if (get_flags() & FLAG_ZF) {
		status = true;
	} else {
		status = false;
	}

	sti();
	popa();
	return status;
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
	pusha();
	mov_val(0x2401, REG_EAX);
	int(0x15);
	popa();
	return true;
}