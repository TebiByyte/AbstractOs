BUILDDIR = build
BOOTSECTO = $(BUILDDIR)/boot.bin
OSOUT = $(BUILDDIR)/os.bin
SRCS = $(shell find . -name '*.c')
CINC = $(shell find . -name '*.h')
COBJS = $(patsubst %.c, %.o, $(SRCS))

$(OSOUT): kernel.bin $(BOOTSECTO)
	cat $(BOOTSECTO) kernel.bin > $(OSOUT)
	truncate -s 14M $(OSOUT) 

$(BOOTSECTO): boot/bootsector.asm
	nasm -f bin boot/bootsector.asm -o $(BOOTSECTO)

%.o:%.c $(CINC)
	gcc -m64 -ffreestanding -fno-pie -fno-stack-protector -nostdlib -c $< -o $@

kernelEntry.o: boot/kernelEntry.asm
	nasm boot/kernelEntry.asm -f elf64 -o kernelEntry.o

kernel.bin: kernelEntry.o $(COBJS)
	ld -o kernel.bin -T Linker -Ttext 0x7F00 $^ --oformat binary

run: 
	qemu-system-x86_64 $(OSOUT)

clean: 
	rm -f $(BOOTSECTO) $(OSOUT) *.bin *.o $(COBJS) $(BUILDDIR)/hexdump.txt

dump: 
	hexdump -C $(OSOUT) > build/hexdump.txt