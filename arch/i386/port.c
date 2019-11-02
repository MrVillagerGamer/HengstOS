#include <arch/i386/port.h>

void outb(uint16_t port, uint8_t data) {
	asm volatile("outb %0, %1"::"a"(data),"Nd"(port));
}

void outw(uint16_t port, uint16_t data) {
	asm volatile("outw %0, %1"::"a"(data),"Nd"(port));
}

void outl(uint16_t port, uint32_t data) {
	asm volatile("outl %0, %1"::"a"(data),"Nd"(port));
}

uint8_t inb(uint16_t port) {
	uint8_t res;
	asm volatile("inb %1, %0":"=a"(res):"Nd"(port));
	return res;
}

uint16_t inw(uint16_t port) {
	uint16_t res;
	asm volatile("inw %1, %0":"=a"(res):"Nd"(port));
	return res;
}

uint32_t inl(uint16_t port) {
	uint32_t res;
	asm volatile("inl %1, %0":"=a"(res):"Nd"(port));
	return res;
}
