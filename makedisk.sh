#!/bin/bash
outFile=$1
imageSize=$2

cat stage2.bin stage3.bin > bootpart.bin 
partsize=$(du -h bootpart.bin | cut -f 1)
dd if=/dev/zero of=$outFile bs=$imageSize count=1 
dd if=stage1.bin of=$outFile seek=0 conv=notrunc 
echo "size=${partsize}, name="bootpart"" | sfdisk $outFile --label gpt 
dd if=bootpart.bin of=$outFile seek=34 conv=notrunc 
