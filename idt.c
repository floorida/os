#include <stddef.h>
#include <stdint.h>
#include "util.h"
#include "idt.h"

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

    for (uint8_t vector = 0; vector < 255; vector++)
    {
        idt_set_descriptor(vector, isr_stub_table[vector], 0x8E);
    }

    __asm__("lidt %0" ::"m"(idtr));
};
