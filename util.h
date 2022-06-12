#include <stdint.h>
#include <stddef.h>

#define GDT_OFFSET_KERNEL_CODE 0x28
#define PIC1 0x20 /* IO base address for master PIC */
#define PIC2 0xA0 /* IO base address for slave PIC */
#define PIC1_COMMAND PIC1
#define PIC1_DATA (PIC1 + 1)
#define PIC2_COMMAND PIC2
#define PIC2_DATA (PIC2 + 1)
#define PIC_EOI 0x20

void pic_unmask(unsigned char);
void pic_eoi(void);
void outb(unsigned short int port, unsigned char value);
unsigned char inb(unsigned short int __port);
void pic_remap(void);
void exception_handler(void);
extern void *isr_stub_table[];