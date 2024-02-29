SHELL=/bin/bash
#Assemble stage1 
#Write to the image files boot sector
#Create a partition table using sfdisk
#Create a new partition starting from LBA 34 - LBA x 
#Assemble stage 2 and compile stage 3
#Write stage 2 to the boot partition
#Write stage 3 directly after it

outFile = $(buildDir)/image/os.hdd
bootstrapDir = boot/bootstrap
stage3dir = boot/stage3
buildDir = build
outFile = $(buildDir)/image/os.hdd
includes = include
nasmBinArgs = -I $(bootstrapDir) -f bin 
imageSize = 1M
bootPreprocessorFlags = COMPILE_NO_FLOAT
compileFlags = -ffreestanding -fno-pie -fno-stack-protector -nostdlib -mno-red-zone -mgeneral-regs-only -D $(bootPreprocessorFlags)

srcs = $(shell find $(stage3dir) -name '*.c') $(shell find $(includes) -name '*.c')
cinc = $(shell find $(stage3dir) -name '*.h') $(shell find $(includes) -name '*.h')
asm  = $(shell find $(stage3dir) -name '*.asm')

cobjs = $(patsubst %.c, %.o, $(srcs))
asmobjs = $(patsubst %.asm, %.o, $(asm)) 

disk: stage1 stage2 stage3
	./makedisk.sh $(outFile) $(imageSize)

stage1: $(bootstrapDir)/stage1.asm
	nasm $^ $(nasmBinArgs) -o $@.bin
# Change the output directory to something else

stage2: $(bootstrapDir)/stage2.asm
	nasm $^ $(nasmBinArgs) -o $@.bin

stage3: $(asmobjs) $(cobjs)
	ld -o $@.bin -T boot/Linker -Ttext 0xB000 $^ --oformat binary 

%.o: %.c $(cinc) 
	gcc -m64 -I $(stage3dir) -I $(includes) $(compileFlags) -c $< -o $@

# Assemble all the assembly source files
%.o: %.asm
	nasm $< -f elf64 -o $@

run-qemu:
	qemu-system-x86_64 -hda $(outFile)

clean: 
	rm -f $(outFile) stage1.bin stage2.bin stage3.bin $(cobjs) $(asmobjs) *.bin *.o $(buildDir)/disassmbly.asm

# For debugging
disassmbly: 
	ndisasm -b 64 $(outFile) > $(buildDir)/disassmbly.asm