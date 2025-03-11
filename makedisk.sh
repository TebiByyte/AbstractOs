#!/bin/bash
#Consider using a layout.json file instead. sfdisk is capable of this
#https://stackoverflow.com/questions/12150116/how-to-script-sfdisk-or-parted-for-multiple-partitions
#Use mkfs.ext2 -E $(sectorOffset * 512) -d <path/to/directory> to format the partition and create the files
outFile=$1
imageSize=$2

mkdir -p build/image/
cat stage2.bin stage3.bin > bootpart.bin 
partsize=$(du -h bootpart.bin | cut -f 1)
dd if=/dev/zero of=$outFile bs=$imageSize count=1 
dd if=stage1.bin of=$outFile seek=0 conv=notrunc 
echo "size=${partsize}, name="bootpart"" | sfdisk $outFile --label gpt 
dd if=bootpart.bin of=$outFile seek=34 conv=notrunc 
