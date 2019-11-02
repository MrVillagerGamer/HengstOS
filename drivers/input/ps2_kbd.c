#include <arch/i386/port.h>
//#include <arch/i386/term.h>
#include <stdint.h>
#include <stdbool.h>

bool shifting = 0;
uint8_t ps2_kbd_asc;

char kbdus1[128] = {
    0,  27, '1', '2', '3', '4', '5', '6', '7', '8', /* 9 */
    '9', '0', '-', '=', '\b',   /* Backspace */
    '\t',           /* Tab */
    'q', 'w', 'e', 'r', /* 19 */
    't', 'y', 'u', 'i', 'o', 'p', '[', ']', '\n',       /* Enter key */
    0,          /* 29   - Control */
    'a', 's', 'd', 'f', 'g', 'h', 'j', 'k', 'l', ';',   /* 39 */
    '\'', '`',   0,     /* Left shift */
    '\\', 'z', 'x', 'c', 'v', 'b', 'n',         /* 49 */
    'm', ',', '.', '/',   0,                    /* Right shift */
    '*',
    0,  /* Alt */
    ' ',    /* Space bar */
    0,  /* Caps lock */
    0,  /* 59 - F1 key ... > */
    0,   0,   0,   0,   0,   0,   0,   0,
    0,  /* < ... F10 */
    0,  /* 69 - Num lock*/
    0,  /* Scroll Lock */
    0,  /* Home key */
    0,  /* Up Arrow */
    0,  /* Page Up */
    '-',
    0,  /* Left Arrow */
    0,
    0,  /* Right Arrow */
    '+',
    0,  /* 79 - End key*/
    0,  /* Down Arrow */
    0,  /* Page Down */
    0,  /* Insert Key */
    0,  /* Delete Key */
    0,   0,   0,
    0,  /* F11 Key */
    0,  /* F12 Key */
    0,  /* All other keys are undefined */
};

char kbdus2[128] = {
    0,  27, '!', '@', '#', '$', '%', '^', '&', '*', /* 9 */
    '(', ')', '_', '+', '\b',   /* Backspace */
    '\t',           /* Tab */
    'Q', 'W', 'E', 'R', /* 19 */
    'T', 'Y', 'U', 'I', 'O', 'P', '{', '}', '\n',       /* Enter key */
    0,          /* 29   - Control */
    'A', 'S', 'D', 'F', 'G', 'H', 'J', 'K', 'L', ':',   /* 39 */
    '"', '~',   0,     /* Left shift */
    '|', 'Z', 'X', 'C', 'V', 'B', 'N',         /* 49 */
    'M', '<', '>', '?',   0,                    /* Right shift */
    '*',
    0,  /* Alt */
    ' ',    /* Space bar */
    0,  /* Caps lock */
    0,  /* 59 - F1 key ... > */
    0,   0,   0,   0,   0,   0,   0,   0,
    0,  /* < ... F10 */
    0,  /* 69 - Num lock*/
    0,  /* Scroll Lock */
    0,  /* Home key */
    0,  /* Up Arrow */
    0,  /* Page Up */
    '-',
    0,  /* Left Arrow */
    0,
    0,  /* Right Arrow */
    '+',
    0,  /* 79 - End key*/
    0,  /* Down Arrow */
    0,  /* Page Down */
    0,  /* Insert Key */
    0,  /* Delete Key */
    0,   0,   0,
    0,  /* F11 Key */
    0,  /* F12 Key */
    0,  /* All other keys are undefined */
};

void ps2_kbd_init() {
	extern void ints_wrapper_33();
	ints_set(33, (uint32_t)ints_wrapper_33);
	while(inb(0x64) & 0x1) {
		inb(0x60);
	}
	outb(0x64, 0xAE);
	outb(0x64, 0x20);
	uint8_t state = inb(0x60);
	outb(0x64, 0x60);
	outb(0x60, state);
	outb(0x60, 0xF4);
	ps2_kbd_asc = 0;
	//inb(0x60);
}

uint8_t ps2_kbd_get() {
	return ps2_kbd_asc;
}

void ps2_kbd_handler() {
	uint8_t data = inb(0x60);
	//term_puth(data);
	if((data & 0x7f) != 0x2a) {
		if(!shifting) ps2_kbd_asc = kbdus1[data & 0x7f];
		else ps2_kbd_asc = kbdus2[data & 0x7f];
	}else{
		shifting = 1;
	}
	if(data & 0x80) {
		if((data & 0x7f) != 0x2a) {
			ps2_kbd_asc = 0;
		}else{
			shifting = 0;
		}
	}
	outb(0x20, 0x20);
}

























