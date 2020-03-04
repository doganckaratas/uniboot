/**
 * @file	util.c
 * @brief	uniboot - the universal bootloader project
 * @version	v1.0
 * @author	dogan c. karatas
 * @date	11/02/2020
 **/

/* Tell gcc to emit 16-bit code */
__asm__ (".code16gcc");

#include <stdarg.h>
#include "stdtypes.h"
#include "common.h"
#include "util.h"

void memcpy(void *dst, void *src, int n)
{
	int i = 0;

	unsigned char *dstptr = dst;
	unsigned char *srcptr = src;
	for (i = 0; i < n; i++) {
		*(dstptr + i) = *(srcptr + i);
	}
}

void memset(void *dst, int c, int n)
{
	unsigned int *buf = (unsigned int *) dst;
	int i = 0;

	for (; i < n; i++) {
		*(buf + i) = c;
	}
}

int strlen(char *c)
{
	int i = 0;
	for (; *c != '\0';c++, i++);
	return i;
}

void reverse(char s[])
 {
	int i, j;
	char c;

	for (i = 0, j = strlen(s)-1; i<j; i++, j--) {
		c = s[i];
		s[i] = s[j];
		s[j] = c;
	}
}

char* itoa(unsigned int val, int base)
{
	static char buf[64] = {0};

	// 32 bit means 8 figures,
	int i = 0;
	int pad = 8;
	do {       /* generate digits in reverse order */
	     buf[i++] = "0123456789abcdef"[val % base];   /* get next digit */
	} while ((val /= base) > 0);    /* delete it */

	if (base == 16) {
		for (; pad > i; i++)
			buf[i] = '0';
	}
	buf[i] = '\0';

	reverse(buf);
	return buf;
}

int atoi(char* str)
{
	int res = 0;
	int sign = 1;
	int i = 0;

	if (str[0] == '-') {
		sign = -1;
		i++;
	}

	for (; str[i] != '\0'; ++i)
		res = res * 10 + str[i] - '0';

	return sign * res;
}
