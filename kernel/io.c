// io.c - Basic I/O functions for MicroX
unsigned char inportb(unsigned short port) {
    unsigned char ret;
    __asm__ __volatile__ ("inb %1, %0" : "=a"(ret) : "dN"(port));
    return ret;
}

void outportb(unsigned short port, unsigned char data) {
    __asm__ __volatile__ ("outb %0, %1" : : "a"(data), "dN"(port));
}
