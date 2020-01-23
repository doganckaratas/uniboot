/**
 * @file	itoa.h
 * @brief	uniboot - the universal bootloader project
 * @version	v1.0
 * @author	dogan c. karatas
 * @date	29/01/2019
 **/
#ifndef _ITOA_H
#define _ITOA_H

char* _itoa(int value, char* buffer, int base);
char* reverse(char *buffer, int i, int j);

#endif /* _ITOA_H */