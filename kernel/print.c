// print.c - simple text output using BIOS teletype
#include "kernel.h"

void print(const char* str) {
    while (*str) {
        __asm__ __volatile__ (
            "movb $0x0E, %%ah\n\t"
            "int $0x10"
            : : "a"(*str)
        );
        str++;
    }
}
