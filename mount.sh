#!/bin/bash

echo ">>> Mounting image..."

sudo losetup -D
sudo losetup /dev/loop0 boot.img
