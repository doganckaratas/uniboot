# @file		stage2/Makefile
# @brief	uniboot - the universal bootloader project
# @version	v1.0
# @author	dogan c. karatas
# @date	29/01/2019

# required tools: gcc, ld, binutils, mtools, dd, qemu-system-i386, dosfstools

CFLAGS=-Wall -ffreestanding -I../include
OUTPUT=../bin
MAKE=make

.PHONY: all
all:	stage2

memory/memory.o: memory/memory.c
	$(CC) $(CFLAGS) -Os -m32 -c -o $@ $^

util/util.o: util/util.c
	$(CC) $(CFLAGS) -Os -m32 -c -o $@ $^

util/itoa.o: util/itoa.c
	$(CC) $(CFLAGS) -Os -m32 -c -o $@ $^

stage2.o: stage2.c
	$(CC) $(CFLAGS) -Os -m32 -c -o $@ $^

stage2: stage2.o memory/memory.o util/util.o util/itoa.o
	if [ ! -d $(OUTPUT) ]; then mkdir $(OUTPUT); fi;
	$(LD) $(LDFLAGS) -T stage2.ld -melf_i386 -o $@.elf $^
	objcopy -O binary $@.elf $(OUTPUT)/$@.bin

.PHONY: clean
clean:
	rm -rf stage2.o memory/memory.o util/util.o util/itoa.o stage2.elf
