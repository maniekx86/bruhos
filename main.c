// bruhos 0.01

#include <stdint.h>

typedef uint8_t u8;
typedef uint16_t u16;
typedef uint32_t u32;
typedef uint64_t u64;

u32 time_ms;

#include "memaccess.c"
#include "misc.c"
#include "mathmisc.c"
#include "disk.c"

char *ivt = (u8*)0x0000; 

void set_timer_phase(u32 hz) {
	u32 divisor = 1193180 / hz;
	outb(0x43, 0x36);
	u8 l = (u8)(divisor & 0xFF);
	u8 h = (u8)((divisor >> 8) & 0xFF);
	outb(0x40, l);
	outb(0x40, h);
}


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
	
	u32 pointer;

	set_timer_phase(1000);

	// nmi interrupt (0x08) 
	pointer = (void *)nmi_handler;
	ivt[0x08] = (u8)(pointer & 0xFF);
	ivt[0x09] = (u8)((pointer >> 8) & 0xFF);
	ivt[0x0A] = (u8)((pointer >> 16) & 0xFF);
	ivt[0x0B] = (u8)((pointer >> 24) & 0xFF);

	// PIC interrupt (0x20)
	pointer = (void *)pic_handler;
	ivt[0x20] = (u8)(pointer & 0xFF);
	ivt[0x21] = (u8)((pointer >> 8) & 0xFF);
	ivt[0x22] = (u8)((pointer >> 16) & 0xFF);
	ivt[0x23] = (u8)((pointer >> 24) & 0xFF);

	// keyboard interrupt (0x24)
	pointer = (void *)keyboard_hanlder;
	ivt[0x24] = (u8)(pointer & 0xFF);
	ivt[0x25] = (u8)((pointer >> 8) & 0xFF);
	ivt[0x26] = (u8)((pointer >> 16) & 0xFF);
	ivt[0x27] = (u8)((pointer >> 24) & 0xFF);
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
