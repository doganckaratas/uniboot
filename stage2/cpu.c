/**
 * @file	cpu.c
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

inline void reset_cpu()
{
	outb(0x64, 0xfe);
}

inline void cli()
{
	__asm__ volatile ("cli");
}

inline void sti()
{
	__asm__ volatile ("sti");
}

inline void hlt()
{
	cli();
	__asm__ volatile ("hlt");
}

inline void push(uint32_t regs)
{
	if (regs & REG_FLAGS) __asm__ volatile("pushf");
	if (regs & REG_DS)    __asm__ volatile("pushw %ds");
	if (regs & REG_FS)    __asm__ volatile("pushw %fs");
	if (regs & REG_ES)    __asm__ volatile("pushw %es");
	if (regs & REG_GS)    __asm__ volatile("pushw %gs");
	if (regs & REG_DI)    __asm__ volatile("pushw %di");
	if (regs & REG_SI)    __asm__ volatile("pushw %si");
	if (regs & REG_AX)    __asm__ volatile("pushw %ax");
	if (regs & REG_BX)    __asm__ volatile("pushw %bx");
	if (regs & REG_CX)    __asm__ volatile("pushw %cx");
	if (regs & REG_DX)    __asm__ volatile("pushw %dx");
}

inline void pop(uint32_t regs)
{
	if (regs & REG_FLAGS) __asm__ volatile("popf");
	if (regs & REG_DS)    __asm__ volatile("popw %ds");
	if (regs & REG_FS)    __asm__ volatile("popw %fs");
	if (regs & REG_ES)    __asm__ volatile("popw %es");
	if (regs & REG_GS)    __asm__ volatile("popw %gs");
	if (regs & REG_DI)    __asm__ volatile("popw %di");
	if (regs & REG_SI)    __asm__ volatile("popw %si");
	if (regs & REG_AX)    __asm__ volatile("popw %ax");
	if (regs & REG_BX)    __asm__ volatile("popw %bx");
	if (regs & REG_CX)    __asm__ volatile("popw %cx");
	if (regs & REG_DX)    __asm__ volatile("popw %dx");
}

inline void inc(uint32_t regs)
{
	if (regs & REG_DI) __asm__ volatile("incw %di");
	if (regs & REG_SI) __asm__ volatile("incw %si");
	if (regs & REG_AX) __asm__ volatile("incw %ax");
	if (regs & REG_BX) __asm__ volatile("incw %bx");
	if (regs & REG_CX) __asm__ volatile("incw %cx");
	if (regs & REG_DX) __asm__ volatile("incw %dx");
}

inline void dec(uint32_t regs)
{
	if (regs & REG_DI) __asm__ volatile("decw %di");
	if (regs & REG_SI) __asm__ volatile("decw %si");
	if (regs & REG_AX) __asm__ volatile("decw %ax");
	if (regs & REG_BX) __asm__ volatile("decw %bx");
	if (regs & REG_CX) __asm__ volatile("decw %cx");
	if (regs & REG_DX) __asm__ volatile("decw %dx");
}

inline void mov_reg(uint32_t from, uint32_t to)
{
	if (from & REG_AX && to & REG_AX) __asm__ volatile("movw %ax, %ax");
	if (from & REG_AX && to & REG_BX) __asm__ volatile("movw %ax, %bx");
	if (from & REG_AX && to & REG_CX) __asm__ volatile("movw %ax, %cx");
	if (from & REG_AX && to & REG_DX) __asm__ volatile("movw %ax, %dx");
	if (from & REG_BX && to & REG_AX) __asm__ volatile("movw %bx, %ax");
	if (from & REG_BX && to & REG_BX) __asm__ volatile("movw %bx, %bx");
	if (from & REG_BX && to & REG_CX) __asm__ volatile("movw %bx, %cx");
	if (from & REG_BX && to & REG_DX) __asm__ volatile("movw %bx, %dx");
	if (from & REG_CX && to & REG_AX) __asm__ volatile("movw %cx, %ax");
	if (from & REG_CX && to & REG_BX) __asm__ volatile("movw %cx, %bx");
	if (from & REG_CX && to & REG_CX) __asm__ volatile("movw %cx, %cx");
	if (from & REG_CX && to & REG_DX) __asm__ volatile("movw %cx, %dx");
	if (from & REG_DX && to & REG_AX) __asm__ volatile("movw %dx, %ax");
	if (from & REG_DX && to & REG_BX) __asm__ volatile("movw %dx, %bx");
	if (from & REG_DX && to & REG_CX) __asm__ volatile("movw %dx, %cx");
	if (from & REG_DX && to & REG_DX) __asm__ volatile("movw %dx, %dx");
}

inline void mov_val(uint32_t value, uint32_t to)
{
	if (to & REG_DI) __asm__ volatile("movw %0, %%di" : : "m" (value));
	if (to & REG_SI) __asm__ volatile("movw %0, %%si" : : "m" (value));
	if (to & REG_AX) __asm__ volatile("movw %0, %%ax" : : "m" (value));
	if (to & REG_BX) __asm__ volatile("movw %0, %%bx" : : "m" (value));
	if (to & REG_CX) __asm__ volatile("movw %0, %%cx" : : "m" (value));
	if (to & REG_DX) __asm__ volatile("movw %0, %%dx" : : "m" (value));
}

inline void clr(uint32_t regs)
{
	if (regs & REG_DI) __asm__ volatile("xorw %di, %di");
	if (regs & REG_SI) __asm__ volatile("xorw %si, %si");
	if (regs & REG_AX) __asm__ volatile("xorw %ax, %ax");
	if (regs & REG_BX) __asm__ volatile("xorw %bx, %bx");
	if (regs & REG_CX) __asm__ volatile("xorw %cx, %cx");
	if (regs & REG_DX) __asm__ volatile("xorw %dx, %dx");
}

inline void hang()
{
	hlt();
	while(1);
}

inline void outb(uint16_t port, uint8_t value)
{
	__asm__ volatile("outb %1, %0" : : "d" (port), "a" (value));
}

inline void outw(uint16_t port, uint16_t value)
{
	__asm__ volatile("outw %1, %0" : : "d" (port), "a" (value));
}

inline uint8_t inb(uint16_t port)
{
	uint8_t ret;
	__asm__ volatile("inb %1, %0" : "=a" (ret) : "d" (port));
	return ret;
}

inline uint16_t inw(uint16_t port)
{
	uint16_t ret;
	__asm__ volatile("inw %1, %0" : "=a" (ret) : "d" (port));
	return ret;
}

inline uint32_t get_flags()
{
	uint32_t flags = 0;
	// push(REG_FLAGS);
	
	__asm__ volatile("pushf\r\n pop %0" : "=m" (flags));
	
	// pop(REG_FLAGS);

	// __asm__ volatile("mov %%ax, %0" : "=a" (flags): );
	return flags;
}
