; interrupts.asm - Interrupt handling code for the kernel
global load_idt
global irq0
global irq12

extern handle_mouse

section .text

load_idt:
    mov eax, [esp+4]
    lidt [eax]
    ret

irq0:
    pusha
    ; timer interrupt stub
    popa
    iretd

irq12:
    pusha
    call handle_mouse
    popa
    iretd
