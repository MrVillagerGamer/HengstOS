#include <stdint.h>
#include <arch/i386/port.h>

typedef struct {
	uint16_t offset_low;
	uint16_t selector;
	uint8_t reserved;
	uint8_t type_attr;
	uint16_t offset_high;
} __attribute__((packed)) ints_t;

typedef struct {
	uint16_t limit;
	uint32_t base;
} __attribute__((packed)) ints_ptr_t;

ints_t idt[256];

void ints_set(uint8_t idx, uint32_t addr) {
	idt[idx].offset_low = addr & 0xFFFF;
	idt[idx].offset_high = (addr >> 16) & 0xFFFF;
	idt[idx].selector = 0x08;
	idt[idx].reserved = 0;
	idt[idx].type_attr = 0x8e|0x60;
}

void remap_pic() {
	outb(0x20, 0x11);
	outb(0xA0, 0x11);
	outb(0x21, 0x20);
	outb(0xA1, 0x28);
	outb(0x21, 0x04);
	outb(0xA1, 0x02);
	outb(0x21, 0x01);
	outb(0xA1, 0x01);
	outb(0x21, 0x00);
	outb(0xA1, 0x00);
}

void ints_flush() {
	ints_ptr_t idtr;
	idtr.base = (uint32_t)idt;
	idtr.limit = 256 * sizeof(ints_t);
	asm volatile("lidt %0"::"m"(idtr));
	//asm volatile("sti");
}

void ints_init() {
	extern void dummy_handler();
	extern void ints_wrapper_32();
	extern void ints_wrapper_128();
	for(int i = 0; i < 1; i++) {
		ints_set(i, (uint32_t)dummy_handler);		
	}
	ints_set(32, (uint32_t)ints_wrapper_32);
	ints_set(128, (uint32_t)ints_wrapper_128);
	remap_pic();
	ints_flush();	
}

void ints_handler(uint16_t idx) {
	if(idx >= 0x20) {
		if(idx >= 0x28) {
			outb(0xA0, 0x20);
		}
		outb(0x20, 0x20);
	}
}
