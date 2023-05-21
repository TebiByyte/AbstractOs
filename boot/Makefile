chainLoaderDir = chainloader
bootstrapDir = bootstrap
buildDir = ../build
outFile = $(buildDir)/image/os.hdd

bootsectorOut = $(bootstrapDir)/bootsector.bin
chainloaderOut = $(chainLoaderDir)/chainloader.bin

srcs = $(shell find $(chainLoaderDir) -name '*.c')
cinc = $(shell find $(chainLoaderDir) -name '*.h')
asm  = $(shell find $(chainLoaderDir) -name '*.asm')
cobjs = $(patsubst %.c, %.o, $(srcs))
asmobjs = $(patsubst %.asm, %.o, $(asm)) 

# The final step in the build process is concatenating the two binaries
$(outFile): $(bootsectorOut) $(chainloaderOut)
	mkdir -p $(buildDir)/image
	cat $(bootsectorOut) $(chainloaderOut) > $(outFile)
	truncate -s 32K $(outFile)

# Assemble the boot sector parts to a flat binary 
$(bootsectorOut): $(bootstrapDir)/bootsector.asm
	nasm $(bootstrapDir)/bootsector.asm -f bin -o $(bootsectorOut) 

# IMPORTANT!!!!: LINKING ORDER MATTERS! IT MUST TAKE IN THE ASSEMBLY OBJECTS *FIRST*
# Link all the .o files together with the linker script
$(chainloaderOut): $(asmobjs) $(cobjs)
	ld -o $(chainloaderOut) -T Linker -Ttext 0x7F00 $^ --oformat binary 

# Compile all the .c source files along with all the header files
%.o: %.c $(cinc) 
	gcc -m64 -I $(chainLoaderDir) -ffreestanding -fno-pie -fno-stack-protector -nostdlib -mno-red-zone -c $< -o $@

# Assemble all the assembly source files
%.o: %.asm
	nasm $< -f elf64 -o $@

# Clean
clean: 
	rm -f $(outFile) $(chainloaderOut) $(bootsectorOut) $(cobjs) $(asmobjs) *.bin *.o $(buildDir)/disassmbly.asm

# For debugging
disassmbly: 
	ndisasm -b 64 $(outFile) > $(buildDir)/disassmbly.asm
