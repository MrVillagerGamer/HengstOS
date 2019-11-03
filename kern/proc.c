#ifndef __INIX_ARCH_I386__
#error Unsupported Architecture
#endif

#include <stdint.h>

int mkproc(void* paddr, int length, int stack, int argc, char** argv) {
#ifdef __INIX_ARCH_I386__
	extern int mseg_alloc(uint32_t,uint32_t);
	int idx = mseg_alloc((uint32_t)paddr, (length >> 12));
	int scode = ((idx*2+6)*8)|3;
	int sdata = ((idx*2+7)*8)|3;
	extern int add_proc(uint32_t,uint32_t,uint32_t,uint32_t,int,char**);
	int pid = add_proc(0, stack, scode, sdata, argc, argv);
	return pid;
#endif	
}

void rmproc(int pid) {
#ifdef __INIX_ARCH_I386__
	extern void del_proc(int);
	del_proc(pid);
#endif
}

void exitproc() {
#ifdef __INIX_ARCH_I386__
	extern int get_current_pid();
	int pid = get_current_pid();
	rmproc(pid);
#endif
}
