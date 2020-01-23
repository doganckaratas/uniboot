/**
 * @file	util.c
 * @brief	uniboot - the universal bootloader project
 * @version	v1.0
 * @author	dogan c. karatas
 * @date	29/01/2019
 **/

/* Tell gcc to emit 16-bit code */
asm (".code16gcc");

#include <stdarg.h>
#include "itoa.h"
#include "stdtypes.h"
#include "common.h"


inline void swap(char *x, char *y) {
	char t = *x; *x = *y; *y = t;
}

// function to reverse buffer[i..j]
char* reverse(char *buffer, int i, int j)
{
	while (i < j)
		swap(&buffer[i++], &buffer[j--]);

	return buffer;
}

// Iterative function to implement itoa() function in C
char* _itoa(int value, char* buffer, int base)
{
	// invalid input
	if (base < 2 || base > 32)
		return buffer;

	// consider absolute value of number
	int n = abs(value);

	int i = 0;
	while (n)
	{
		int r = n % base;

		if (r >= 10) 
			buffer[i++] = 65 + (r - 10);
		else
			buffer[i++] = 48 + r;

		n = n / base;
	}

	// if number is 0
	if (i == 0)
		buffer[i++] = '0';

	// If base is 10 and value is negative, the resulting string 
	// is preceded with a minus sign (-)
	// With any other base, value is always considered unsigned
	if (value < 0 && base == 10)
		buffer[i++] = '-';

	buffer[i] = '\0'; // null terminate string

	// reverse the string and return it
	return reverse(buffer, 0, i - 1);
}

/** @fn _itoa_dec
 *  @param 
 *  @brief _itoa_dec convert integer to string
 **/
const char *_itoa_dec(int val)
{

}

#if 0
/** @fn _itoa_hex8
 *  @param 
 *  @brief _itoa_hex8 convert byte integer to hex string (1 character + 1 null byte)
 **/
const char *_itoa_hex8()
{

}

/** @fn _itoa_hex16
 *  @param 
 *  @brief _itoa_hex16 convert word integer to hex string (2 character + 1 null byte)
 **/
const char *_itoa_hex16()
{

}

/** @fn _itoa_hex32
 *  @param 
 *  @brief _itoa_hex32 convert dword integer to hex string (4 character + 1 null byte)
 **/
const char *_itoa_hex32()
{

}
#endif