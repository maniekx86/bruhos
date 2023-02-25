# Simple x86 real-mode operating system 

BruhOS is simple x86 real-mode operating system written in C. It uses GCC to generate code for i386 and higher. Currently it fills screen with `#`

## Compiling and running
Required packages: nasm, gcc and mtools

compile: ./compile.sh

run: qemu-system-i386 -fda ./floppy.img<br>
debug run: qemu-system-j386 -fda ./floppy.img -debugcon stdio
