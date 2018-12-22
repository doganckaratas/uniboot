CC		= nasm
CFLAGS		+= -O0 -w+orphan-labels -f bin 
CFLAGS		+= -i./include/
SRC		= stage1.asm stage2.asm
TARGET		= $(SRC:.asm=.bin)
IMAGE		= disk.img

.PHONY: all
all: assemble image

.PHONY:	assemble
assemble: $(TARGET)

%.bin: %.asm
	$(CC) $(CFLAGS) -o $@ $<

.PHONY: image
image:
	@touch $(IMAGE)
	@rm $(IMAGE)
	@mkdosfs -C $(IMAGE) 1440
# mtools is a good utility for FAT formatted mediums
# mdir, mdel, mcopy, etc..
	@mcopy -i $(IMAGE) stage2.bin ::/stage2.bin
	@dd status=none conv=notrunc if=stage1.bin of=$(IMAGE)

# not necessary to mount fs because I've using mtools
.PHONY: mount
mount: all
	@sudo losetup /dev/loop0 $(IMAGE)

# ditto
.PHONY: umount
umount:
	@sudo losetup -d /dev/loop0 > /dev/null 2>&1

.PHONY: boot
boot: all
	@qemu-system-i386 -soundhw pcspk -hda $(IMAGE) -serial mon:stdio

.PHONY: clean
clean:
	@rm -rf $(IMAGE) $(TARGET)
