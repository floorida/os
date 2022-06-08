#include <stddef.h>
#include <stdint.h>

struct registers_t
{
    uint64_t rip, cs, rflags, rsp, ss;
} __attribute__((packed));

typedef struct
{
    uint16_t base_low;  // offset bits 0..15
    uint16_t cs;        // a code segment selector in GDT or LDT
    uint8_t ist;        // bits 0..2 holds Interrupt Stack Table offset, rest of bits zero.
    uint8_t attributes; // gate type, dpl, and p fields
    uint16_t base_mid;  // offset bits 16..31
    uint32_t base_high; // offset bits 32..63
    uint32_t rsv0;      // reserved
}
__attribute__((packed)) idt_desc_t;

typedef struct
{
    uint16_t limit;
    uint64_t base;
}
__attribute__((packed)) idtr_t;

static idtr_t idtr;

typedef struct
{
    uint16_t isr_low;   // The lower 16 bits of the ISR's address
    uint16_t kernel_cs; // The GDT segment selector that the CPU will load into CS before calling the ISR
    uint8_t ist;        // The IST in the TSS that the CPU will load into RSP; set to zero for now
    uint8_t attributes; // Type and attributes; see the IDT page
    uint16_t isr_mid;   // The higher 16 bits of the lower 32 bits of the ISR's address
    uint32_t isr_high;  // The higher 32 bits of the ISR's address
    uint32_t reserved;  // Set to zero
}
__attribute__((packed)) idt_entry_t;

void idt_set_descriptor(uint8_t, void *, uint8_t);
void idt_init(void);

__attribute__((aligned(0x10))) static idt_entry_t idt[256]; // Create an array of IDT entries; aligned for performance