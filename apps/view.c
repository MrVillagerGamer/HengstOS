#define MAX_BMP_SIZE 65536
#include "libc/sys.h"
#include "libc/gfw.h"

unsigned char buf[MAX_BMP_SIZE];

int main(int argc, char** argv) {
	if(argc <= 1) return 1;
	if(argc >= 2) {
		int bth = 128;
		if(argc == 3) {
			int mult = argv[2][0]-48;
			bth = 32 + mult * 16;
		}
		gfwStartup();
		char* filename = argv[1];
		read(filename, buf, MAX_BMP_SIZE);
		int width = *(int*)&buf[18];
		int height = *(int*)&buf[22];
		int stepX = width/80;
		int stepY = height/50;
		int pad = ((width*3+4)&(~4))-width;
		for(int x = 0; x < 80; x++) {
			for(int y = 0; y < 50; y++) {
				int cx = x * stepX;
				int cy = y * stepY;
				unsigned char r = buf[(cx+width*cy)*3+54];
				unsigned char g = buf[(cx+width*cy)*3+55];
				unsigned char b = buf[(cx+width*cy)*3+56];
				unsigned char color = (((b / bth)>0?1:0) << 2) | (((g / bth)>0?1:0) << 1) | (((r / bth)>0?1:0) << 0);
				gfwPlotPixel(x, 48-y, color);
			}
		}
		gfwSwapBuffers();
		while(getc());
		while(!getc());
		gfwCleanup();
		gfwExit();
	}
}
