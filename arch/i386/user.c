#include <stdint.h>

typedef struct {
	uint32_t prev_tss;
	uint32_t esp0;
	uint32_t ss0;
	uint32_t esp1;
	uint32_t ss1;
	uint32_t esp2;
	uint32_t ss2;
	uint32_t cr3;
	uint32_t eip;
	uint32_t eflags;
	uint32_t eax;
	uint32_t ecx;
	uint32_t edx;
	uint32_t ebx;
	uint32_t esp;
	uint32_t ebp;
	uint32_t esi;
	uint32_t edi;
	uint32_t es;         
	uint32_t cs;        
	uint32_t ss;        
	uint32_t ds;        
	uint32_t fs;       
	uint32_t gs;         
	uint32_t ldt;      
	uint16_t trap;
	uint16_t iomap_base;
} __attribute__((packed)) tss_t;

tss_t tss;
void init_tss() {
	extern unsigned int INTERRUPT_STACK;
	for(int i = 0; i < sizeof(tss_t); i++) {
		((uint8_t*)&tss)[i] = 0;
	}
	tss.esp0 = (uint32_t)&INTERRUPT_STACK;
	tss.ss0 = 0x10;
}

void load_tss() {
	extern void mseg_set(uint16_t,uint32_t,uint32_t,uint8_t,uint8_t);
	uint32_t base = (uint32_t)&tss;
	uint32_t limit = 0x67;
	mseg_set(5, base, limit, 0xE9, 0x00);
	asm volatile("push %ax; mov $0x2b, %ax; ltr %ax; pop %ax");
}

void enter_userm() {
	asm volatile("mov $0x23, %ax; mov %ax, %ds; mov %ax, %es; mov %ax, %fs; mov %ax, %gs;\
	movl %esp, %eax; pushl $0x23; pushl %eax; pushl $0x202; pushl $0x1b; pushl $.done; iretl; .done:");
}

void user_init() {
	init_tss();
	load_tss();
	enter_userm();
}

























