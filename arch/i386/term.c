#include <arch/i386/term.h>
#include <stdint.h>
#include <stdbool.h>

uint8_t term_attr = 0x02;
int term_x = 0, term_y = 0;
uint8_t* term_vram = (uint8_t*)0xB8000;

void term_clear() {
	for(int i = 0; i < 80*25; i++) {
		term_vram[i*2+0] = ' ';
		term_vram[i*2+1] = term_attr;
	}
	term_x = 0;
	term_y = 0;
}

void term_color(uint8_t val) {
	term_attr = val;
}

void term_puth(uint32_t hex) {
	bool ignore_zero = 0;
	char* map = "0123456789ABCDEF";	
	for(int i = 7; i >= 0; i--) {
		uint32_t val = (hex >> (i * 4)) & 0xf;
		if(val == 0 && !ignore_zero) {
			continue;
		}else{
			ignore_zero = 1;
		}
		term_putc(map[val]);
	}
}

void term_scroll() {
	for(int i = 0; i < 80; i++) {
		for(int j = 1; j < 25; j++) {
			term_vram[(i+80*(j-1))*2] = term_vram[(i+80*j)*2];
			term_vram[(i+80*(j-1))*2+1] = term_vram[(i+80*j)*2+1];
		}
	}
	for(int i = 0; i < 80; i++) {
		term_vram[(i+80*24)*2] = ' ';
		term_vram[(i+80*24)*2+1] = term_attr;
	}
}

void term_putc(char c) {
	switch(c) {
	case '\n':
		term_x = 0;
		term_y++;
		if(term_y >= 24) {
			term_scroll();
			term_y--;
		}
		break;
	default:
		term_vram[(term_y*80+term_x)*2] = c;
		term_vram[(term_y*80+term_x)*2+1] = term_attr;
		term_x++;
		if(term_x >= 80) {
			term_x = 0;
			term_y++;
			if(term_y >= 24) {
				term_scroll();
				term_y--;
			}
		}
		break;
	}
}

void term_puts(char* s) {
	int i = 0;
	while(s[i]) {
		term_putc(s[i]);
		i++;
	}
}

void term_goto(int x, int y) {
	term_x = x;
	term_y = y;
}


























