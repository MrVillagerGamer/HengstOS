#include <arch/i386/sys.h>
#include <stdint.h>
#include <arch/i386/term.h>
#include <fs/fs.h>
#include <stdbool.h>

extern uint32_t get_data_base(uint16_t);
extern uint8_t ps2_kbd_get();

void sys_handler(uint32_t eax, uint32_t ds) {
	uint32_t* data = (uint32_t*)(eax+get_data_base(ds));
	if(data[0] == 0) { // write terminated buffer to console
		sys_puts_info_t* info = (sys_puts_info_t*)(data[1]+get_data_base(ds));
		term_color(info->color);
		term_puts((char*)(info->buffer+get_data_base(ds)));
	}
	if(data[0] == 1) { // read character
		sys_getc_info_t* info = (sys_getc_info_t*)(data[1]+get_data_base(ds));
		uint8_t c = ps2_kbd_get();
		info->data = c;
	}
	if(data[0] == 2) { // read buffer from file
		sys_read_info_t* info = (sys_read_info_t*)(data[1]+get_data_base(ds));
		fs_read(info->path+get_data_base(ds), info->buffer+get_data_base(ds), info->count);
	}
	if(data[0] == 3) { // write buffer to file
		sys_write_info_t* info = (sys_write_info_t*)(data[1]+get_data_base(ds));
		fs_write(info->path+get_data_base(ds), info->buffer+get_data_base(ds), info->count);
	}
	if(data[0] == 4) { // change current directory
		sys_chdir_info_t* info = (sys_chdir_info_t*)(data[1]+get_data_base(ds));
		fs_chdir((char*)(info->buffer+get_data_base(ds)));
	}
	if(data[0] == 5) { // get current directory
		sys_getcwd_info_t* info = (sys_getcwd_info_t*)(data[1]+get_data_base(ds));
		char* cwd = fs_getcwd();
		char* dest = (info->buffer+get_data_base(ds));
		for(int i = 0; i < strlen(cwd)+1; i++) dest[i] = cwd[i];
	}
	if(data[0] == 6) { // clear console screen
		term_clear();
	}
	if(data[0] == 7) { // execute process
		extern int exec(const char*,int,char**);
		sys_exec_info_t* info = (sys_exec_info_t*)(data[1]+get_data_base(ds));
		char** argv = (char**)((uint32_t)(info->argv)+get_data_base(ds));
		for(int i = 0; i < info->argc; i++) {
			argv[i] = (char*)((uint32_t)(argv[i])+get_data_base(ds));
		}
		int pid = exec(info->path+get_data_base(ds), info->argc, argv);
		for(int i = 0; i < info->argc; i++) {
			argv[i] = (char*)((uint32_t)(argv[i])-get_data_base(ds));
		}
		info->handle = pid;
	}
	if(data[0] == 8) { // get process state
		sys_state_info_t* info = (sys_state_info_t*)(data[1]+get_data_base(ds));
		extern bool get_pid_state(int);
		info->state = (int)get_pid_state(info->handle);
	}
	if(data[0] == 9) { // terminate process
		rmproc(get_current_pid());
	}
	if(data[0] == 10) {
		sys_goto_info_t* info = (sys_goto_info_t*)(data[1]+get_data_base(ds));
		term_goto(info->x, info->y);
	}
	if(data[0] == 11) {
		sys_uptime_info_t* info = (sys_uptime_info_t*)(data[1]+get_data_base(ds));
		info->data = get_uptime();
	}
	if(data[0] == 12) {
		sys_list_info_t* info = (sys_list_info_t*)(data[1]+get_data_base(ds));
		char** list = (char**)((uint32_t)(info->list)+get_data_base(ds));
		for(int i = 0; i < info->count; i++) {
			list[i] += get_data_base(ds);
		}
		fs_list(info->path+get_data_base(ds), list, info->count, info->len);
		for(int i = 0; i < info->count; i++) {
			list[i] -= get_data_base(ds);
		}
	}
}


































