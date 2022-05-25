#include <stdint.h>
#include <stddef.h>
#include <limine.h>
#include "util.h"
#include "printf.h"

static volatile struct limine_framebuffer_request framebuffer_request = {
    .id = LIMINE_FRAMEBUFFER_REQUEST,
    .revision = 0
};

void write_fb(int x, int y, uint32_t pixel) {
    struct limine_framebuffer *framebuffer = framebuffer_request.response->framebuffers[0];
    void *fb_address = framebuffer->address;
    int pitch = framebuffer->pitch;
    char * height = framebuffer->height;
    char * width = framebuffer->width;
    *((uint32_t*)(fb_address + 4 * pitch * y + 4 * x)) = pixel;
}

void specs() {
    char * buffer[255];
    struct limine_framebuffer *framebuffer = framebuffer_request.response->framebuffers[0];
    void *fb_address = framebuffer->address;
    uint16_t width = framebuffer->width;
    uint16_t height = framebuffer->height;
    printf("H %d W %d", height, width);
}