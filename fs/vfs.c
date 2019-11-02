#include <fs/fs.h>
#include <stdint.h>
#include <stdbool.h>

#define MAX_DRIVES 64

char cwd[256] = "Z:\\HengstOS\\System32";

int strlen(const char* s) {
	int i = 0;
	while(s[i]) i++;
	return i;
}

bool strncmp(const char* s1, const char* s2, int len) {
	for(int i = 0; i < len; i++) {
		if(s1[i] != s2[i]) {
			return 0;
		}
	}
	return 1;
}

char* fs_getcwd() {
	return cwd;
}

void fs_chdir(const char* path) {
	if(strncmp(path, ".", strlen(path)+1)) {
		return;
	}
	char temp[256];
	for(int i = 0; i < strlen(cwd)+1; i++) {
		temp[i] = cwd[i];
	}
	if(strncmp(path, "..", strlen(path)+1)) {
		int i;
		for(i = strlen(temp)-1; i >= 0; i--) {
			if(temp[i] == '\\') {
				temp[i] = 0;
				break;			
			}
			temp[i] = 0;
		}
		char buff[1];
		char test[256];
		for(i = 0; i < strlen(temp); i++) test[i] = temp[i];
		test[i] = '\\';
		test[i+1] = 0;
		if(fs_read(test, buff, 1) >= 0) {
			for(int i = 0; i < strlen(temp)+1; i++) cwd[i] = temp[i];
		}
	}else{
		int i;
		temp[strlen(temp)] = '\\';
		for(i = 0; i < strlen(path); i++) {
			temp[strlen(cwd)+i+1] = path[i];
		}
		temp[strlen(temp)] = 0;
		char buff[1];
		char test[256];
		for(i = 0; i < strlen(temp); i++) test[i] = temp[i];
		test[i] = '\\';
		test[i+1] = 0;
		if(fs_read(test, buff, 1) >= 0) {
			for(int i = 0; i < strlen(temp)+1; i++) cwd[i] = temp[i];
		}
	}
	
}

fs_ops_t* fs_ops[MAX_DRIVES];
char drives[MAX_DRIVES];

void fs_map_drive(int index, char drv, fs_ops_t* ops) {
	drives[index] = drv;
	fs_ops[index] = ops;
}

int fs_write(const char* path, const char* buff, int len) {
	char drv = path[0];
	int idx = -1;
	if(path[1] == ':') {
		for(int i = 0; i < MAX_DRIVES; i++) {
			if(drives[i] == drv) {
				idx = i;
				break;
			}
		}
	}
	if(idx == -1) {
		return ERR_INVAL;
	}
	if(fs_ops[idx] == 0) {
		return ERR_RDONLY;
	}
	return fs_ops[idx]->write(path+2, buff, len);
}

int fs_read(const char* path, char* buff, int len) {
	char drv = path[0];
	int idx = -1;
	if(path[1] == ':') {
		for(int i = 0; i < MAX_DRIVES; i++) {
			if(drives[i] == drv) {
				idx = i;
				break;
			}
		}
	}
	if(idx == -1) {
		char path2[256];
		int i, j;
		for(i = 0; i < strlen(cwd); i++) {
			path2[i] = cwd[i];
		}
		path2[i] = '\\';
		for(j = 0; j < strlen(path); j++) {
			path2[i+j+1] = path[j];
		}
		path2[i+j+1] = 0;
		drv = path2[0];
		if(path2[1] == ':') {
			for(int i = 0; i < MAX_DRIVES; i++) {
				if(drives[i] == drv) {
					idx = i;
					break;
				}
			}
		}
		//term_puts(path2);
		if(idx == -1) {
			return ERR_INVAL;
		}
		//term_puts(path2+2);
		return fs_ops[idx]->read(path2+2, buff, len);
		//return ERR_INVAL;
	}
	if(fs_ops[idx] == 0) {
		return ERR_RDONLY;
	}
	return fs_ops[idx]->read(path+2, buff, len);
}

































































