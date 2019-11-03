#include "sys.h"

#define MAX_FILE_SIZE 800

void puts(const char* buffer, uint8_t color) {
    sys_puts_info_t wdata;
    wdata.buffer = buffer;
    wdata.color = color;
    sys_info_t winfo;
    winfo.mode = SYS_PUTS;
    winfo.info = &wdata;
    syscall(&winfo);
}

void read(const char* path, char* buffer, int count) {
    sys_read_info_t wdata;
    wdata.path = path;
    wdata.buffer = buffer;
    wdata.count = count;
    sys_info_t winfo;
    winfo.mode = SYS_READ;
    winfo.info = &wdata;
    syscall(&winfo);
}

int main(int argc, char** argv) {
    if(argc <= 1) {
        puts("No input files.", 0x07);
        return -1;
    }
    for(int i = 1; i < argc; i++) {
        char buffer[MAX_FILE_SIZE];
        read(argv[i], buffer, MAX_FILE_SIZE);
        puts(buffer, 0x07);
    }
    return 0;
}