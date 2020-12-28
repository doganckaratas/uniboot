/**
 * @file	util.h
 * @brief	uniboot - the universal bootloader project
 * @version	v1.0
 * @author	dogan c. karatas
 * @date	11/02/2020
 **/
#ifndef _UTIL_H
#define _UTIL_H

#include <stdint.h>

void memcpy(void *dst, void *src, int n);
void memset(void *dst, int c, int n);
int strlen(char *);
void reverse(char *);
char* itoa(uint32_t val, int base);
int atoi(char* str);

#endif /* _UTIL_H */