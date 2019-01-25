CFLAGS=-Wall

all:	disk

clean:
	@rm -rf *~ *.o *.elf stage1.bin stage2.bin disk.img

stage1.o: stage1.c
	$(CC) $(CFLAGS) -Os -m32 -c -o $@ $^

stage2.o: stage2.c
	$(CC) $(CFLAGS) -O -fno-builtin -m32 -c -o $@ $^

stage1: stage1.o
	$(LD) $(LDFLAGS) -T stage1.ld -melf_i386 -o $@.elf $^
	@objcopy -O binary $@.elf $@.bin

stage2: stage2.o
	$(LD) $(LDFLAGS) -T stage2.ld -melf_i386 -o $@.elf $^
	@objcopy -O binary $@.elf $@.bin

disk: stage1 stage2
	@dd if=/dev/zero of=disk.img bs=512 count=20160
	@if [ -f disk.img ]; then rm disk.img; fi;
	@mkdosfs -C disk.img 1440
	@dd if=stage1.bin of=disk.img bs=1 count=3 conv=notrunc
	@dd if=stage1.bin of=disk.img skip=36 seek=36 bs=1 count=1 conv=notrunc
	@dd if=stage1.bin of=disk.img skip=62 seek=62 bs=1 count=450 conv=notrunc
	@mcopy -i disk.img stage2.bin ::/STAGE2.BIN

boot:
	@qemu-system-i386 disk.img
