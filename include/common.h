/**
 * @file	common.h
 * @brief	uniboot - the universal bootloader project
 * @version	v1.0
 * @author	dogan c. karatas
 * @date	11/02/2020
 **/
#ifndef _COMMON_H
#define _COMMON_H

#include "stdtypes.h"

#define VERSION "v1.0"


#if 0

/* just a little sample for flags check */
char *check_carry()
{
	return regs.eflags & FLAG_CF ? "Carry set" : "Carry Clear";
}
#endif

#endif /* _COMMON_H */