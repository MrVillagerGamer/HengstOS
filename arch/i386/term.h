#pragma one

#include <stdint.h>

extern void term_color(uint8_t);
extern void term_puts(char*);
extern void term_putc(char);
extern void term_clear();
extern void term_scroll();
extern void term_goto(int,int);
