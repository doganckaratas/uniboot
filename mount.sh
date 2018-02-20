#!/bin/bash

echo ">>> Mounting image..."

losetup -d /dev/loop0
losetup /dev/loop0 boot.img
