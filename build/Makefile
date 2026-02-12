ASM = nasm
CC = i386-elf-gcc
LD = i386-elf-ld

all: os.bin

boot.o: boot/boot.asm
    $(ASM) -f bin boot/boot.asm -o boot.o

kernel.o: kernel/kernel.c kernel/interrupts.c drivers/mouse.c
    $(CC) -ffreestanding -c kernel/kernel.c -o kernel.o
    $(CC) -ffreestanding -c kernel/interrupts.c -o interrupts.o
    $(CC) -ffreestanding -c drivers/mouse.c -o mouse.o

os.bin: boot.o kernel.o interrupts.o mouse.o
    $(LD) -T boot/linker.ld -o kernel.bin kernel.o interrupts.o mouse.o
    cat boot.o kernel.bin > os.bin

run:
    qemu-system-i386 -cdrom os.bin

clean:
	rm -rf os.bin boot.o kernel.o interrupts.o mouse.o