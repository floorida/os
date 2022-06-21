#include <stddef.h>
#include <stdint.h>
#include "util.h"
#include "idt.h"
#include "printf.h"
__attribute__((interrupt)) void kbd_handler(struct registers_t *registers)
{
    printf("input");
    pic_eoi();
    while (1)
        ;
}

__attribute__((aligned(0x10))) idt_entry_t idt[256]; // Create an array of IDT entries; aligned for performance
idtr_t idtr;

void idt_set_descriptor(uint8_t vector, void *isr, uint8_t flags)
{
    idt_desc_t *descriptor = &idt[vector];

    descriptor->base_low = (uint64_t)isr & 0xFFFF;
    descriptor->cs = GDT_OFFSET_KERNEL_CODE;
    descriptor->ist = 0;
    descriptor->attributes = flags;
    descriptor->base_mid = ((uint64_t)isr >> 16) & 0xFFFF;
    descriptor->base_high = ((uint64_t)isr >> 32) & 0xFFFFFFFF;
    descriptor->rsv0 = 0;
}

void idt_init()
{

    idtr.base = (uintptr_t)&idt[0];
    idtr.limit = (uint16_t)sizeof(idt_desc_t) * 256 - 1;

    for (uint8_t vector = 0; vector < 256; vector++)
    {
        idt_set_descriptor(vector, isr_stub_table[vector], 0x8E);
    }

    pic_unmask(0);
    idt_set_descriptor(0x20, kbd_handler, 0x8E);

    __asm__("lidt %0" ::"m"(idtr));
};
