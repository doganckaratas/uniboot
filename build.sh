#!/bin/sh

echo ">>> Creating Floppy Image..."

touch boot.img
rm -rf boot.img
mkdosfs -C boot.img 1440

echo ">>> Assembling bootloader..."

nasm -O0 -w+orphan-labels -f bin -o boot.bin boot.asm 

echo ">>> Writing bootloader..."

dd status=none conv=notrunc if=boot.bin of=boot.img

echo '>>> Done!'

