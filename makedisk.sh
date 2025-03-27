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

#Build steps:
#Make a new directory build/image
#concatenate stage2 binary and stage 3 binary
#Calculate the partition size (In bytes)
#Fill the file with zeros as big as the imagesize variable specifies
#Copy stage1 binary to the first sector of the image
#Build a new partition template file to give to sfdisk
#Use sfdisk to create the partition table from the template
#Copy the concatenated stage 2 and 3 binary to the new partition on disk
#... build file system...
#Use the command mkfs.ext2 -E $(sectorOffset * 512) -d <path/to/directory> 

