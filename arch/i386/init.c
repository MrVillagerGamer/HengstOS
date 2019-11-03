#include <arch/i386/pci.h>
#include <arch/i386/port.h>

void init() {
	extern void mseg_init();
	extern void ints_init();
	extern void user_init();
	extern void task_init();
	extern void heap_init();
	extern void ps2_kbd_init();
	//mseg_set(6, addr, 0xfffff, 0xFA, 0x4F);
	//mseg_set(7, addr, 0xfffff, 0xF2, 0x4F);
	mseg_init();
	task_init();
	ints_init();
	heap_init();
	ps2_kbd_init();
	pci_scan();
	int frequency = 480;
	uint32_t divisor = 1193180 / frequency;
	outb(0x43, 0x36);
	uint8_t l = divisor & 0xff;
	uint8_t h = (divisor >> 8) & 0xff;
	outb(0x40, l);
	outb(0x40, h);
	user_init();
}
