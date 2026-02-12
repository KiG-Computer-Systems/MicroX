// mouse.c - Minimal PS/2 mouse driver for x86
// Handles initialization and IRQ12 events

#include "kernel.h"

// I/O port functions (you should implement inportb/outportb in kernel)
extern unsigned char inportb(unsigned short port);
extern void outportb(unsigned short port, unsigned char data);

// Mouse packet buffer
static unsigned char mouse_cycle = 0;
static char mouse_bytes[3];
static int mouse_x = 0;
static int mouse_y = 0;

// Send command to mouse
static void mouse_write(unsigned char data) {
    // Wait until we can send
    while ((inportb(0x64) & 0x02) != 0);
    outportb(0x64, 0xD4);       // Tell controller we are sending to mouse
    while ((inportb(0x64) & 0x02) != 0);
    outportb(0x60, data);
}

// Read response from mouse
static unsigned char mouse_read() {
    while ((inportb(0x64) & 0x01) == 0);
    return inportb(0x60);
}

// Initialize mouse
void init_mouse() {
    print("Initializing mouse...\n");

    // Enable auxiliary device (mouse)
    outportb(0x64, 0xA8);

    // Enable interrupts
    outportb(0x64, 0x20);
    unsigned char status = inportb(0x60);
    status |= 0x02;
    outportb(0x64, 0x60);
    outportb(0x60, status);

    // Tell mouse to use default settings
    mouse_write(0xF6);
    mouse_read();

    // Enable mouse
    mouse_write(0xF4);
    mouse_read();

    print("Mouse initialized\n");
}

// IRQ12 handler
void handle_mouse() {
    unsigned char data = inportb(0x60);

    switch (mouse_cycle) {
        case 0:
            mouse_bytes[0] = data;
            mouse_cycle++;
            break;
        case 1:
            mouse_bytes[1] = data;
            mouse_cycle++;
            break;
        case 2:
            mouse_bytes[2] = data;
            mouse_x += mouse_bytes[1];
            mouse_y -= mouse_bytes[2]; // Y is inverted
            mouse_cycle = 0;

            // Print coordinates (for demo)
            print("Mouse moved: X=");
            // You can implement print_int() to show numbers
            break;
    }
}