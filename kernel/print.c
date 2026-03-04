// print.c - improved text output using BIOS teletype
#include "kernel.h"

void print(const char* str) {
    while (*str) {
        if (*str == '\n') {
            __asm__ __volatile__ (
                "movb $0x0E, %%ah\n\t"
                "movb $0x0D, %%al\n\t"   // CR
                "int $0x10\n\t"
                "movb $0x0E, %%ah\n\t"
                "movb $0x0A, %%al\n\t"   // LF
                "int $0x10\n\t"
                :
                :
                : "ax"
            );
        } else {
            __asm__ __volatile__ (
                "movb $0x0E, %%ah\n\t"
                "int $0x10\n\t"
                :
                : "a"(*str)
                : "ax"
            );
        }
        str++;
    }
}
