/**
 * @file	memory.h
 * @brief	uniboot - the universal bootloader project
 * @version	v1.0
 * @author	dogan c. karatas
 * @date	29/01/2019
 **/
#ifndef _MEMORY_H
#define _MEMORY_H

#include "stdtypes.h"

typedef struct map_entry {

	uint32_t base_addr_low; // base address uint64_t
	uint32_t base_addr_high;
	uint32_t length_low; // length uint64_t
	uint32_t length_high;
	uint32_t type; // entry type
	uint32_t acpi_extended; // extended

}__attribute__((packed)) map_entry_t;

const char* memory_type_to_string(uint32_t type);
int __attribute__((noinline)) __attribute__((regparm(3))) memory_detect(map_entry_t* buffer, int maxentries);
void memory_report_e820(int map_addr, int map_size);

#endif /* _MEMORY_H */