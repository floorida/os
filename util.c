#include <stdint.h>
#include <stddef.h>
#include "util.h"
#include "printf.h"

void outb(unsigned short int port, unsigned char value)
{
    __asm__("outb %b0,%w1"
            :
            : "a"(value), "Nd"(port));
}

void pic_remap()
{
    outb(0x20, 0x11);
    outb(0xA0, 0x11);
    outb(0x21, 0x20);
    outb(0xA1, 0x28);
    outb(0x21, 0x04);
    outb(0xA1, 0x02);
    outb(0x21, 0x01);
    outb(0xA1, 0x01);
    outb(0x21, 0x00);
    outb(0xA1, 0x00);
}

unsigned char inb(unsigned short int __port)
{
    unsigned char _v;

    __asm__ __volatile__("inb %w1,%0"
                         : "=a"(_v)
                         : "Nd"(__port));
    return _v;
}

void exception_handler()
{
    printf("[KERNEL] Exception occured.\n");
}