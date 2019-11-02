#include <stdint.h>
#include <stdbool.h>

typedef struct {
	uint16_t limit_low;
	uint16_t base_low;
	uint8_t base_mid;
	uint8_t access_rights;
	uint8_t flags_limit_high;
	uint8_t base_high;
} __attribute__((packed)) mseg_t;

typedef struct {
	uint16_t limit;
	uint32_t base;
} __attribute__((packed)) mseg_ptr_t;

bool used_segs[2048-3];
mseg_t gdt[4096];

extern void mseg_set(uint16_t,uint32_t,uint32_t,uint8_t,uint8_t);

int mseg_alloc(uint32_t base, uint32_t limit) {
	int idx = 0;
	bool found = 0;
	while(1) {
		if(!used_segs[idx]) {
			break;
		}
		idx++;
	}
	mseg_set(idx*2+6, base, limit, 0xFA, 0xCF);
	mseg_set(idx*2+7, base, limit, 0xF2, 0xCF);
	used_segs[idx] = 1;
	return idx;
}

void mseg_free(int idx) {
	used_segs[idx] = 0;
}

uint32_t get_data_base(uint16_t seg) {
	seg /= 8;
	mseg_t segd = gdt[seg];
	uint32_t base = segd.base_low |
			(segd.base_mid << 16) |
			(segd.base_high << 24);
	return base;
}

void mseg_set(uint16_t idx, uint32_t base, uint32_t limit, uint8_t type, uint8_t flags) {
	gdt[idx].base_low = (base >> 0) & 0xFFFF;
	gdt[idx].base_mid = (base >> 16) & 0xFF;
	gdt[idx].base_high = (base >> 24) & 0xFF;
	gdt[idx].limit_low = (limit >> 0) & 0xFFFF;
	gdt[idx].flags_limit_high = (flags & 0xF0) | ((limit >> 16) & 0xF);
	gdt[idx].access_rights = type;
}

void mseg_flush() {
	mseg_ptr_t gdtr;
	gdtr.limit = sizeof(gdt) - 1;
	gdtr.base = (uint32_t)gdt;
	asm volatile("lgdt %0"::"m"(gdtr));
	asm volatile("ljmp $0x08,$.fixcs;.fixcs:;mov $0x10, %ax; mov %ax, %es; mov %ax, %ds; mov %ax, %fs; mov %ax, %gs; mov %ax, %ss;");
}

void mseg_init() {
	for(int i = 0; i < 2048-3; i++) {
		used_segs[i] = 0;
	}
	mseg_set(0, 0, 0, 0, 0);
	mseg_set(1, 0, 0xffffffff, 0x9A, 0xCF);
	mseg_set(2, 0, 0xffffffff, 0x92, 0xCF);
	mseg_set(3, 0, 0xffffffff, 0xFA, 0xCF);
	mseg_set(4, 0, 0xffffffff, 0xF2, 0xCF);
	mseg_flush();
}
