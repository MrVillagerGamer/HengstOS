#include <arch/i386/task.h>
#include <arch/i386/port.h>
#include <stdbool.h>

bool task_states[256];
regs_t tasks[256];
int cur_task = 0;

int find_free_pid() {
	int fidx = cur_task;
	bool found = 0;
	while(1) {
		fidx++;
		if(fidx >= 256) {
			fidx = 0;
		}
		if(!task_states[fidx]) {
			break;
		}
	}
	return fidx;
}

int get_next_task() {
	int fidx = cur_task;
	bool found = 0;
	while(1) {
		fidx++;
		if(fidx >= 256) {
			fidx = 0;
		}
		if(task_states[fidx]) {
			break;
		}
	}
	return fidx;
}

void task_init() {
	for(int i = 0; i < 256; i++) {
		task_states[i] = 0;
	}
	task_states[0] = 1;
}

int get_current_pid() {
	return cur_task;
}

void del_proc(int idx) {
	task_states[idx] = 0;
}

int add_proc(uint32_t eip, uint32_t esp, uint32_t cs, uint32_t ds) {
	int idx = find_free_pid();
	tasks[idx] = tasks[cur_task];
	tasks[idx].eip = eip;
	tasks[idx].esp = esp;
	tasks[idx].cs = cs;
	tasks[idx].ds = ds;
	tasks[idx].es = ds;
	tasks[idx].fs = ds;
	tasks[idx].gs = ds;
	tasks[idx].ss = ds;
	task_states[idx] = 1;
	return idx;
}

void switch_task(uint32_t esp) {
	//term_puts("Switching task\n");
	regs_t* state = (regs_t*)(esp);
	tasks[cur_task] = *state;
	cur_task = get_next_task();
	*state = tasks[cur_task];
	outb(0x20, 0x20);
	//term_puts("Switching task\n");
}
