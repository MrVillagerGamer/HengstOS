#include "libc/sys.h"
#include "libc/gfw.h"
#include <stdbool.h>

typedef struct {
    int x, y;
} coord_t;

coord_t asteroids[64];
coord_t asteroidDirections[64];
bool asteroidPresents[64];
coord_t beamPosition;

int asteroidCount = 5;
int shipPosition = 40;
int countdown = 0;
int elapsedTime = 0;

void initAsteroids() {
    for(int i = 0; i < asteroidCount; i++) {
        asteroidPresents[i] = 1;
        asteroids[i].x = (gfwNoise1d(i*4+0) % 72);
        asteroids[i].y = (gfwNoise1d(i*4+1) % 38);
        asteroidDirections[i].x = gfwNoise1d(i*4+2)<0?-1:1;
        asteroidDirections[i].y = gfwNoise1d(i*4+3)<0?-1:1;
        if(asteroids[i].x < 0) {
            asteroids[i].x = -asteroids[i].x + 4;
        }else{
            asteroids[i].x += 4;
        }
        if(asteroids[i].y < 0) {
            asteroids[i].y = -asteroids[i].y + 4;
        }else{
            asteroids[i].y += 4;
        }
        int y = asteroids[i].y;
        int x = asteroids[i].x;
        if(y >= 40 && y <= 42) {
            if(x >= shipPosition-1 && x <= shipPosition+1) {
                asteroidPresents[i] = 0;
            }
        }
    }
}

void drawSpaceship() {
    if(getc() == 119 && shipPosition < 76) {
        shipPosition++;
    }else if(getc() == 113 && shipPosition > 4) {
        shipPosition--;
    }
    if(getc() == 97 && countdown > 8 ) {
        beamPosition.x = shipPosition;
        countdown = 0;
    }
    beamPosition.y = 42 - countdown * 4;
    countdown++;

    bool noAsteroids = true;
    for(int i = 0; i < asteroidCount; i++) {
        if(!asteroidPresents[i]) {
            continue;
        }
        noAsteroids = false;
        int y = asteroids[i].y;
        int x = asteroids[i].x;
        if(beamPosition.x < x+2 && beamPosition.x > x-2) {
            if(y > beamPosition.y-4 && y < beamPosition.y+4) {
                asteroidPresents[i] = 0;
            }
        }
        if(y >= 40 && y <= 42) {
            if(x >= shipPosition-1 && x <= shipPosition+1) {
                gfwCleanup();
                puts("Ouch! Maybe you'll win next time.", COLOR_DEFAULT);
                gfwExit();
                while(1);
            }
        }
    }
    if(noAsteroids) {
        gfwCleanup();
        puts("Yes! You invaded the asteroid belt!", COLOR_DEFAULT);
        gfwExit();
        while(1);
    }

    if(beamPosition.y > 4) {
        gfwPlotPixel(beamPosition.x, beamPosition.y-3, GFW_COLOR_DEFAULT);
        gfwPlotPixel(beamPosition.x, beamPosition.y-2, GFW_COLOR_DEFAULT);
        gfwPlotPixel(beamPosition.x, beamPosition.y-1, GFW_COLOR_DEFAULT);
        gfwPlotPixel(beamPosition.x, beamPosition.y, GFW_COLOR_DEFAULT);
        gfwPlotPixel(beamPosition.x, beamPosition.y+1, GFW_COLOR_DEFAULT);
        gfwPlotPixel(beamPosition.x, beamPosition.y+2, GFW_COLOR_DEFAULT);
        gfwPlotPixel(beamPosition.x, beamPosition.y+3, GFW_COLOR_DEFAULT);
    }
    

    gfwPlotPixel(shipPosition+2, 42, GFW_COLOR_DEFAULT);
    gfwPlotPixel(shipPosition+1, 42, GFW_COLOR_DEFAULT);
    gfwPlotPixel(shipPosition, 42, GFW_COLOR_DEFAULT);
    gfwPlotPixel(shipPosition-1, 42, GFW_COLOR_DEFAULT);
    gfwPlotPixel(shipPosition-2, 42, GFW_COLOR_DEFAULT);

    gfwPlotPixel(shipPosition+1, 41, GFW_COLOR_DEFAULT);
    gfwPlotPixel(shipPosition, 41, GFW_COLOR_DEFAULT);
    gfwPlotPixel(shipPosition-1, 41, GFW_COLOR_DEFAULT);
    gfwPlotPixel(shipPosition, 40, GFW_COLOR_DEFAULT);
}

void drawAsteroids() {
    for(int i = 0; i < asteroidCount; i++) {
        if(!asteroidPresents[i]) {
            continue;
        }
        int y = asteroids[i].y;
        int x = asteroids[i].x;
        gfwPlotPixel(x-1, y-1, GFW_COLOR_DEFAULT);
        gfwPlotPixel(x-1, y, GFW_COLOR_DEFAULT);
        gfwPlotPixel(x-1, y+1, GFW_COLOR_DEFAULT);
        gfwPlotPixel(x, y-1, GFW_COLOR_DEFAULT);
        gfwPlotPixel(x, y, GFW_COLOR_DEFAULT);
        gfwPlotPixel(x, y+1, GFW_COLOR_DEFAULT);
        gfwPlotPixel(x+1, y-1, GFW_COLOR_DEFAULT);
        gfwPlotPixel(x+1, y, GFW_COLOR_DEFAULT);
        gfwPlotPixel(x+1, y+1, GFW_COLOR_DEFAULT);
    }
    for(int i = 0; i < asteroidCount; i++) {
        if(!asteroidPresents[i]) {
            continue;
        }
        if(asteroids[i].x <= 4 || asteroids[i].x >= 76) {
            asteroidDirections[i].x = -asteroidDirections[i].x;
        }
        if(asteroids[i].y <= 4 || asteroids[i].y >= 42) {
            asteroidDirections[i].y = -asteroidDirections[i].y;
        }
    }
    for(int i = 0; i < asteroidCount; i++) {
        if(!asteroidPresents[i]) {
            continue;
        }
        asteroids[i].x += asteroidDirections[i].x;
        asteroids[i].y += asteroidDirections[i].y;
    }
}

void update() {
    gfwDelay(30);
    gfwClearScreen();
    drawSpaceship();
    drawAsteroids();
    gfwSwapBuffers();
    int k = getc();
    if(k == 27) {
        gfwCleanup();
        gfwExit();
    }
    elapsedTime++;
}

void main() {
    gfwStartup();
    initAsteroids();
    while(1) update();
    return 0;
}
