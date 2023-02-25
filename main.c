// main.c
/*
	Main file
	Written by maniek86 2023 (c) 
*/

#include <stdint.h>

typedef uint8_t u8;
typedef uint16_t u16;
typedef uint32_t u32;
typedef uint64_t u64;

u32 time_ms;

#include "memaccess.c"
#include "misc.c"
#include "ivt.c"
#include "math.c"
#include "disk.c"

void nmi_handler() {
	__asm("pusha");
	__asm("nop;nop;nop;nop");
	outb(0x20, 0x20);
	__asm("popa;leave;iret");
}

void keyboard_hanlder() {
	__asm("pusha");
	
	__asm("nop;nop;nop;nop");
	outb(0x20, 0x20);
	__asm("popa;leave;iret");
}

void pic_handler() { // PIC is set to 1000 Hz
	__asm("pusha");

	time_ms=time_ms+1;

	__asm("nop;nop;nop;nop");
	outb(0x20, 0x20);
	__asm("popa;leave;iret");
}

void interrupt_setup() {
	set_timer_hz(1000);

	ivt_set_callback(&nmi_handler,2);
	ivt_set_callback(&pic_handler,8); //8-8=irq 0
	ivt_set_callback(&keyboard_hanlder,9); //9-8=irq 1
}

void main(void) {

	__asm("cli");
	interrupt_setup();
	__asm("sti");
	outb(0xe9,'1'); // qemu debugcon
	resetDisk(0);
	
	
	int x=0;
	int y=0;
	u32 t=0;
	while(1) {
		x=0;
		y=0;
		while(y<25) {
			while(x<80) {
				dispchar(chr('#',15),(y*80+x)*2);
				x++;
			}
			x=0;
			y++;
		}
		t++;
		sleep(16);
	}

    while (1) {
        __asm("nop");
    };
}
