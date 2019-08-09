# @file	stage1.c
# @brief	uniboot - the universal bootloader project
# @version	v1.0
# @author	dogan c. karatas
# @date	29/01/2019

# required tools: gcc, ld, binutils, mtools, dd, qemu-system-i386, dosfstools

CFLAGS=-Wall -ffreestanding -I./include
OUTPUT=./bin
IMAGE=disk.img
MAKE=make

.PHONY: all
all:	disk

.PHONY: clean
clean:
	$(MAKE) -C stage1 clean V=1
	$(MAKE) -C stage2 clean V=1
	rm -rf *~ $(OUTPUT) disk.img

.PHONY: stage1
stage1:
	$(MAKE) -C stage1 all V=1

.PHONY: stage2
stage2:
	$(MAKE) -C stage2 all V=1

.PHONY: disk
disk: stage1 stage2
	dd if=/dev/zero of=$(IMAGE) bs=1 count=2880
	if [ -f $(IMAGE) ]; then rm $(IMAGE); fi;
	mkdosfs -C $(IMAGE) 1440
	dd if=$(OUTPUT)/stage1.bin of=$(IMAGE) bs=1 count=512 conv=notrunc
	mcopy -i $(IMAGE) $(OUTPUT)/stage2.bin ::/STAGE2.BIN

.PHONY: boot
boot: all
	qemu-system-i386 -drive media=disk,format=raw,file=$(IMAGE)
