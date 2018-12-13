CC	= nasm
CFLAGS	+= -O0 -w+orphan-labels -f bin 
SRC	+= boot.asm
TARGET	= $(SRC:.asm=.bin)

.PHONY: all
all: assemble image

.PHONY:	assemble
assemble: $(TARGET)

$(TARGET): $(SRC)
	$(CC) $(CFLAGS) $^ -o $@

.PHONY: image
image:
	@touch boot.img
	@rm boot.img
	@mkdosfs -C boot.img 1440
	@dd status=none conv=notrunc if=boot.bin of=boot.img

.PHONY: mount
mount: all
	@sudo losetup /dev/loop0 boot.img

.PHONY: umount
umount:
	@sudo losetup -d /dev/loop0 > /dev/null 2>&1

.PHONY: boot
boot: all
	@qemu-system-i386 -soundhw pcspk -fda boot.img -monitor stdio

.PHONY: clean
clean: 
	@rm -rf *.img *.bin
