#include "screen.h"
#include <stddef.h>

#define VIDEO_MEMORY (char *)0xB8000
#define SCREEN_WIDTH 80
#define SCREEN_HEIGHT 25

static size_t cursor_x = 0, cursor_y = 0;
static unsigned char text_color = 0x07; // Domyślny: jasnoszary na czarnym tle

void set_color(unsigned char color) {
    text_color = color;
}

void clrscr() {
    asm volatile (
        "mov $0xB8000, %edi\n"
        "mov $0x0720, %ax\n"  // ' ' z atrybutem koloru 0x07 (domyślny)
        "mov $2000, %ecx\n"   // 80x25 = 2000 znaków
        "rep stosw\n"         // Wypełnia pamięć wartościami z AX
    );
    cursor_x = cursor_y = 0;
}

void putchar(char c) {
    char *video_memory = VIDEO_MEMORY;
    
    if (c == '\n') {
        cursor_x = 0;
        cursor_y++;
    } else {
        size_t pos = (cursor_y * SCREEN_WIDTH + cursor_x) * 2;
        video_memory[pos] = c;
        video_memory[pos + 1] = text_color;
        cursor_x++;
    }
    
    if (cursor_x >= SCREEN_WIDTH) {
        cursor_x = 0;
        cursor_y++;
    }
    
    if (cursor_y >= SCREEN_HEIGHT) {
        clrscr();
    }
}

void puts(const char *str) {
    while (*str) {
        putchar(*str++);
    }
}
