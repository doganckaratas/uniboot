/**
 * @file	mem.c
 * @brief	uniboot - the universal bootloader project
 * @version	v1.0
 * @author	dogan c. karatas
 * @date	11/02/2020
 **/

/* Tell gcc to emit 16-bit code */
__asm__ (".code16gcc");

#include <stdint.h>
#include "common.h"
#include "util.h"
#include "stage2/mem.h"
#include "stage2/tty.h"

void reclaim_stage1_memory_area()
{
	unsigned int *stage1 = (unsigned int *) 0x7c00;
	memset(stage1, 0, 512);
	print("[boot]: stage1 unloaded, 0x7c00 - 0x7dff is available\r\n");
}

const char* memory_type_to_string(uint32_t type)
{
	switch (type) {
		case 1: return "available";
		case 2: return "unavailable";
		case 3: return "acpi reclaimable";
		case 4: return "acpi non-volatile";
		case 5: return "bad";
		default: return "unknown";
	}
}

// load memory map to buffer - note: regparm(3) avoids stack issues with gcc in real mode
int __attribute__((noinline)) __attribute__((regparm(3))) e820_memory_detect(struct memory_map_entry* buffer)
{
	uint32_t end_of_memory = 0;
	int entry_count = 0, magic, bytes;
	while (1) {
		__asm__ ("int $0x15" :
			"=a"(magic), "=c"(bytes), "=b"(end_of_memory) :
			"a"(0xE820), "b"(end_of_memory), "c"(24), "d"(0x534D4150), "D"(buffer) /* ES:DI */
		);
		if (magic != 0x534D4150) {
			return -1; // error
		}
		if (bytes > 20 && (buffer->acpi_extended & 0x0001) == 0) {
			// ignore this entry
		} else {
			buffer++;
			entry_count++;
		}
		if (end_of_memory == 0) {
			break;
		}
	}
	return entry_count;
}

void __attribute__((regparm(3))) load_memory_map(int map_addr)
{
	print("[mem]: detecting memory via e820...\r\n");

	struct memory_map_entry* entry = (struct memory_map_entry*) map_addr;
	int entry_count = e820_memory_detect(entry);
	uint64_t upper_addr;

	print("[mem]: memory map is loaded at 0x%x\r\n", map_addr);

	if (entry_count == -1) {
		print("[mem]: failed to get memory map\r\n");
		return;
	} else {
		for (int i = 0; i < entry_count; i++) {
			upper_addr = entry[i].base_addr + entry[i].length;
			print("[mem]: entry %d: %s range: 0x%x - 0x%x len: 0x%x\r\n", i,
				memory_type_to_string(entry[i].type),
				entry[i].base_addr,
				upper_addr, // bug here in 64 bit printing!!!!!
				entry[i].length
			);
		}
	}
}
