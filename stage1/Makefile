# @file		stage1/Makefile
# @brief	uniboot - the universal bootloader project
# @version	v1.0
# @author	dogan c. karatas
# @date	11/02/2020

# required tools: gcc, ld, binutils, mtools, dd, qemu-system-i386, dosfstools

CC=i686-elf-gcc
LD=i686-elf-ld
CFLAGS=-Wall -std=c99 -ffreestanding -I../include
OUTPUT=../bin

.PHONY: all
all:	stage1

stage1.o: stage1.c
	$(CC) $(CFLAGS) -Os -m32 -c -o $@ $^

stage1: stage1.o
	if [ ! -d $(OUTPUT) ]; then mkdir $(OUTPUT); fi;
	$(LD) $(LDFLAGS) -T stage1.ld -melf_i386 -o $@.elf stage1.o
	objcopy -O binary $@.elf $(OUTPUT)/$@.bin

.PHONY: clean
clean:
	rm -rf stage1.o stage1.elf 
