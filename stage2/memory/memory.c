typedef struct SMAP_entry {

	uint32_t BaseL; // base address uint64_t
	uint32_t BaseH;
	uint32_t LengthL; // length uint64_t
	uint32_t LengthH;
	uint32_t Type; // entry Type
	uint32_t ACPI; // extended

}__attribute__((packed)) SMAP_entry_t;

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
int __attribute__((noinline)) __attribute__((regparm(3))) detectMemory(SMAP_entry_t* buffer, int maxentries)
{
	uint32_t contID = 0;
	int entries = 0, signature, bytes;
	do
	{
		__asm__ __volatile__ ("int  $0x15"
				: "=a"(signature), "=c"(bytes), "=b"(contID)
				: "a"(0xE820), "b"(contID), "c"(24), "d"(0x534D4150), "D"(buffer));
		if (signature != 0x534D4150)
			return -1; // error
		if (bytes > 20 && (buffer->ACPI & 0x0001) == 0)
		{
			// ignore this entry
		}
		else {
			buffer++;
			entries++;
		}
	}
	while (contID != 0 && entries < maxentries);
	return entries;
}

void memory_report_e820(int map_addr, int map_size)
{
	SMAP_entry_t* smap = (SMAP_entry_t*) map_addr;
	const int smap_size = map_size;
	int entry_count = detectMemory(smap, smap_size / sizeof(SMAP_entry_t));

	print("[+] Detecting memory...\r\n");

	if (entry_count == -1) {
		print("[!] Failed to get memory map! Fall back to another method!\r\n");
		return;
	} else {
		for (int i = 0; i < entry_count; i++) {
			print(" Index: %d Type: %d (%s) Address: 0x%x%x Length: 0x%x%x\r\n", i, smap[i].Type, memory_type_to_string(smap[i].Type), smap[i].BaseH, smap[i].BaseL, smap[i].LengthH, smap[i].LengthL);
		}
	}
}
