static inline void outb(u16 port, u8 val) {
    __asm volatile("outb %0, %1"
                   :
                   : "a"(val), "Nd"(port));
}
static inline u8 inb(u16 port)
{
    u8 ret;
    __asm volatile ( "inb %1, %0"
                   : "=a"(ret)
                   : "Nd"(port) );
    return ret;
}

void my_reverse(char str[], int len)
{
    u16 start, end;
    char temp;
    for(start=0, end=len-1; start < end; start++, end--) {
        temp = *(str+start);
        *(str+start) = *(str+end);
        *(str+end) = temp;
    }
}
 
char* itoa(int num, char* str, int base)
{
    u16 i = 0;
    u8 isNegative = 0;
  
    if (num == 0) {
        str[i] = '0';
        str[i + 1] = '\0';
        return str;
    }

    if (num < 0 && base == 10) {
        isNegative = 1;
        num = -num;
    }
  
    while (num != 0) {
        int rem = num % base;
        str[i++] = (rem > 9)? (rem-10) + 'A' : rem + '0';
        num = num/base;
    }

    if (isNegative==1){
        str[i++] = '-';
    }
  
    str[i] = '\0';
 
    my_reverse(str, i);
  
    return str;
}
void qemu_debugcon(char* str) {
    while(*str) {
        outb(0xe9,*str); // write to debug port
        str++;
    }
    outb(0xe9,'\n');
}

void sleep(u32 ms) {
	u32 start=time_ms;
	while(1) {
	if(start+ms<time_ms) break;
	}
}