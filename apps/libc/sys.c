#include "sys.h"

void puts(const char* buffer, uint8_t color) {
    sys_puts_info_t data;
    data.buffer = buffer;
    data.color = color;
    sys_info_t info;
    info.mode = SYS_PUTS;
    info.info = &data;
    syscall(&info);
}

uint8_t getc() {
    sys_getc_info_t data;
    sys_info_t info;
    info.mode = SYS_GETC;
    info.info = &data;
    syscall(&info);
    return data.data;
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

void list(const char* path, char** paths, int count, int len) {
    sys_list_info_t data;
    data.path = path;
    data.list = paths;
    data.count = count;
    data.len = len;
    sys_info_t info;
    info.mode = SYS_LIST;
    info.info = &data;
    syscall(&info);
}

void clear() {
    sys_info_t info;
    info.mode = SYS_CLEAR;
    syscall(&info);
}

void chdir(const char* path) {
    sys_chdir_info_t data;
    data.buffer = path;
    sys_info_t info;
    info.mode = SYS_CHDIR;
    info.info = &data;
    syscall(&info);
}

void getcwd(char* path) {
    sys_getcwd_info_t data;
    data.buffer = path;
    sys_info_t info;
    info.mode = SYS_GETCWD;
    info.info = &data;
    syscall(&info);
}