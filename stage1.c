#include "stdtypes.h"

asm(".code16gcc");

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
} __attribute__((packed)) bios_parameter_block __attribute__((section (".data1"))) = {
	.jump_vector = {0xe9, 0x3c, 0x00},
	.filesystem_type = "FAT12   ",
	.drive_label = "POWERLOADER",
	.oem_name = "POWERLDR",
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
	.volume_id = 0x0d0a0ca00
};

struct partition {
	struct boot_sector volume_info_fat12;
	uint8_t boot_code[448];
	uint16_t boot_signature;
} __attribute__((packed));

struct drive {
	uint8_t sector;
	uint32_t lba;
};

/* directory entry struct */
struct file {
	int8_t name[8];
	int8_t ext[3]; /* MSDOS 8.3 filename format */
	uint8_t attr;
	uint8_t __reserved_1;
	uint8_t create_time_microseconds;
	uint16_t create_time;
	uint16_t create_date;
	uint16_t last_access_date;
	uint16_t __reserved_2;
	uint16_t last_modified_time;
	uint16_t last_modified_date;
	uint16_t starting_cluster;
	uint32_t size;
} __attribute__((packed));

struct boot_sector *boot = (struct boot_sector *) 0x7c00;
struct drive *disk_info = (struct drive *) 0x7e00;
struct file const *entry = 0;
char const* stage2_filename = "STAGE2  BIN";
uint8_t *buffer = 0;
uint8_t size;

void stage1()
{
	puts("test ");
	puts(boot->filesystem_type);
} __attribute__((section (".load")));

void putchar(char c)
{
  asm ("int $0x0010" : : "a"(0x0e00 | c));
}

void puts(char* s)
{
  for (; *s; ++s)
    putchar(*s);
}

//
// #define u8_t uint8_t
// #define u16_t uint16_t
// #define s8_t int8_t
// #define s16_t int16_t
// #define u32_t uint32_t
//
// /**
//  * Hard disk drive information.
//  * @param sectors The number of sectors on the disk.
//  * @param lba     The LBA of the block to read from the hard disk drive.
//  */
// typedef struct
// {
//   u8_t  sectors;
//   u32_t lba;
// } FILE;
//
// /**
//  * A root directory entry.
//  * @param filename           The file name.
//  * @param extension          The file extension.
//  * @param attributes         File attributes.
//  * @param _a                 Reserved.
//  * @param create_time_us     The microsecond value of the creation time.
//  * @param create_time        The creation time.
//  * @param create_date        The creation date.
//  * @param last_access_date   The date the file was last accessed.
//  * @param _b                 Reserved.
//  * @param last_modified_time The time the file was last modified.
//  * @param last_modified_date The date the file was last modified.
//  * @param cluster            The cluster containing the start of the file.
//  * @param size               The file size in bytes.
//  */
// typedef struct
// {
//   s8_t  filename[8];
//   s8_t  extension[3];
//   u8_t  attributes;
//   u8_t  _a;
//   u8_t  create_time_us;
//   u16_t create_time;
//   u16_t create_date;
//   u16_t last_access_date;
//   u8_t  _b[2];
//   u16_t last_modified_time;
//   u16_t last_modified_date;
//   u16_t cluster;
//   u32_t size;
// } __attribute__ ((packed)) entry_t;
//
// /**
//  * A pointer to the boot sector. After the BIOS POST, the boot sector is loaded
//  * into memory at 0x7c00.
//  */
// struct boot_sector * _bs = (struct boot_sector*) 0x7c00;
//
// /**
//  * A pointer to the hard disk drive information. We use 0x7e00, the first block
//  * of available memory next to the boot sector.
//  */
// FILE* _disk = (FILE*) 0x7e00;
//
// /**
//  * A constant containing the name of the binary used to initialize the system
//  * and device drivers. This is typically IO.SYS or IBMBIO.COM.
//  */
// s8_t const* _io_bin = "STAGE2  BIN";
//
// /**
//  * A pointer to a general-purpose buffer in memory. Used to store the root
//  * directory entries and the first 3 sectors of IO.SYS.
//  */
// u8_t* _buffer;
//
// /**
//  * A variable used by the read() method to indicate how many sectors to read
//  * from the hard disk into memory.
//  */
// u8_t _size;
//
// /**
//  * A pointer to the root directory entry currently being read.
//  */
// entry_t const* _entry;
//
// /**
//  * Checks if the current root directory entry has the name IO.SYS by performing
//  * a string comparison.
//  * @return 0 if the current root directory entry has the name IO.SYS, a
//  *     negative value if the entry name is less than IO.SYS, or a positive
//  *     value if the entry name is greater than IO.SYS.
//  */
// s8_t
// iosyscmp()
// {
//   u16_t i;
//   for (i = 0;
//        i < 10 && ((s8_t*) _entry)[i] && ((s8_t*) _entry)[i] == _io_bin[i];
//        ++i);
//   return ((s8_t*) _entry)[i] - _io_bin[i];
// }
//
// /**
//  * Reads sectors at the provided LBA from the hard disk into memory. Uses the
//  * value stored in _size as the number of sectors to read.
//  * @return 0
//  */
// u16_t
// read()
// {
//   // convert the LBA into CHS values
//   u32_t t = _bs->head_count * _disk->sectors;
//   u16_t c = _disk->lba / t;
//   u16_t h = (_disk->lba % t) / _disk->sectors;
//   c <<= 8;
//   c |= ((_disk->lba % t) % _disk->sectors) + 1;
//
//   // read sectors from the hard disk into memory at 0x0500
//   asm ("int $0x13" : : "a"(0x0200 | _size), "b"(_buffer), "c"(c), "d"((h << 8) | 0x0080));
//   return 0;
// }
//
// /**
//  * Reads drive information, searches for IO.SYS, loads the first 3 sectors into
//  * memory, and begins executing it.
//  * @return 0
//  */
// u16_t
// _start()
// {
//   // read hard disk drive information into memory at 0x7e00
//   asm ("int $0x13" : "=c"(_disk->sectors) : "a"(0x0800), "d"(0x80) : "bx");
//   _disk->sectors &= 0b00111111;
//
//   // read the root directory into memory at 0x500
//   _buffer = (u8_t*) 0x0500;
//   _disk->lba = _bs->reserved_sector_count + (_bs->fat_count * _bs->sectors_per_fat);
//   _size = _bs->root_entry_count * sizeof(entry_t) / _bs->bytes_per_sector;
//   read();
//
//   // iterate over root directory entries and look for IO.SYS
//   for (_entry = (entry_t*) _buffer; ; ++_entry)
//     if (iosyscmp() == 0) {
//       // load the first 3 sectors of IO.SYS into memory at 0x0700
//       _buffer = (u8_t*) 0x0700;
//       _disk->lba += _size + (_entry->cluster - 2) * _bs->sectors_per_cluster;
//       _size = 3;
//       read();
//
//       // execute IO.SYS
//       asm ("jmpw %0, %1" : : "g"(0x0000), "g"(0x0700));
//     }
//
//   // read hard disk drive information into memory at 0x7e00
//   asm ("int $0x13" : "=c"(_disk->sectors) : "a"(0x0800), "d"(0x80) : "bx");
//   _disk->sectors &= 0b00111111;
//
//   // read the root directory into memory at 0x500
//   _buffer = (u8_t*) 0x0500;
//   _disk->lba = _bs->reserved_sector_count + (_bs->fat_count * _bs->sectors_per_fat);
//   _size = _bs->root_entry_count * sizeof(entry_t) / _bs->bytes_per_sector;
//   read();
//
//   // iterate over root directory entries and look for IO.SYS
//   for (_entry = (entry_t*) _buffer; ; ++_entry)
//     if (iosyscmp() == 0) {
//       // load the first 3 sectors of IO.SYS into memory at 0x0700
//       _buffer = (u8_t*) 0x0700;
//       _disk->lba += _size + (_entry->cluster - 2) * _bs->sectors_per_cluster;
//       _size = 3;
//       read();
//
//       // execute IO.SYS
//       asm ("jmpw %0, %1" : : "g"(0x0000), "g"(0x0700));
//     }
//
//   return 0;
// }
