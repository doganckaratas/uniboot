/**
 * @file	common.h
 * @brief	uniboot - the universal bootloader project
 * @version	v1.0
 * @author	dogan c. karatas
 * @date	11/02/2020
 **/
#ifndef _COMMON_H
#define _COMMON_H

#include <stdint.h>

#define VERSION "v1.0"

#ifndef NULL
#define NULL ((void *) 0)
#endif /* NULL */

typedef enum {false, true} bool;

#endif /* _COMMON_H */