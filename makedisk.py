#Build steps (OLD):
#Make a new directory build/image
#concatenate stage2 binary and stage 3 binary
#Calculate the partition size (In bytes)
#Fill the file with zeros as big as the imagesize variable specifies
#Copy stage1 binary to the first sector of the image
#Build a new partition template file to give to sfdisk
#Use sfdisk to create the partition table from the template
#Copy the concatenated stage 2 and 3 binary to the new partition on disk

#Build steps (NEW):
#Create necessary directories
#Concatenate stage 2 and 3
#Generate the template that gets passed to sfdisk
# - get the size of the combined stage 2 and 3 to determine the start LBA of the file system
#Create the disk file filled with 0 of a certain size
#Use sfdisk with the format file that was generated
#... build file system...
#Use the command mkfs.ext2 -E $(sectorOffset * 512) -d <path/to/directory> 

import os
import math
import uuid

imagePath = "build/image/os.hdd"
templatePath = "disk_fmt.fmt"
bootPartStart = 34
diskSize = 2048

#Note about last-lba: the GPT is repeated at the end of the volume, so it will be the image size in sectors - 34
diskFmtHeader = '''label: gpt
label-id: 6B1E4E99-0921-4630-80C4-CCADC0A5B75B
device: {imagef}
unit: sectors
first-lba: {start}
last-lba: {size}
grain: 512
sector-size: 512\n'''

partTemplate = "{fileName}{partitionNum} : start=          {start}, size=          {size}, type={typeUuid}, uuid={uuid}, name=\"{name}\"\n"

biosBootGUID = "21686148-6449-6E6F-744E-656564454649"
fsGUID = "0FC63DAF-8483-4772-8E79-3D69D8477DE4"

def generateUUID():
    return str(uuid.uuid4())

def generate_disk_fmt(bootPartSize):
    header = diskFmtHeader.format(imagef=imagePath,start=str(bootPartStart),size=str(diskSize - 34))
    partition1 = partTemplate.format(fileName=imagePath,partitionNum="1",start="34",size=str(bootPartSize),typeUuid=biosBootGUID,uuid=generateUUID(),name="bootpart")

    with open(templatePath, "w") as image:
        image.write(header)
        image.write(partition1)



def main():
    bootPartSize = os.path.getsize("stage2.bin") + os.path.getsize("stage3.bin") #TODO make these CL variables
    sizeSectors = math.ceil(bootPartSize / 1024) * 2

    generate_disk_fmt(sizeSectors)



main()
