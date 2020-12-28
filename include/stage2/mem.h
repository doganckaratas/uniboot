/**
 * @file	memory.h
 * @brief	uniboot - the universal bootloader project
 * @version	v1.0
 * @author	dogan c. karatas
 * @date	11/02/2020
 **/
#ifndef _MEMORY_H
#define _MEMORY_H

#include <stdint.h>

struct memory_map_entry {
	uint64_t base_addr;
	uint64_t length;
	uint32_t type; // entry type
	uint32_t acpi_extended; // extended
}__attribute__((packed));

void reclaim_stage1_memory_area();
const char* memory_type_to_string(uint32_t type);
int __attribute__((noinline)) __attribute__((regparm(3))) e820_memory_detect(struct memory_map_entry* buffer);
void __attribute__((regparm(3))) load_memory_map(int map_addr);

#endif /* _MEMORY_H */