#include "gfw.h"
#include "sys.h"

uint8_t buffer[80*25*2];

void gfwStartup() {
    static sys_info_t info;
    info.mode = SYS_CLEAR;
    syscall(&info);
}

void gfwCleanup() {
    static sys_info_t info;
    info.mode = SYS_CLEAR;
    syscall(&info);
}

void gfwPlotPixel(int x, int y, int color) {
    int y1 = y / 2;
    if((y % 2) == 0) {
        buffer[(x+80*y1)*2] = 220;
        buffer[(x+80*y1)*2+1] = (color << 4) | (buffer[(x+80*y1)*2+1] & 0x0F); 
    }else{
        buffer[(x+80*y1)*2] = 220;
        buffer[(x+80*y1)*2+1] = (color << 0) | (buffer[(x+80*y1)*2+1] & 0xF0);
    }
}

void gfwClearScreen() {
    for(int i = 0; i < 80*25; i++) {
        buffer[i*2] = 0;
        buffer[i*2+1] = 0x11;
    }
}

int gfwGetTime() {
    static sys_uptime_info_t data;
    static sys_info_t info;
    info.mode = SYS_UPTIME;
    info.info = &data;
    syscall(&info);
    return data.data;
}

void gfwDelay(int length) {
    int time = gfwGetTime();
    while(gfwGetTime() < time+length);
}

void gfwExit() {
    static sys_info_t info;
    info.mode = SYS_EXIT;
    syscall(&info);
    while(1);
}

void gfwDrawText(int x, int y, const char* str, int color) {
    int i = 0;
    int cx = x;
    int cy = y/2;
    while(str[i]) {
        buffer[(cx+80*cy)*2] = str[i];
        buffer[(cx+80*cy)*2+1] = color;
        cx++;
        i++;
    }
}

void gfwDrawRect(int x1, int y1, int x2, int y2, int color) {
    for(int x = x1; x <= x2; x++) {
        gfwPlotPixel(x, y1, color);
        gfwPlotPixel(x, y2, color);
    }
    for(int y = y1; y <= y2; y++) {
        gfwPlotPixel(x1, y, color);
        gfwPlotPixel(x2, y, color);
    }
}

void gfwFillRect(int x1, int y1, int x2, int y2, int color) {
    for(int x = x1; x <= x2; x++) {
        for(int y = y1; y <= y2; y++) {
            gfwPlotPixel(x, y, color);
        }
    }
}

void gfwSwapBuffers() {
    for(int x = 0; x < 80; x++) {
        for(int y = 0; y < 24; y++) {
            int i = x+80*y;
            static char* str = " ";
            str[0] = buffer[i*2];
            if(str[0] == 0) {
                str[0] = ' ';
            }
            
            static sys_info_t info;

            static sys_goto_info_t data1;
            data1.x = x;
            data1.y = y;
            info.mode = SYS_GOTO;
            info.info = &data1;
            syscall(&info);

            static sys_puts_info_t data2;
            data2.buffer = str;
            data2.color = buffer[i*2+1];
            info.mode = SYS_PUTS;
            info.info = &data2;
            syscall(&info);
        }
    }
}
