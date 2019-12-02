#include <stdint.h>
#include <stdbool.h>

#define KERNEL_HEAP_START_64KB (16*32)

bool heap_bitmap[0x10000-KERNEL_HEAP_START_64KB]; // 1 MiB blocks
uint16_t heap_sizes[0x10000-KERNEL_HEAP_START_64KB];

void heap_init() {
	for(int i = 0; i < 0x10000-KERNEL_HEAP_START_64KB; i++) {
		heap_bitmap[i] = 0;
	}
}

void kfree(void* data) {
	int idx = ((uint32_t)data/0x10000)-KERNEL_HEAP_START_64KB;
	uint16_t size = heap_sizes[idx];
	term_puth(size);
	for(int i = 0; i < size; i++) {
		heap_bitmap[idx+i] = 0;
	}
}

void* kalloc(int size) {
	int i;
	for(i = 0; i < 0x10000-KERNEL_HEAP_START_64KB; i++) {
		if(heap_bitmap[i] == 0) {
			bool valid = 1;
			for(int j = 0; j < size; j++) {
				if(heap_bitmap[i+j] == 1) {
					valid = 0;
				}
			}
			if(valid) {
				for(int j = 0; j < size; j++) {
					heap_bitmap[i+j] = 1;
				}
				break;
			}
		}
	}
	heap_sizes[i] = size;
	return (void*)((i+KERNEL_HEAP_START_64KB)*0x10000);
}


















