#ifndef VBE_MODE_H
#define VBE_MODE_H

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

#define VBE_MODE_DEFAULT 0x13
#define VBE_MODE_640_480_MONO 0x11

#define VBE_ATTR_LINEAR_FRAMEBUFFER 0x80

#define VBE_MEMORY_MODEL_PACKED_PIXEL 4
#define VBE_MEMORY_MODEL_DIRECT_COLOR 6

typedef struct {
    uint16_t mode_number;
    uint16_t width;
    uint16_t height;
    uint8_t bits_per_pixel;
    uint8_t memory_model;
    uint16_t attributes;
} VbeMode;

typedef struct {
    uint16_t width;
    uint16_t height;
    uint8_t bits_per_pixel;
} VbeModeRequest;

uint16_t vbe_select_mode(
    const VbeModeRequest *request,
    const VbeMode *modes,
    size_t mode_count
);

#endif
