#define GFW_COLOR_BLACK 0
#define GFW_COLOR_BLUE 1
#define GFW_COLOR_GREEN 2
#define GFW_COLOR_CYAN 2
#define GFW_COLOR_RED 4
#define GFW_COLOR_MAGENTA 5
#define GFW_COLOR_YELLOW 6
#define GFW_COLOR_WHITE 7

#include <stdint.h>

extern void gfwStartup();
extern void gfwCleanup();
extern void gfwClearScreen();
extern void gfwPlotPixel(int x, int y, int color);
extern void gfwDrawText(int x, int y, const char* str, int color);
// not yet implemented
//extern void gfwDrawLine(int x1, int y1, int x2, int y2, int fg);
extern void gfwDrawRect(int x1, int y1, int x2, int y2, int color);
extern void gfwFillRect(int x1, int y1, int x2, int y2, int color);
extern void gfwSwapBuffers();
extern void gfwExit();
extern void gfwDelay(int t);
extern int gfwGetTime();