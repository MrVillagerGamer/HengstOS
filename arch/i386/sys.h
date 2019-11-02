#pragma once

#include <stdint.h>

typedef struct {
	uint32_t func;
	void* data;
} __attribute__((packed)) syscall_info_t;

typedef struct {
	char* buffer;
} __attribute__((packed)) sys_chdir_info_t;

typedef struct {
	char* buffer;
} __attribute__((packed)) sys_getcwd_info_t;

typedef struct {
	uint8_t data;
} __attribute__((packed)) sys_getc_info_t;

typedef struct {
	const char* buffer;
	uint8_t color;
} __attribute__((packed)) sys_puts_info_t;

typedef struct {
	const char* path;
	char* buffer;
	int count;
} __attribute__((packed)) sys_read_info_t;

typedef struct {
	const char* path;
	const char* buffer;
	int count;
} __attribute__((packed)) sys_write_info_t;

#define syscall(data) {\
asm volatile("pushl %%eax; movl %0, %%eax; int $0x80; popl %%eax"::"r"((uint32_t)data));\
\
}
