BUILDDIR = build
BOOTSECTO = $(BUILDDIR)/boot.bin
OSOUT = $(BUILDDIR)/os.bin
SRCS = $(shell find . -name '*.c')
CINC = $(shell find . -name '*.h')
COBJS = $(patsubst %.c, %.o, $(SRCS))

$(OSOUT): chainloader.bin $(BOOTSECTO)
	cat $(BOOTSECTO) chainloader.bin > $(OSOUT)
	truncate -s 14M $(OSOUT) 

$(BOOTSECTO): boot/bootsector.asm
	nasm -f bin boot/bootsector.asm -o $(BOOTSECTO)

%.o:%.c $(CINC)
	gcc -m64 -I os -ffreestanding -fno-pie -fno-stack-protector -nostdlib -mno-red-zone -c $< -o $@

pic.o: os/drivers/pic.asm
	nasm os/drivers/pic.asm -f elf64 -o pic.o

chainloaderEntry.o: boot/chainloaderEntry.asm
	nasm boot/chainloaderEntry.asm -f elf64 -o chainloaderEntry.o

chainloader.bin: chainloaderEntry.o pic.o $(COBJS)
	ld -o chainloader.bin -T Linker -Ttext 0x7F00 $^ --oformat binary

run: 
	qemu-system-x86_64 -m 256M $(OSOUT)

clean: 
	rm -f $(BOOTSECTO) $(OSOUT) *.bin *.o $(COBJS) $(BUILDDIR)/hexdump.txt

dump: 
	hexdump -C $(OSOUT) > build/hexdump.txt