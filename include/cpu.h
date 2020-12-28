/**
 * @file	cpu.h
 * @brief	uniboot - the universal bootloader project
 * @version	v1.0
 * @author	dogan c. karatas
 * @date	11/02/2020
 **/
#ifndef _CPU_H
#define _CPU_H

#include <stdint.h>

/* x86 Register Names */
enum {
	REG_EAX = 1 << 0, REG_AX = 1 << 1, REG_AH = 1 << 2, REG_AL = 1 << 3,     
	REG_EBX = 1 << 4, REG_BX = 1 << 5, REG_BH = 1 << 6, REG_BL = 1 << 7,     
	REG_ECX = 1 << 8, REG_CX = 1 << 9, REG_CH = 1 << 10, REG_CL = 1 << 11,   
	REG_EDX = 1 << 12, REG_DX = 1 << 13, REG_DH = 1 << 14, REG_DL = 1 << 15,
	REG_EDI = 1 << 16, REG_DI = 1 << 17,                                    
	REG_ESI = 1 << 18, REG_SI = 1 << 19,                                    
	REG_EBP = 1 << 20, REG_BP = 1 << 21,                                    
	REG_ESP = 1 << 22, REG_SP = 1 << 23,                                    
	REG_GS = 1 << 24, REG_FS = 1 << 25, REG_ES = 1 << 26, REG_DS = 1 << 27,  
	REG_EFLAGS = 1 << 28, REG_FLAGS = 1 << 29 
};

/* Flag Masks */
enum {
	FLAG_CF = 1 << 0,
	FLAG_PF = 1 << 2,
	FLAG_AF = 1 << 4,
	FLAG_ZF = 1 << 6,
	FLAG_SF = 1 << 7,
	FLAG_TF = 1 << 8,
	FLAG_IF = 1 << 9,
	FLAG_DF = 1 << 10,
	FLAG_OF = 1 << 11,
	FLAG_IOPL = 1 << 12 | 1 << 13,
	FLAG_NT = 1 << 14,
	FLAG_RF = 1 << 16,
	FLAG_VM = 1 << 17,
	FLAG_AC = 1 << 18,
	FLAG_VIF = 1 << 19,
	FLAG_VIP = 1 << 20,
	FLAG_ID = 1 << 21
};

#define outb(port, value) __asm__ volatile("outb %1, %0\n" : : "Nd" ((uint16_t) port), "a" ((uint8_t) value))
#define outw(port, value) __asm__ volatile("outw %1, %0\n" : : "Nd" ((uint16_t) port), "Na" ((uint16_t) value))

#define inb(port) ({ \
	uint8_t ret = 0; \
	__asm__ volatile("inb %1, %0\n" : "=a" (ret) : "Nd" ((uint16_t) port)); \
	ret; \
})

#define inw(port) ({ \
	uint16_t ret = 0; \
	__asm__ volatile("inw %1, %0\n" : "=a" (ret) : "Nd" ((uint16_t) port)); \
	ret; \
})

#define reset_cpu()	outb(0x64, 0xfe)
#define int(interrupt) __asm__ volatile("int $" #interrupt "\n")
#define cli() __asm__ volatile ("cli\n")
#define sti() __asm__ volatile ("sti\n")
#define pusha() __asm__ volatile("pusha\n")
#define popa() __asm__ volatile("popa\n")
#define hlt() do { cli(); __asm__ volatile ("hlt\n"); } while(0);
#define hang() do { hlt(); } while(1);

#define mov_reg(from, to) do { \
	if ((from & REG_AX) && (to & REG_AX)) { __asm__ volatile("movw %ax, %ax\n"); } \
	else if ((from & REG_AX) && (to & REG_BX)) { __asm__ volatile("movw %ax, %bx\n"); } \
	else if ((from & REG_AX) && (to & REG_CX)) { __asm__ volatile("movw %ax, %cx\n"); } \
	else if ((from & REG_AX) && (to & REG_DX)) { __asm__ volatile("movw %ax, %dx\n"); } \
	else if ((from & REG_BX) && (to & REG_AX)) { __asm__ volatile("movw %bx, %ax\n"); } \
	else if ((from & REG_BX) && (to & REG_BX)) { __asm__ volatile("movw %bx, %bx\n"); } \
	else if ((from & REG_BX) && (to & REG_CX)) { __asm__ volatile("movw %bx, %cx\n"); } \
	else if ((from & REG_BX) && (to & REG_DX)) { __asm__ volatile("movw %bx, %dx\n"); } \
	else if ((from & REG_CX) && (to & REG_AX)) { __asm__ volatile("movw %cx, %ax\n"); } \
	else if ((from & REG_CX) && (to & REG_BX)) { __asm__ volatile("movw %cx, %bx\n"); } \
	else if ((from & REG_CX) && (to & REG_CX)) { __asm__ volatile("movw %cx, %cx\n"); } \
	else if ((from & REG_CX) && (to & REG_DX)) { __asm__ volatile("movw %cx, %dx\n"); } \
	else if ((from & REG_DX) && (to & REG_AX)) { __asm__ volatile("movw %dx, %ax\n"); } \
	else if ((from & REG_DX) && (to & REG_BX)) { __asm__ volatile("movw %dx, %bx\n"); } \
	else if ((from & REG_DX) && (to & REG_CX)) { __asm__ volatile("movw %dx, %cx\n"); } \
	else if ((from & REG_DX) && (to & REG_DX)) { __asm__ volatile("movw %dx, %dx\n"); } \
} while(0);

#define mov_val(value, to) do { \
	if (to & REG_DI) { __asm__ volatile("movw %0, %%di\n" : : "i" ((uint32_t) value)); } \
	else if (to & REG_SI) { __asm__ volatile("movw %0, %%si\n" : : "i" ((uint32_t) value)); } \
	else if (to & REG_AX) { __asm__ volatile("movw %0, %%ax\n" : : "i" ((uint32_t) value)); } \
	else if (to & REG_BX) { __asm__ volatile("movw %0, %%bx\n" : : "i" ((uint32_t) value)); } \
	else if (to & REG_CX) { __asm__ volatile("movw %0, %%cx\n" : : "i" ((uint32_t) value)); } \
	else if (to & REG_DX) { __asm__ volatile("movw %0, %%dx\n" : : "i" ((uint32_t) value)); } \
	else if (to & REG_EAX) { __asm__ volatile("movl %0, %%eax\n" : : "i" ((uint32_t) value)); } \
	else if (to & REG_EBX) { __asm__ volatile("movl %0, %%ebx\n" : : "i" ((uint32_t) value)); } \
	else if (to & REG_ECX) { __asm__ volatile("movl %0, %%ecx\n" : : "i" ((uint32_t) value)); } \
	else if (to & REG_EDX) { __asm__ volatile("movl %0, %%edx\n" : : "i" ((uint32_t) value)); } \
} while(0);

#define inc(regs) do { \
	if (regs & REG_DI) { __asm__ volatile("incw %di\n"); } \
	else if (regs & REG_SI) { __asm__ volatile("incw %si\n"); } \
	else if (regs & REG_AX) { __asm__ volatile("incw %ax\n"); } \
	else if (regs & REG_BX) { __asm__ volatile("incw %bx\n"); } \
	else if (regs & REG_CX) { __asm__ volatile("incw %cx\n"); } \
	else if (regs & REG_DX) { __asm__ volatile("incw %dx\n"); } \
} while(0);

#define dec(regs) do { \
	if (regs & REG_DI) { __asm__ volatile("decw %di\n"); } \
	else if (regs & REG_SI) { __asm__ volatile("decw %si\n"); } \
	else if (regs & REG_AX) { __asm__ volatile("decw %ax\n"); } \
	else if (regs & REG_BX) { __asm__ volatile("decw %bx\n"); } \
	else if (regs & REG_CX) { __asm__ volatile("decw %cx\n"); } \
	else if (regs & REG_DX) { __asm__ volatile("decw %dx\n"); } \
} while(0);

#define push(regs) do { \
	if (regs & REG_FLAGS) { __asm__ volatile("pushf\n"); } \
	else if (regs & REG_DS) { __asm__ volatile("pushw %ds\n"); } \
	else if (regs & REG_FS) { __asm__ volatile("pushw %fs\n"); } \
	else if (regs & REG_ES) { __asm__ volatile("pushw %es\n"); } \
	else if (regs & REG_GS) { __asm__ volatile("pushw %gs\n"); } \
	else if (regs & REG_DI) { __asm__ volatile("pushw %di\n"); } \
	else if (regs & REG_SI) { __asm__ volatile("pushw %si\n"); } \
	else if (regs & REG_AX) { __asm__ volatile("pushw %ax\n"); } \
	else if (regs & REG_BX) { __asm__ volatile("pushw %bx\n"); } \
	else if (regs & REG_CX) { __asm__ volatile("pushw %cx\n"); } \
	else if (regs & REG_DX) { __asm__ volatile("pushw %dx\n"); } \
} while(0);

#define pop(regs) do { \
	if (regs & REG_FLAGS) { __asm__ volatile("popf\n"); } \
	else if (regs & REG_DS) { __asm__ volatile("popw %ds\n"); } \
	else if (regs & REG_FS) { __asm__ volatile("popw %fs\n"); } \
	else if (regs & REG_ES) { __asm__ volatile("popw %es\n"); } \
	else if (regs & REG_GS) { __asm__ volatile("popw %gs\n"); } \
	else if (regs & REG_DI) { __asm__ volatile("popw %di\n"); } \
	else if (regs & REG_SI) { __asm__ volatile("popw %si\n"); } \
	else if (regs & REG_AX) { __asm__ volatile("popw %ax\n"); } \
	else if (regs & REG_BX) { __asm__ volatile("popw %bx\n"); } \
	else if (regs & REG_CX) { __asm__ volatile("popw %cx\n"); } \
	else if (regs & REG_DX) { __asm__ volatile("popw %dx\n"); } \
} while(0);

#define get_flags() ({ \
	uint32_t flags = 0;	\
	__asm__ volatile("pushl %%eax\npushf\npopl %0\npopl %%eax\n" : "=a" (flags)); \
	flags; \
})

#endif /* _CPU_H */