/**
 * @file	serial.h
 * @brief	uniboot - the universal bootloader project
 * @version	v1.0
 * @author	dogan c. karatas
 * @date	11/02/2020
 **/
#ifndef _SERIAL_H
#define _SERIAL_H

#include <stdint.h>

/**
 * @brief 16 Bit color escape strings for serial terminal
 */
#define	NORM	"\033[0m"
#define	BLACK	"\033[30m"
#define	RED	"\033[31m"
#define	GREEN	"\033[32m"
#define	ORANGE	"\033[33m"
#define	BLUE	"\033[34m"
#define	PURPLE	"\033[35m"
#define	CYAN	"\033[36m"
#define	LGRAY	"\033[37m"
#define	DGRAY	"\033[30;1m"
#define	LRED	"\033[31;1m"
#define	LGREEN	"\033[32;1m"
#define	YELLOW	"\033[33;1m"
#define	LBLUE	"\033[34;1m"
#define	LPURPLE	"\033[35;1m"
#define	LCYAN	"\033[36;1m"
#define	WHITE	"\033[37;1m"

/**
 * @brief base addresses for default serial ports
 */
enum serial_base_addr {
	tty_serial0=0x3F8,
	tty_serial1=0x2F8,
	tty_serial2=0x3E8,
	tty_serial3=0x2E8
};

char* serial_addr_to_string(enum serial_base_addr);
void serial_setup(enum serial_base_addr, uint16_t);
int serial_send_available();
void serial_putc(const char);
void serial_puts(const char *);
void serial_send(const char *fmt, ...);
int serial_recv_available();
void serial_recv_char(char*);

#endif /* __SERIAL_H__ */
