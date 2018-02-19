#!/bin/sh

echo ">>> Assembling bootloader..."

nasm -O0 -w+orphan-labels -f bin -o boot.bin boot.asm 

echo '>>> Done!'

qemu-system-i386 -soundhw pcspk -fda boot.bin 
