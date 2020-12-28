/**
 * @file	gdt.c
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
#include "stage2/gdt.h"
#include "stage2/idt.h"

void load_gdt()
{
    print("[pmode]: loading gdt\r\n");
    return;
}