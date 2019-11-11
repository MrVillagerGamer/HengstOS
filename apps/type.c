#include "libc/sys.h"

#define MAX_FILE_SIZE 800

int main(int argc, char** argv) {
    if(argc <= 1) {
        puts("No input files.", COLOR_DEFAULT);
        return -1;
    }
    for(int i = 1; i < argc; i++) {
        char buffer[MAX_FILE_SIZE];
        read(argv[i], buffer, MAX_FILE_SIZE);
        puts(buffer, COLOR_DEFAULT);
    }
    return 0;
}