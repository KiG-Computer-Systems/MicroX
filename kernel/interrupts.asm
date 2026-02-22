; interrupts.asm - Interrupt handling code for the kernel
global load_idt
global irq0
global irq12

extern handle_mouse

section .text

; Load IDT pointer
; Argument: pointer to IDT descriptor in eax
load_idt:
    lidt [eax]
    ret

; Timer interrupt (IRQ0)
irq0:
    cli                 ; disable interrupts
    pusha               ; save general registers
    push ds
    push es
    push fs
    push gs

    ; timer interrupt handler could be called here

    pop gs
    pop fs
    pop es
    pop ds
    popa
    sti                 ; re-enable interrupts
    iretd

; Mouse interrupt (IRQ12)
irq12:
    cli
    pusha
    push ds
    push es
    push fs
    push gs

    call handle_mouse   ; call C handler

    pop gs
    pop fs
    pop es
    pop ds
    popa
    sti
    iretd
