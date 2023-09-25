/**
 * @file	vga.h
 * @brief	uniboot - the universal bootloader project
 * @version	v1.0
 * @author	dogan c. karatas
 * @date	11/02/2020
 **/
#ifndef _VGA_H
#define _VGA_H

void vga_putc(const char c);
void vga_puts(const char *s);

#endif /* _VGA_H */