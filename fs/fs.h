#pragma once

#define ERR_RDONLY -1
#define ERR_INVAL -2

typedef struct {
	int(*read)(const char*,char*,int);
	int(*write)(const char*,const char*,int);
} fs_ops_t;

extern int fs_write(const char*,const char*,int);
extern int fs_read(const char*,char*,int);
