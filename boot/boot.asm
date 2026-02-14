; boot.asm - BIOS bootloader for x86 Legacy (CD-ROM boot)
; Loads the kernel from CD-ROM using BIOS INT 13h extensions.

BITS 16
ORG 0x7C00

start:
    ; Set up stack
    xor ax, ax
    mov ss, ax
    mov sp, 0x7C00

    ; Print loading message
    mov si, msg
    call print_string

    ; Prepare Disk Address Packet (DAP) and call INT 13h extensions
    mov ah, 0x42            ; Extended Read
    mov dl, 0xE0            ; Drive number (CD-ROM, usually E0h+)
    mov bx, dap             ; ES:BX points to DAP
    mov ds, ax              ; DS = 0
    mov es, ax              ; ES = 0
    int 0x13
    jc disk_error           ; Jump if carry flag set (error)

    ; Jump to kernel loaded at 0x0000:0x8000
    jmp 0x0000:0x8000

disk_error:
    mov si, err
    call print_string
    hlt

; --- Print string routine ---
print_string:
    lodsb                   ; Load next byte from DS:SI into AL
    or al, al               ; Check for null terminator
    jz .done
    mov ah, 0x0E            ; BIOS teletype output
    mov bh, 0x00            ; Page number
    mov bl, 0x07            ; Text attribute (light gray on black)
    int 0x10
    jmp print_string
.done:
    ret

; --- Messages ---
msg db "Loading kernel from CD...",0
err db "CD read error!",0

; --- Disk Address Packet (DAP) ---
dap:
    db 16                   ; Size of packet
    db 0                    ; Reserved
    dw 1                    ; Number of sectors to read
    dw 0x8000               ; Buffer offset
    dw 0x0000               ; Buffer segment
    dq 2                    ; Starting LBA (kernel at sector 2)

TIMES 510-($-$$) db 0
DW 0xAA55                   ; Boot signature
