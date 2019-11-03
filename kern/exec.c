#include <stdint.h>
#include <stdbool.h>
#include <fs/fs.h>

int exec(const char* path, int argc, char** argv) {
	int size = fs_size(path);
	if(size < 0) return size;
	size += 0x10000*2;
	void* ptr = kalloc(size/0x10000);
	fs_read(path, ptr, size);
	char** argv1 = (char**)(ptr+size-0x10000);
	for(int i = 0; i < argc; i++) {
		argv1[i] = (char*)(ptr+size-0x10000+1024*(i+1));
		for(int j = 0; j < strlen(argv[i])+1; j++) {
			argv1[i][j] = argv[i][j];
		}
		argv1[i] = (char*)((uint32_t)(argv1[i])-(uint32_t)ptr);
	}
	argv1 = (char**)((uint32_t)argv1-(uint32_t)ptr);
	int pid = mkproc(ptr, size, size-1, argc, argv1);
	return pid;
}
