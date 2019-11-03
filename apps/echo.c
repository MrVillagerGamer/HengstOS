#include "sys.h"

void puts(const char* buffer, uint8_t color) {
    sys_puts_info_t wdata;
    wdata.buffer = buffer;
    wdata.color = color;
    sys_info_t winfo;
    winfo.mode = SYS_PUTS;
    winfo.info = &wdata;
    syscall(&winfo);
}

int main(int argc, char** argv) {
    if(argc <= 1) {
        puts("No input data.", 0x07);
        return -1;
    }
    for(int i = 1; i < argc; i++) {
        puts(argv[i], 0x07);
        puts(" ", 0x07);
    }
    return 0;
}