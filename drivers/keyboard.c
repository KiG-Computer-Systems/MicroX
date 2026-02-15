#include <microx/io.h>
#include <microx/interrupts.h>
#include <microx/console.h>

#define PS2_DATA_PORT   0x60
#define PS2_STATUS_PORT 0x64
#define KEYBOARD_IRQ    1
#define BUFFER_SIZE     128

static char key_buffer[BUFFER_SIZE];
static int buffer_head = 0;
static int buffer_tail = 0;
static int shift_pressed = 0;

static const char scancode_table[128] = {
    0, 27, '1','2','3','4','5','6','7','8','9','0','-','=', '\b',
    '\t','q','w','e','r','t','y','u','i','o','p','[',']','\n', 0,
    'a','s','d','f','g','h','j','k','l',';','\'','`', 0,
    '\\','z','x','c','v','b','n','m',',','.','/', 0,
    '*', 0,' ', 0,
};

static const char scancode_table_shift[128] = {
    0, 27, '!','@','#','$','%','^','&','*','(',')','_','+', '\b',
    '\t','Q','W','E','R','T','Y','U','I','O','P','{','}','\n', 0,
    'A','S','D','F','G','H','J','K','L',':','"','~', 0,
    '|','Z','X','C','V','B','N','M','<','>','?', 0,
    '*', 0,' ', 0,
};

static void buffer_put(char c) {
    int next = (buffer_head + 1) % BUFFER_SIZE;
    if (next != buffer_tail) {
        key_buffer[buffer_head] = c;
        buffer_head = next;
    }
}

char keyboard_read() {
    if (buffer_head == buffer_tail) return 0;
    char c = key_buffer[buffer_tail];
    buffer_tail = (buffer_tail + 1) % BUFFER_SIZE;
    return c;
}

int keyboard_has_data() {
    return buffer_head != buffer_tail;
}

static void keyboard_isr() {
    unsigned char scancode = inb(PS2_DATA_PORT);

    if (scancode == 0x2A || scancode == 0x36) {
        shift_pressed = 1;
        return;
    }
    if (scancode == 0xAA || scancode == 0xB6) {
        shift_pressed = 0;
        return;
    }

    if (scancode & 0x80) {
    } else {
        char c = shift_pressed ? scancode_table_shift[scancode] : scancode_table[scancode];
        if (c) {
            buffer_put(c);
            console_putc(c);
        }
    }
}

void keyboard_init() {
    register_interrupt_handler(KEYBOARD_IRQ, keyboard_isr);
    console_write("Keyboard driver initialized.\n");
}
