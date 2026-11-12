#include "vbe_mode.h"

#include <assert.h>
#include <stdio.h>

#define MODE(width, height, depth, number) \
    {                                      \
        .mode_number = number,             \
        .width = width,                    \
        .height = height,                  \
        .bits_per_pixel = depth,           \
        .memory_model = VBE_MEMORY_MODEL_DIRECT_COLOR, \
        .attributes = VBE_ATTR_LINEAR_FRAMEBUFFER     \
    }

static void test_exact_match(void)
{
    const VbeMode modes[] = {
        MODE(800, 600, 32, 0x115),
        MODE(1024, 768, 32, 0x118)
    };

    const VbeModeRequest request = {800, 600, 32};

    assert(vbe_select_mode(&request, modes, 2) == 0x115);
}

static void test_resolution_selection(void)
{
    const VbeMode modes[] = {
        MODE(800, 600, 32, 0x115),
        MODE(1024, 768, 32, 0x118),
        MODE(640, 480, 32, 0x112)
    };

    const VbeModeRequest request = {900, 650, 32};

    assert(vbe_select_mode(&request, modes, 3) == 0x115);
}

static void test_depth_tiebreaker(void)
{
    const VbeMode modes[] = {
        MODE(800, 600, 16, 0x114),
        MODE(800, 600, 32, 0x115)
    };

    const VbeModeRequest request = {800, 600, 24};

    assert(vbe_select_mode(&request, modes, 2) == 0x115);
}

static void test_unsupported_memory_model(void)
{
    const VbeMode modes[] = {
        {
            .mode_number = 0x115,
            .width = 800,
            .height = 600,
            .bits_per_pixel = 32,
            .memory_model = 3,
            .attributes = VBE_ATTR_LINEAR_FRAMEBUFFER
        }
    };

    const VbeModeRequest request = {800, 600, 32};

    assert(vbe_select_mode(&request, modes, 1) == VBE_MODE_DEFAULT);
}

static void test_missing_linear_framebuffer(void)
{
    const VbeMode modes[] = {
        {
            .mode_number = 0x115,
            .width = 800,
            .height = 600,
            .bits_per_pixel = 32,
            .memory_model = VBE_MEMORY_MODEL_DIRECT_COLOR,
            .attributes = 0
        }
    };

    const VbeModeRequest request = {800, 600, 32};

    assert(vbe_select_mode(&request, modes, 1) == VBE_MODE_DEFAULT);
}

static void test_default_fallback(void)
{
    const VbeModeRequest request = {320, 200, 8};

    assert(vbe_select_mode(&request, NULL, 0) == VBE_MODE_DEFAULT);
}

static void test_monochrome_fallback(void)
{
    const VbeModeRequest request = {640, 480, 1};

    assert(vbe_select_mode(&request, NULL, 0) ==
           VBE_MODE_640_480_MONO);
}

static void test_invalid_request(void)
{
    const VbeModeRequest request = {0, 480, 32};

    assert(vbe_select_mode(&request, NULL, 0) == VBE_MODE_DEFAULT);
    assert(vbe_select_mode(NULL, NULL, 0) == VBE_MODE_DEFAULT);
}

int main(void)
{
    test_exact_match();
    test_resolution_selection();
    test_depth_tiebreaker();
    test_unsupported_memory_model();
    test_missing_linear_framebuffer();
    test_default_fallback();
    test_monochrome_fallback();
    test_invalid_request();

    puts("All VBE mode selector tests passed.");

    return 0;
}
