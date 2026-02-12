// kernel.c - MicroX minimal kernel entry point
// Initializes interrupts and mouse driver

#include "kernel.h"

void kmain() {
    print("MicroX Kernel started\n");

    // Initialize interrupts
    init_interrupts();

    // Initialize mouse driver
    init_mouse();

    // Idle loop
    while (1) {
        __asm__ __volatile__("hlt"); // halt until next interrupt
    }
}
