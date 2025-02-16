CC = gcc
LD = ld
AS = nasm
CFLAGS = -ffreestanding -m32 -nostdlib -fno-pic
LDFLAGS = -T linker.ld -m elf_i386

all: kernel.iso

kernel.bin: boot.o kernel.o screen.o
	$(LD) $(LDFLAGS) -o kernel.bin boot.o kernel.o screen.o

boot.o: boot.asm
	$(AS) -f elf boot.asm -o boot.o

kernel.o: kernel.c
	$(CC) $(CFLAGS) -c kernel.c -o kernel.o

screen.o: screen.c
	$(CC) $(CFLAGS) -c screen.c -o screen.o

kernel.iso: kernel.bin
	mkdir -p iso/boot/grub
	cp kernel.bin iso/boot/kernel.bin
	echo 'set timeout=0' > iso/boot/grub/grub.cfg
	echo 'set default=0' >> iso/boot/grub/grub.cfg
	echo 'menuentry \"MyOS\" {' >> iso/boot/grub/grub.cfg
	echo '    multiboot /boot/kernel.bin' >> iso/boot/grub/grub.cfg
	echo '}' >> iso/boot/grub/grub.cfg
	grub-mkrescue -o kernel.iso iso

run: kernel.iso
	qemu-system-i386 -cdrom kernel.iso

clean:
	rm -rf *.o *.bin iso kernel.iso
