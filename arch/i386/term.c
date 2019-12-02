#include <arch/i386/term.h>
#include <arch/i386/font.h>
#include <stdint.h>
#include <stdbool.h>

uint8_t term_attr = 0x07;
int term_x = 0, term_y = 0;
uint8_t* term_vram = (uint8_t*)0xb8000;

int term_w = 80;
int term_h = 25;

void term_clear() {
	term_attr = 0x1f;
	for(int i = 0; i < term_w*term_h; i++) {
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
	for(int i = 0; i < term_w; i++) {
		for(int j = 1; j < term_h; j++) {
			term_vram[(i+term_w*(j-1))*2] = term_vram[(i+term_w*j)*2];
			term_vram[(i+term_w*(j-1))*2+1] = term_vram[(i+term_w*j)*2+1];
		}
	}
	for(int i = 0; i < term_w; i++) {
		term_vram[(i+term_w*(term_h-1))*2] = ' ';
		term_vram[(i+term_w*(term_h-1))*2+1] = 0x1f;
	}
}

void term_putc(char c) {
	switch(c) {
	case '\n':
		term_x = 0;
		term_y++;
		if(term_y >= term_h) {
			term_scroll();
			term_y--;
		}
		break;
	case '\t':
		term_x += 4;
		if(term_x >= term_w) {
			term_x = 0;
			term_y++;
			if(term_y >= term_h) {
				term_scroll();
				term_y--;
			}
		}
		break;
	case '\b':
		term_x--;
		term_vram[(term_y*term_w+term_x)*2] = ' ';
		break;
	default:
		term_vram[(term_y*term_w+term_x)*2] = c;
		term_vram[(term_y*term_w+term_x)*2+1] = term_attr;
		term_x++;
		if(term_x >= term_w) {
			term_x = 0;
			term_y++;
			if(term_y >= term_h) {
				term_scroll();
				term_y--;
			}
		}
		break;
	}
}

void term_puts(char* s) {
	int i = 0;
	while(s[i] != 0) {
		term_putc(s[i]);
		i++;
	}
}

void term_goto(int x, int y) {
	term_x = x;
	term_y = y;
}


























