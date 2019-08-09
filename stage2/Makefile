# @file		stage2/Makefile
# @brief	uniboot - the universal bootloader project
# @version	v1.0
# @author	dogan c. karatas
# @date	29/01/2019

# required tools: gcc, ld, binutils, mtools, dd, qemu-system-i386, dosfstools

CFLAGS=-Wall -ffreestanding -I../include
OUTPUT=../bin

.PHONY: all
all:	stage2

stage2.o: stage2.c
	$(CC) $(CFLAGS) -Os -m32 -c -o $@ $^

stage2: stage2.o
	if [ ! -d $(OUTPUT) ]; then mkdir $(OUTPUT); fi;
	$(LD) $(LDFLAGS) -T stage2.ld -melf_i386 -o $@.elf stage2.o
	objcopy -O binary $@.elf $(OUTPUT)/$@.bin

.PHONY: clean
clean:
	rm -rf stage2.o stage2.elf
