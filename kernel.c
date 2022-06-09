#include "limine.h"

#include "printf.h"

#include <stddef.h>

#include <stdint.h>

#include "util.h"

#include "idt.h"

static volatile struct limine_terminal_request terminal_request = {
    .id = LIMINE_TERMINAL_REQUEST,
    .revision = 0};

__attribute__((interrupt)) void kbd_handler(struct registers_t *registers)
{
  printf("input");
  while (1)
    ;
}

void _start(void)
{
  pic_remap();
  idt_init();
  idt_set_descriptor(0x21, kbd_handler, 0x8E);
  __asm__("sti");
  while (1)
    ;
}