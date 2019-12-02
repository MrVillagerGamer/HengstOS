#include <arch/i386/io.h>
#include <arch/i386/font.h>
#include <stdint.h>
#include <stdbool.h>

uint8_t io_attr = 0x07;
int io_x = 0, io_y = 0;
uint8_t* io_vram = (uint8_t*)0xb8000;

int io_w = 80;
int io_h = 25;

void io_clear() {
	io_attr = 0x07;
	for(int i = 0; i < io_w*io_h; i++) {
		io_vram[i*2+0] = ' ';
		io_vram[i*2+1] = io_attr;
	}
	io_x = 0;
	io_y = 0;
}

void io_color(uint8_t val) {
	io_attr = val;
}

void io_puth(uint32_t hex) {
	bool ignore_zero = 0;
	char* map = "0123456789ABCDEF";	
	for(int i = 7; i >= 0; i--) {
		uint32_t val = (hex >> (i * 4)) & 0xf;
		if(val == 0 && !ignore_zero) {
			continue;
		}else{
			ignore_zero = 1;
		}
		io_putc(map[val]);
	}
}

void io_scroll() {
	for(int i = 0; i < io_w; i++) {
		for(int j = 1; j < io_h; j++) {
			io_vram[(i+io_w*(j-1))*2] = io_vram[(i+io_w*j)*2];
			io_vram[(i+io_w*(j-1))*2+1] = io_vram[(i+io_w*j)*2+1];
		}
	}
	for(int i = 0; i < io_w; i++) {
		io_vram[(i+io_w*(io_h-1))*2] = ' ';
		io_vram[(i+io_w*(io_h-1))*2+1] = io_attr;
	}
}

void io_putc(char c) {
	switch(c) {
	case '\n':
		io_x = 0;
		io_y++;
		if(io_y >= io_h) {
			io_scroll();
			io_y--;
		}
		break;
	case '\t':
		io_x += 4;
		if(io_x >= io_w) {
			io_x = 0;
			io_y++;
			if(io_y >= io_h) {
				io_scroll();
				io_y--;
			}
		}
		break;
	case '\b':
		io_x--;
		io_vram[(io_y*io_w+io_x)*2] = ' ';
		break;
	default:
		io_vram[(io_y*io_w+io_x)*2] = c;
		io_vram[(io_y*io_w+io_x)*2+1] = io_attr;
		io_x++;
		if(io_x >= io_w) {
			io_x = 0;
			io_y++;
			if(io_y >= io_h) {
				io_scroll();
				io_y--;
			}
		}
		break;
	}
}

void io_puts(char* s) {
	int i = 0;
	while(s[i] != 0) {
		io_putc(s[i]);
		i++;
	}
}

void io_goto(int x, int y) {
	io_x = x;
	io_y = y;
}


























