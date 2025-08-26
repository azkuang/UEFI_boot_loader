#!/bin/bash

# Build with edk2
cd edk2
source ./edksetup.sh
.HelloWorld /edksetup.sh
build -p ../BootManagerPkg/BootManagerPkg.dsc

# Create boot image
cd ..
dd if=/dev/zero of=boot.img bs=1M count=64 2>/dev/null
mkfs.vfat boot.img > /dev/null

# Mount and copy the image
mkdir -p mnt
sudo mount -o loop boot.img mnt

if [ ! -d "mnt" ] || ! mountpoint -q mnt; then
    echo "Mount failed"
    exit 1
fi

sudo mkdir -p mnt/EFI/BOOT
sudo cp edk2/Build/BuildPkg/DEBUG_GCC5/X64/BOOTX64.efi mnt/EFI/BOOT/BOOTX64.EFI
sudo umount mnt

# Run qemu
qemu-system-x86_64 -bios ./OVMF.fd -drive file=boot.img,format=raw
