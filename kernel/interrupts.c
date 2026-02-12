// interrupts.c - Basic interrupt setup for MicroX
// Sets up IDT, remaps PIC, and installs IRQ handlers (including mouse IRQ12)

#include "kernel.h"

#define IDT_SIZE 256

// IDT entry structure
struct idt_entry {
    unsigned short base_low;
    unsigned short sel;
    unsigned char always0;
    unsigned char flags;
    unsigned short base_high;
} __attribute__((packed));

struct idt_ptr {
    unsigned short limit;
    unsigned int base;
} __attribute__((packed));

extern void load_idt(struct idt_ptr*);   // ASM function to load IDT
extern void irq0();                      // Timer handler stub
extern void irq12();                     // Mouse handler stub

struct idt_entry idt[IDT_SIZE];
struct idt_ptr idtp;

// Set IDT gate
static void idt_set_gate(unsigned char num, unsigned int base, unsigned short sel, unsigned char flags) {
    idt[num].base_low = (base & 0xFFFF);
    idt[num].sel = sel;
    idt[num].always0 = 0;
    idt[num].flags = flags;
    idt[num].base_high = (base >> 16) & 0xFFFF;
}

// Remap PIC
static void pic_remap() {
    outportb(0x20, 0x11);
    outportb(0xA0, 0x11);
    outportb(0x21, 0x20); // Master PIC vector offset
    outportb(0xA1, 0x28); // Slave PIC vector offset
    outportb(0x21, 0x04);
    outportb(0xA1, 0x02);
    outportb(0x21, 0x01);
    outportb(0xA1, 0x01);
    outportb(0x21, 0x0);
    outportb(0xA1, 0x0);
}

// Initialize interrupts
void init_interrupts() {
    idtp.limit = (sizeof(struct idt_entry) * IDT_SIZE) - 1;
    idtp.base = (unsigned int)&idt;

    // Clear IDT
    for (int i = 0; i < IDT_SIZE; i++) {
        idt_set_gate(i, 0, 0, 0);
    }

    // Remap PIC
    pic_remap();

    // Install IRQ0 (timer) and IRQ12 (mouse)
    idt_set_gate(32, (unsigned int)irq0, 0x08, 0x8E);
    idt_set_gate(44, (unsigned int)irq12, 0x08, 0x8E);

    // Load IDT
    load_idt(&idtp);

    print("Interrupts initialized\n");
}