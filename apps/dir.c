#include "libc/sys.h"
#include "libc/lib.h"

#define MAX_ENTS 32

char buffer[MAX_ENTS][128];
char* paths[MAX_ENTS];

int wgetc() {
    while(getc());
    while(!getc());
    int key = getc();
    return key;
}

void print_list(const char* path) {
    for(int i = 0; i < MAX_ENTS; i++) {
        for(int j = 0; j < 128; j++) {
            buffer[i][j] = 0;
        }
        paths[i] = buffer[i];
    }
    list(path, paths, MAX_ENTS, 128);
    for(int i = 0; i < MAX_ENTS; i++) {
        if(paths[i][0] == 0) {
            return;
        }
        puts(paths[i], 0x07);
        puts("\n", 0x07);
        if((i % 23) == 22) {
            puts("--more--\n", 0x07);
            while(wgetc() != ' ');
        }
    }
}

int main(int argc, char** argv) {
    if(argc <= 1) {
        print_list("");
    }else{
        print_list(argv[1]);
    }
    return 0;
}