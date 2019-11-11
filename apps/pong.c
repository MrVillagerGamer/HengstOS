#include "libc/gfw.h"
#include "libc/sys.h"

#define LUP 119
#define LDOWN 115
#define RUP 113
#define RDOWN 97

typedef struct {
    int x, y;
} coord_t;

coord_t ball_loc;
coord_t ball_dir;

int lpos = 0;
int rpos = 0;
int lpoints = 0;
int rpoints = 0;

int wgetc() {
    while(!getc());
    int key = getc();
    while(getc());
    return key;
}

void reflectX() {
    ball_dir.x = -ball_dir.x;
}

void reflectY() {
    ball_dir.y = -ball_dir.y;
}

int checkPaddle(int side) {
    if(side && ball_loc.x == 71 && ball_loc.y >= rpos && ball_loc.y < rpos + 8) {
        return 1;
    }
    if(!side && ball_loc.x == 1 && ball_loc.y >= lpos && ball_loc.y < lpos + 8) {
        return 1;
    }
    return 0;
}

void drawBall() {
    ball_loc.x += ball_dir.x;
    ball_loc.y += ball_dir.y;
    gfwPlotPixel(4+ball_loc.x, 4+ball_loc.y, GFW_COLOR_WHITE);
    gfwPlotPixel(4+ball_loc.x+1, 4+ball_loc.y, GFW_COLOR_WHITE);
    gfwPlotPixel(4+ball_loc.x, 4+ball_loc.y+1, GFW_COLOR_WHITE);
    gfwPlotPixel(4+ball_loc.x+1, 4+ball_loc.y+1, GFW_COLOR_WHITE);
}

void drawPaddle(int side, int loc) {
    for(int i = 4+loc; i < 12+loc; i++) {
        if(side) {
            gfwPlotPixel(76, i, GFW_COLOR_WHITE);
        }else{
            gfwPlotPixel(4, i, GFW_COLOR_WHITE);
        }
    }
    
}

int exiting = 0;

void update() {
    gfwDelay(20);
    gfwClearScreen();
    gfwDrawText(2, 0, "Points: [Not Implemented]", GFW_COLOR_WHITE);
    gfwDrawRect(2, 2, 78, 44, GFW_COLOR_WHITE);
    for(int i = 4; i <= 42; i++) {
        gfwPlotPixel(40, i, GFW_COLOR_WHITE);
    }
    int chr = getc();
    if(chr == LUP && lpos > 0) {
        lpos--;
    }
    if(chr == LDOWN && lpos < 29) {
        lpos++;
    }
    if(chr == RUP && rpos > 0) {
        rpos--;
    }
    if(chr == RDOWN && rpos < 29) {
        rpos++;
    }
    if(checkPaddle(0)) {
        reflectX();
        lpoints++;
    }
    if(checkPaddle(1)) {
        reflectX();
        rpoints++;
    }
    if(ball_loc.y >= 38) {
        reflectY();
    }
    if(ball_loc.y <= 0) {
        reflectY();
    }
    if(ball_loc.x <= 0 && !checkPaddle(0)) {
        ball_loc.x = 40;
        ball_loc.y = 15;
        ball_dir.x = -ball_dir.x;
    }
    if(ball_loc.x >= 72 && !checkPaddle(1)) {
        ball_loc.x = 40;
        ball_loc.y = 15;
        ball_dir.x = -ball_dir.x;
    }
    if(rpoints >= 3) {
        gfwCleanup();
        puts("Player 2 won!", GFW_COLOR_WHITE);
        gfwExit();
    }
    if(lpoints >= 3) {
        gfwCleanup();
        puts("Player 1 won!", GFW_COLOR_WHITE);
        gfwExit();
    }
    drawBall();
    drawPaddle(0, lpos);
    drawPaddle(1, rpos);
    gfwSwapBuffers();
}

int main(int argc, char** argv) {
    gfwStartup();
    ball_loc.x = 40;
    ball_loc.y = 15;
    ball_dir.x = -1;
    ball_dir.y = 1;
    while(1) {
        update();
    }
    return 0;
}