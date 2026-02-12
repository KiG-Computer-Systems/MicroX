; boot.asm - BIOS bootloader for x86 Legacy (CD-ROM boot)
; This code loads the kernel from CD-ROM using BIOS INT 13h extensions.

BITS 16
ORG 0x7C00

start:
    ; Set up stack
    xor ax, ax
    mov ss, ax
    mov sp, 0x7C00

    ; Print message
    mov si, msg
    call print_string

    ; Use INT 13h extensions to read kernel sector from CD-ROM
    mov ah, 0x42            ; Extended Read
    mov dl, 0xE0            ; Drive number (CD-ROM, BIOS usually assigns E0h+)
    mov si, dap             ; Disk Address Packet
    int 0x13
    jc disk_error

    ; Jump to kernel loaded at 0x8000
    jmp 0x0000:0x8000

disk_error:
    mov si, err
    call print_string
    hlt

print_string:
    lodsb
    or al, al
    jz .done
    mov ah, 0x0E
    int 0x10
    jmp print_string
.done:
    ret

msg db "Loading kernel from CD...",0
err db "CD read error!",0

; Disk Address Packet (DAP) for INT 13h extensions
dap:
    db 16                  ; size of packet
    db 0                   ; reserved
    dw 1                   ; number of sectors
    dw 0x8000              ; buffer offset
    dw 0x0000              ; buffer segment
    dq 2                   ; starting LBA (kernel at sector 2)

TIMES 510-($-$$) db 0
DW 0xAA55