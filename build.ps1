# build.ps1 — сборка MicroX OS

# Папка для бинарников
$compiled = "compiled"
if (!(Test-Path $compiled)) {
    New-Item -ItemType Directory -Path $compiled | Out-Null
}

Write-Host "==> Сборка загрузчика"
nasm -f bin boot\boot.asm -o $compiled\boot.bin

Write-Host "==> Компиляция ядра и драйверов"
i386-elf-gcc -ffreestanding -c kernel\kernel.c -o $compiled\kernel.o -I kernel
i386-elf-gcc -ffreestanding -c kernel\interrupts.c -o $compiled\interrupts.o -I kernel
i386-elf-gcc -ffreestanding -c drivers\mouse.c -o $compiled\mouse.o -I kernel
i386-elf-gcc -ffreestanding -c kernel\io.c -o $compiled\io.o -I kernel
i386-elf-gcc -ffreestanding -c kernel\print.c -o $compiled\print.o -I kernel
nasm -f elf32 kernel\interrupts.asm -o $compiled\interrupts_asm.o

Write-Host "==> Линковка ядра"
i386-elf-ld -T boot\linker.ld -o $compiled\kernel.bin `
    $compiled\kernel.o $compiled\interrupts.o $compiled\mouse.o `
    $compiled\io.o $compiled\print.o $compiled\interrupts_asm.o

Write-Host "==> Склейка загрузчика и ядра"
copy /b $compiled\boot.bin + $compiled\kernel.bin $compiled\os.bin

Write-Host "==> Создание ISO"
$isoPath = "$compiled\iso\boot\grub"
if (!(Test-Path $isoPath)) {
    New-Item -ItemType Directory -Path $isoPath -Force | Out-Null
}
copy $compiled\os.bin $compiled\iso\boot\os.bin

@"
set timeout=0
set default=0

menuentry "MicroX" {
    multiboot /boot/os.bin
}
"@ | Out-File -Encoding ASCII $compiled\iso\boot\grub\grub.cfg

grub-mkrescue -o $compiled\MicroX.iso $compiled\iso

Write-Host "==> Запуск в QEMU"
qemu-system-i386 -cdrom $compiled\MicroX.iso
