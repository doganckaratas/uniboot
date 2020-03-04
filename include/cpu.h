/**
 * @file	cpu.h
 * @brief	uniboot - the universal bootloader project
 * @version	v1.0
 * @author	dogan c. karatas
 * @date	11/02/2020
 **/
#ifndef _CPU_H
#define _CPU_H

#include "stdtypes.h"

/* x86 Register Values */
union regs {
	struct {
		uint32_t eax;
		uint32_t ebx;
		uint32_t ecx;
		uint32_t edx;
		uint32_t edi;
		uint32_t esi;
		uint32_t ebp;
		uint32_t esp;
		uint32_t fsgs;
		uint32_t dses;
		uint32_t eflags;
	};
	struct {
		uint16_t ax, eaxh;
		uint16_t bx, ebxh;
		uint16_t cx, ecxh;
		uint16_t dx, edxh;
		uint16_t di, edih;
		uint16_t si, esih;
		uint16_t bp, ebph;
		uint16_t sp, esph;
		uint16_t gs;
		uint16_t fs;
		uint16_t es;
		uint16_t ds;
		uint16_t flags, eflagsh;
	};
	struct {
		uint8_t al, ah, axl, axh;
		uint8_t bl, bh, bxl, bxh;
		uint8_t cl, ch, cxl, cxh;
		uint8_t dl, dh, dxl, dxh;
		uint8_t dil, dih, _edil, _edih;
		uint8_t sil, sih, _esil, _esih;
		uint8_t bpl, bph, _ebpl, _ebph;
		uint8_t spl, sph, _espl, _esph;
	};
};

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

void reset_cpu();
void cli();
void sti();
void hlt();
void hang();
void push(uint32_t regs);
void pop(uint32_t regs);
void inc(uint32_t regs);
void dec(uint32_t regs);
void mov_reg(uint32_t from, uint32_t to);
void mov_val(uint32_t value, uint32_t to);
void clr(uint32_t regs);
void outb(uint16_t port, uint8_t value);
void outw(uint16_t port, uint16_t value);
uint8_t inb(uint16_t port);
uint16_t inw(uint16_t port);
uint32_t get_flags();

#endif /* _CPU_H */