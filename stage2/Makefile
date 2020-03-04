# @file		stage2/Makefile
# @brief	uniboot - the universal bootloader project
# @version	v1.0
# @author	dogan c. karatas
# @date	11/02/2020

# required tools: gcc, ld, binutils, mtools, dd, qemu-system-i386, dosfstools

CC=i686-elf-gcc
LD=i686-elf-ld
CFLAGS=-std=gnu99 -ffreestanding -O0 -Wall -I../include
OUTPUT=../bin
MAKE=make

.PHONY: all
all:	stage2

stage2.o: stage2.c
	$(CC) $(CFLAGS) -Os -m32 -c -o $@ $^

util.o: util.c
	$(CC) $(CFLAGS) -Os -m32 -c -o $@ $^

tty.o: tty.c
	$(CC) $(CFLAGS) -Os -m32 -c -o $@ $^

memory.o: memory.c
	$(CC) $(CFLAGS) -Os -m32 -c -o $@ $^

serial.o: serial.c
	$(CC) $(CFLAGS) -Os -m32 -c -o $@ $^

vga.o: vga.c
	$(CC) $(CFLAGS) -Os -m32 -c -o $@ $^

pmode.o: pmode.c
	$(CC) $(CFLAGS) -Os -m32 -c -o $@ $^

cpu.o: cpu.c
	$(CC) $(CFLAGS) -Os -m32 -c -o $@ $^

stage2: stage2.o util.o tty.o memory.o serial.o vga.o pmode.o cpu.o
	if [ ! -d $(OUTPUT) ]; then mkdir $(OUTPUT); fi;
	$(LD) $(LDFLAGS) -T stage2.ld -melf_i386 -o $@.elf $^
	objcopy -O binary $@.elf $(OUTPUT)/$@.bin

.PHONY: clean
clean:
	rm -rf stage2.elf stage2.o util.o tty.o memory.o serial.o vga.o pmode.o cpu.o
