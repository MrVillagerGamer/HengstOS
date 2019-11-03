#include <fs/fs.h>
#include <stdint.h>
#include <stdbool.h>
#ifdef __INIX_ARCH_I386__
#include <arch/i386/term.h>
#endif

uint32_t rd_addr;
char* rdr = "/";

extern int strlen(const char*);
extern bool strncmp(const char*,const char*, int);

void decode_rd(char* d, const char* s) {
	int i;
	d[0] = '\\';
	for(i = 0; i < strlen(s); i++) {
		d[i+1] = s[i];
		if(d[i+1] == '/') {
			d[i+1] = '\\';
		}
	}
	d[i+1] = 0;
}

void encode_rd(char* d, const char* s) {
	int i;
	for(i = 0; i < strlen(s)-1; i++) {
		d[i] = s[i+1];
		if(d[i] == '\\') {
			d[i] = '/';
		}
	}
	d[i+1] = 0;
}

typedef struct {
    char filename[100];
    char mode[8];
    char uid[8];
    char gid[8];
    char size[12];
    char mtime[12];
    char chksum[8];
    char typeflag[1];
} tar_header;

unsigned int rd_get_size(const char *in) {
 
    unsigned int size = 0;
    unsigned int j;
    unsigned int count = 1;
 
    for (j = 11; j > 0; j--, count *= 8)
        size += ((in[j - 1] - '0') * count);
 
    return size;
 
}

tar_header* rd_get_hdr_ptr(const char* path) {
	uint32_t addr = rd_addr;
	for (int i = 0; ; i++) {
		tar_header* header = (tar_header*)addr;
		if (header->filename[0] == '\0')
			break;
		unsigned int size = rd_get_size(header->size);
		char filename[256];
		decode_rd(filename, header->filename);
		if(strncmp(path, filename, strlen(filename)+1)) {
			return header;
		}
		addr += ((size / 512) + 1) * 512;
		if(size % 512)
			addr += 512;
	}
	return (tar_header*)0;
}

int rd_size(const char* path) {
	tar_header* ptr = rd_get_hdr_ptr(path);
	if(ptr == 0) {
		return ERR_INVAL;
	}
	return (int)rd_get_size(ptr->size);
}

int rd_read(const char* path, char* buf, int len) {
	tar_header* ptr = rd_get_hdr_ptr(path);
	if(ptr == 0) {
		return ERR_INVAL;
	}
	int size = (int)rd_get_size(ptr->size);
	int j;
	for(j = 0; j < size && j < len; j++) {
		buf[j] = ((char*)ptr)[j+512];
	}
	return j;
}

int rd_write(const char* path, const char* buf, int len) {
	return ERR_RDONLY;
}

fs_ops_t rdops = {
	rd_read,
	0,
	rd_size,
};

extern void fs_map_drive(int, char, fs_ops_t*);


void rd_init(uint32_t init_addr) {
	rd_addr = init_addr;
	fs_map_drive(0, 'Z', &rdops);
	return;
}


