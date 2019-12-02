#include <stdint.h>

#define NULL ((void*)0)

#define syscall(data) {\
asm volatile("pushl %%eax; movl %0, %%eax; int $0x80; popl %%eax"::"r"((uint32_t)data));\
\
}

#define COLOR_DEFAULT 0x1f

#define SYS_PUTS 0
#define SYS_GETC 1
#define SYS_READ 2
#define SYS_WRITE 3
#define SYS_CHDIR 4
#define SYS_GETCWD 5
#define SYS_CLEAR 6
#define SYS_EXEC 7
#define SYS_STATE 8
#define SYS_EXIT 9
#define SYS_GOTO 10
#define SYS_UPTIME 11
#define SYS_LIST 12

typedef struct {
    uint32_t mode;
    void* info;
} __attribute__((packed)) sys_info_t;

typedef struct {
	const char* buffer;
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

typedef struct {
	const char* path;
	int argc;
	char** argv;
	int handle;
} __attribute__((packed)) sys_exec_info_t;

typedef struct {
	int handle;
	int state;
} __attribute__((packed)) sys_state_info_t;

typedef struct {
	int x;
	int y;
} __attribute__((packed)) sys_goto_info_t;

typedef struct {
	uint32_t data;
} __attribute__((packed)) sys_uptime_info_t;

typedef struct {
	const char* path;
	char** list;
	int count;
	int len;
} __attribute__((packed)) sys_list_info_t;

extern void puts(const char*,uint8_t);
extern uint8_t getc();
extern void read(const char*,char*,int);
extern void list(const char*,char**,int,int);
extern void clear();
extern void chdir(const char*);
extern void getcwd(char*);
