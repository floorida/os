#include "limine.h"

#include "printf.h"

#include <stddef.h>

#include <stdint.h>

#include "util.h"

#define GDT_OFFSET_KERNEL_CODE 0x28

struct registers_t
{
    uint64_t rip, cs, rflags, rsp, ss;
} __attribute__((packed));

typedef struct {
  uint16_t base_low; // offset bits 0..15
  uint16_t cs; // a code segment selector in GDT or LDT
  uint8_t ist; // bits 0..2 holds Interrupt Stack Table offset, rest of bits zero.
  uint8_t attributes; // gate type, dpl, and p fields
  uint16_t base_mid; // offset bits 16..31
  uint32_t base_high; // offset bits 32..63
  uint32_t rsv0; // reserved
}
__attribute__((packed)) idt_desc_t;

typedef struct {
  uint16_t limit;
  uint64_t base;
}
__attribute__((packed)) idtr_t;

static idtr_t idtr;

typedef struct {
  uint16_t isr_low; // The lower 16 bits of the ISR's address
  uint16_t kernel_cs; // The GDT segment selector that the CPU will load into CS before calling the ISR
  uint8_t ist; // The IST in the TSS that the CPU will load into RSP; set to zero for now
  uint8_t attributes; // Type and attributes; see the IDT page
  uint16_t isr_mid; // The higher 16 bits of the lower 32 bits of the ISR's address
  uint32_t isr_high; // The higher 32 bits of the ISR's address
  uint32_t reserved; // Set to zero
}
__attribute__((packed)) idt_entry_t;

static idt_entry_t * idt[256]; // Create an array of IDT entries; aligned for performance

void idt_set_descriptor(uint8_t vector, void * isr, uint8_t flags) {
  idt_desc_t * descriptor = & idt[vector];

  descriptor -> base_low = (uint64_t) isr & 0xFFFF;
  descriptor -> cs = GDT_OFFSET_KERNEL_CODE;
  descriptor -> ist = 0;
  descriptor -> attributes = flags;
  descriptor -> base_mid = ((uint64_t) isr >> 16) & 0xFFFF;
  descriptor -> base_high = ((uint64_t) isr >> 32) & 0xFFFFFFFF;
  descriptor -> rsv0 = 0;
}

static volatile struct limine_terminal_request terminal_request = {
  .id = LIMINE_TERMINAL_REQUEST,
  .revision = 0
};

__attribute__ ((interrupt))
void kbd_handler(struct registers_t *registers)
{
  printf("input");
}


extern void * isr_stub_table[];

void idt_init() {
  idtr.base = (uintptr_t) & idt[0];
  idtr.limit = (uint16_t) sizeof(idt_desc_t) * 256 - 1;
  __asm__("lidt %0"::"m"(idtr));
  __asm__("sti");
};

void exception_handler() {
  __asm__("cli; hlt");
}

void _start(void) {
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
  idt_init();
  idt_set_descriptor(0x21, kbd_handler, 0x8E);
  printf("%x", idtr.base);
}