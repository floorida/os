#include <stdint.h>
#include <stddef.h>

#define GDT_OFFSET_KERNEL_CODE 0x28

void outb(unsigned short int port, unsigned char value);
unsigned char inb(unsigned short int __port);
void pic_remap(void);
void exception_handler(void);
extern void *isr_stub_table[];