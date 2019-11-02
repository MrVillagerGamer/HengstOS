#ifndef __INIX_ARCH_I386__
#define __INIX_ARCH_I386__
#endif

#ifndef __INIX_ARCH_I386__
#error Unsupported Architecture
#endif

#include <stdint.h>
#include <arch/i386/term.h>
#include <arch/i386/sys.h>

uint8_t stack[0x4000];

void other_main() {
	while(1);
}

char buf[0x10000];

void main(uint32_t mbaddr) {
	uint32_t mods_count = *(uint32_t*)(mbaddr+20);	
	uint32_t mods_addr = *(uint32_t*)(mbaddr+24);
	uint32_t init_addr = *(uint32_t*)mods_addr;
	term_color(0x07);
	term_clear();
	//term_puts("Initializing hardware... \n");
	extern void init();
	extern void rd_init(uint32_t);
	init();
	rd_init(init_addr);
	fs_read("Z:\\HengstOS\\System32\\Init.bin", buf, 0x10000);
	//term_puts(buf);
	asm volatile("int $32");
	mkproc((uint32_t)buf, 0x10000, 0x8000);
	//create_process((uint32_t)other_main, (uint32_t)stack+0x4000, 0x1B, 0x23);
	while(1);
}
