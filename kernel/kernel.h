// kernel.h - MicroX kernel header

#ifndef KERNEL_H
#define KERNEL_H

// Basic kernel functions
void print(const char* str);
void init_interrupts();
void init_mouse();

// Low-level port I/O
unsigned char inportb(unsigned short port);
void outportb(unsigned short port, unsigned char data);

#endif
