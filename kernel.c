#include "screen.h"

void kernel_main() {
    clrscr();

    set_color(0x0E); 
    puts("HI\n");

    set_color(0x4F); 
    puts("hello in CGRUB2OS!");

    while (1);
}
