/**
 * @file	memory.c
 * @brief	uniboot - the universal bootloader project
 * @version	v1.0
 * @author	dogan c. karatas
 * @date	29/01/2019
 **/

/* Tell gcc to emit 16-bit code */
asm (".code16gcc");

#include "memory.h"
#include "stdtypes.h"
#include "common.h"
#include "util.h"

const char* memory_type_to_string(uint32_t type)
{
	switch (type) {
		case 1: return "Available";
		case 2: return "Unavailable";
		case 3: return "ACPI Reclaimable Memory";
		case 4: return "ACPI Non Volatile Memory";
		case 5: return "Bad Memory";
		default: return "Unknown Type of Memory!";
	}
}

// load memory map to buffer - note: regparm(3) avoids stack issues with gcc in real mode
int __attribute__((noinline)) __attribute__((regparm(3))) memory_detect(map_entry_t* buffer, int maxentries)
{
	uint32_t contID = 0;
	int entries = 0, signature, bytes;
	do {
		__asm__ __volatile__ ("int  $0x15"
				: "=a"(signature), "=c"(bytes), "=b"(contID)
				: "a"(0xE820), "b"(contID), "c"(24), "d"(0x534D4150), "D"(buffer));
		if (signature != 0x534D4150)
			return -1; // error
		if (bytes > 20 && (buffer->acpi_extended & 0x0001) == 0) {
			// ignore this entry
		} else {
			buffer++;
			entries++;
		}
	} while (contID != 0 && entries < maxentries);
	return entries;
}

void memory_report_e820(int map_addr, int map_size)
{
	map_entry_t* map = (map_entry_t*) map_addr;
	int entry_count = memory_detect(map, map_size / sizeof(map_entry_t));

	print("[+] Detecting memory...\r\n");

	if (entry_count == -1) {
		print("[!] Failed to get memory map! Fall back to another method!\r\n");
		return;
	} else {
		for (int i = 0; i < entry_count; i++) {
			print(" Index: %d Type: %d (%s) Address: 0x%x%x Length: 0x%x%x\r\n", i, map[i].type, memory_type_to_string(map[i].type), map[i].base_addr_high, map[i].base_addr_low, map[i].length_high, map[i].length_low);
		}
	}
}
