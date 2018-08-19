#!/bin/sh

echo ">>> Creating Floppy Image..."

touch boot.img
rm -rf boot.img
mkdosfs -C boot.img 1440

echo ">>> Assembling bootloader..."

nasm -O0 -w+orphan-labels -f bin -o stage1.bin stage1.asm 
nasm -O0 -w+orphan-labels -f bin -o stage2.bin stage2.asm 

echo ">>> Writing bootloader..."

dd status=none conv=notrunc if=stage1.bin of=boot.img

echo '>>> Done!'
