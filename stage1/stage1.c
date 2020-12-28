/**
 * @file	stage1.c
 * @brief	uniboot - the universal bootloader project
 * @version	v1.0
 * @author	dogan c. karatas
 * @date	11/02/2020
 **/

/* Tell gcc to emit 16-bit code */
__asm__ (".code16gcc");

#include <stdint.h>
#include "common.h"

void stage1() __attribute__((section (".init_fn")));
int8_t seek_file();
uint8_t read_sectors();
void puts(char *);

/* Boot sector linker location mapping
 *  ______________________
 * |  3 BYTE jmp vector  |  ---
 * |---------------------|   |
 * |    BPB Structure    |   |
 * |---------------------|   |
 * |    void stage1()    |   |
 * |---------------------|   |
 * |    .text segment    |   |
 * |---------------------|   |
 * |   .rodata segment   |   |  = 512 byte or 1 sector
 * |---------------------|   |
 * |    .bss segment     |   |
 * |---------------------|   |
 * :                     :   |
 * :                     :   |
 * |---------------------|   |
 * | 2 BYTE magic 0xaa55 |   |
 * |_____________________|  ---
 */

struct boot_sector {
	uint8_t jump_vector[3];
	int8_t	oem_name[8];
	uint16_t bytes_per_sector;
	uint8_t sectors_per_cluster;
	uint16_t reserved_sector_count;
	uint8_t fat_count;
	uint16_t root_entry_count;
	uint16_t total_sector_count;
	uint8_t media_descriptor;
	uint16_t sectors_per_fat;
	uint16_t sectors_per_track;
	uint16_t head_count;
	uint32_t hidden_sector_count;
	uint32_t total_sector_count_2;
	uint16_t drive_index;
	uint8_t extended_boot_signature;
	uint32_t volume_id;
	int8_t drive_label[11];
	int8_t filesystem_type[8];
} __attribute__((packed)) const bpb __attribute__((section (".boot_sector"))) = {
	.jump_vector = {0xe9, sizeof(bpb) , 0x00},
	.filesystem_type = "FAT12   ",
	.drive_label = "UNIBOOTLDR",
	.oem_name = "UNIBOOT0",
	.bytes_per_sector = 512,
	.sectors_per_cluster = 1,
	.reserved_sector_count = 1,
	.fat_count = 2,
	.root_entry_count = 224,
	.total_sector_count = 2880,
	.media_descriptor = 0xf0,
	.sectors_per_fat = 9,
	.sectors_per_track = 18,
	.head_count = 2,
	.hidden_sector_count = 0,
	.total_sector_count_2 = 0,
	.drive_index = 0,
	.extended_boot_signature = 41,
	.volume_id = 0x00001b007
};

struct disk_drive {
	uint8_t sector;
	uint32_t lba;
};

struct file {
	int8_t name[8];
	int8_t extension[3]; /* MSDOS 8.3 filename format */
	uint8_t attributes;
	uint8_t __padding_1;
	uint8_t create_time_microseconds;
	uint16_t create_time;
	uint16_t create_date;
	uint16_t last_access_date;
	uint16_t __padding_2;
	uint16_t last_modified_time;
	uint16_t last_modified_date;
	uint16_t starting_cluster;
	uint32_t size;
} __attribute__((packed));

struct disk_drive *disk = (struct disk_drive *) 0x7f00;
uint8_t *buffer = (uint8_t *) 0x0500;
void (*stage2)(void) = (void (*)(void)) 0x0700;
struct file const *entry = 0;
char const *stage2_filename = "STAGE2  BIN";
uint8_t size;

void stage1()
{
	__asm__ volatile ("movl $0x7bff, %esp");
	puts("uniboot "VERSION"\r\n[boot]: stage1 loaded\r\n");
	__asm__ ("int $0x13" : "=c"(disk->sector) : "a"(0x0800), "d"(0x80) : "bx");
	disk->sector &= 0b00111111;

	disk->lba = bpb.reserved_sector_count + (bpb.fat_count * bpb.sectors_per_fat);
	size = bpb.root_entry_count * sizeof(struct file) / bpb.bytes_per_sector;
	/* root dir loaded into &buffer */
	read_sectors();

	/* seek stage2 in root */
	for (entry = (struct file *) buffer; ; ++entry) {
		if (seek_file() == 0) {
			/* load the first 8 sectors of  into memory at 0x0700 */
			buffer = (uint8_t *) stage2;
			disk->lba += size + (entry->starting_cluster - 2) * bpb.sectors_per_cluster;
			/* TODO: Seek until end of file, not predefined size */
			read_sectors();

			/* jump to stage2 */
			stage2();
		}
	}
}

int8_t seek_file()
{
	uint16_t i;
	for (i = 0; i < 10 && ((int8_t*) entry)[i] && ((int8_t*) entry)[i] == stage2_filename[i]; ++i);
	return ((int8_t*) entry)[i] - stage2_filename[i];
}

uint8_t read_sectors()
{
	/* lba to chs conversion */
	uint32_t track;
	uint16_t cylinder;
	uint16_t head;
	track = bpb.head_count * disk->sector;
	head = (disk->lba % track) / disk->sector;
	cylinder = disk->lba / track;
	cylinder <<= 16;
	cylinder |= ((disk->lba % track) % disk->sector) + 1;

	__asm__ ("int $0x13" : : "a"(0x0200 | size), "b"(buffer), "c"(cylinder), "d"((head << 8) | 0x0080));
	return 0;
}

void puts(char *s)
{
	for (; *s; ++s)
		__asm__ ("int $0x0010" : : "a"(0x0e00 | *s));
}