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

int strhas(const char* s, char c) {
	int i = 0;
	int cnt = 0;
	while(s[i]) {
		if(s[i] == c) {
			cnt++;
		}
		i++;
	}
	return cnt;
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

void vfs_abs_path(char* path2, const char* path) {
	int i, j;
	for(i = 0; i < strlen(cwd); i++) {
		path2[i] = cwd[i];
	}
	path2[i] = '\\';
	for(j = 0; j < strlen(path); j++) {
		path2[i+j+1] = path[j];
	}
	path2[i+j+1] = 0;
}

int vfs_get_drv_idx(const char* path) {
	char drv = path[0];
	if(path[1] == ':') {
		for(int i = 0; i < MAX_DRIVES; i++) {
			if(drives[i] == drv) {
				return i;
			}
		}
	}
	return -1;
}

int fs_list(const char* path, char** list, int count, int len) {
	int idx = vfs_get_drv_idx(path);
	if(idx == -1) {
		char path2[256];
		vfs_abs_path(path2, path);
		idx = vfs_get_drv_idx(path2);
		if(idx == -1) {
			return ERR_INVAL;
		}
		return fs_ops[idx]->list(path2+2, list, count, len);
	}
	if(fs_ops[idx] == 0) {
		return ERR_RDONLY;
	}
	return fs_ops[idx]->list(path+2, list, count, len);
}

int fs_size(const char* path) {
	int idx = vfs_get_drv_idx(path);
	if(idx == -1) {
		char path2[256];
		vfs_abs_path(path2, path);
		idx = vfs_get_drv_idx(path2);
		if(idx == -1) {
			return ERR_INVAL;
		}
		return fs_ops[idx]->size(path2+2);
	}
	if(fs_ops[idx] == 0) {
		return ERR_RDONLY;
	}
	return fs_ops[idx]->size(path+2);
}

int fs_read(const char* path, char* buff, int len) {
	int idx = vfs_get_drv_idx(path);
	if(idx == -1) {
		char path2[256];
		vfs_abs_path(path2, path);
		idx = vfs_get_drv_idx(path2);
		if(idx == -1) {
			return ERR_INVAL;
		}
		return fs_ops[idx]->read(path2+2, buff, len);
	}
	if(fs_ops[idx] == 0) {
		return ERR_RDONLY;
	}
	return fs_ops[idx]->read(path+2, buff, len);
}

































































