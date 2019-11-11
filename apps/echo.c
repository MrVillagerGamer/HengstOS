#include "libc/sys.h"

int main(int argc, char** argv) {
    if(argc <= 1) {
        puts("No input data.", COLOR_DEFAULT);
        return -1;
    }
    for(int i = 1; i < argc; i++) {
        puts(argv[i], COLOR_DEFAULT);
        puts(" ", COLOR_DEFAULT);
    }
    return 0;
}