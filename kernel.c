#include "limine.h"

#include "printf.h"

#include <stddef.h>

#include <stdint.h>

#include "util.h"

#include "idt.h"

static volatile struct limine_terminal_request terminal_request = {
    .id = LIMINE_TERMINAL_REQUEST,
    .revision = 0};

void _start(void)
{
  pic_remap();
  idt_init();
  __asm__("sti");
  while (1)
    ;
}