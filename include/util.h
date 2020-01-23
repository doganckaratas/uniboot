/**
 * @file	util.h
 * @brief	uniboot - the universal bootloader project
 * @version	v1.0
 * @author	dogan c. karatas
 * @date	29/01/2019
 **/
#ifndef _UTIL_H
#define _UTIL_H

void putchar(char c);
void puts(const char *s);
const char* itoa(int val, int base);
int atoi(char* str);
void print(const char *fmt, ...);
int abs(int val);


#endif /* _UTIL_H */