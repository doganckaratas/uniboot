/**
 * @file	tty.h
 * @brief	uniboot - the universal bootloader project
 * @version	v1.0
 * @author	dogan c. karatas
 * @date	11/02/2020
 **/
#ifndef _TTY_H
#define _TTY_H

#include "common.h"
#include "stage2/vga.h"
#include "stage2/serial.h"

enum tty_device {
	TTY_DEVICE_NONE,
	TTY_DEVICE_SERIAL,
	TTY_DEVICE_VGA,
};

struct tty {
	enum tty_device dev;
	void (*putc) (char);
	void (*puts) (char *);
};

void initialize_tty(enum tty_device);
void print(const char *fmt, ...);
void sprint(char *buf, const char *fmt, ...);

#endif /* _TTY_H */