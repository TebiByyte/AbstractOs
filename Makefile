BUILDDIR = build
BOOTSECTO = $(BUILDDIR)/boot/boot.bin
OSOUT = $(BUILDDIR)/image/os.iso
SRCS = $(shell find . -name '*.c')
CINC = $(shell find . -name '*.h')
COBJS = $(patsubst %.c, %.o, $(SRCS))

$(OSOUT): chainloader.bin $(BOOTSECTO)
	cat $(BOOTSECTO) chainloader.bin > $(OSOUT)
	truncate -s 14M $(OSOUT) 

$(BOOTSECTO): boot/bootsector.asm
	nasm -f bin boot/bootsector.asm -o $(BOOTSECTO)

%.o:%.c $(CINC)
	gcc -m64 -I kernel -ffreestanding -fno-pie -fno-stack-protector -nostdlib -mno-red-zone -c $< -o $@

chainloaderEntry.o: boot/chainloaderEntry.asm
	nasm boot/chainloaderEntry.asm -f elf64 -o chainloaderEntry.o

chainloader.bin: chainloaderEntry.o $(COBJS)
	ld -o chainloader.bin -T Linker -Ttext 0x7F00 $^ --oformat binary

run: 
	qemu-system-x86_64 -m 256M -hda $(OSOUT)

clean: 
	rm -f $(BOOTSECTO) $(OSOUT) *.bin *.o $(COBJS) $(BUILDDIR)/hexdump.txt

dump: 
	hexdump -C $(OSOUT) > build/hexdump.txt