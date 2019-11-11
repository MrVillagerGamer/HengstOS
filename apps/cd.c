#include "libc/sys.h"

int main(int argc, char** argv) {
    if(argc <= 1) {
        puts("No input data.", 0x07);
        return -1;
    }
    chdir(argv[1]);
    return 0;
}