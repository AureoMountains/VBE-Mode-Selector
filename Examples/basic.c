#include "vbe_mode.h"

#include <stdio.h>

int main(void)
{
    const VbeMode available_modes[] = {
        {0x115, 800, 600, 32, 6, 0x80},
        {0x118, 1024, 768, 32, 6, 0x80},
        {0x114, 800, 600, 16, 6, 0x80},
        {0x112, 640, 480, 32, 6, 0x80}
    };

    const VbeModeRequest request = {
        .width = 900,
        .height = 650,
        .bits_per_pixel = 32
    };

    const uint16_t selected_mode = vbe_select_mode(
        &request,
        available_modes,
        sizeof(available_modes) / sizeof(available_modes[0])
    );

    printf("Selected VBE mode: 0x%X\n", selected_mode);

    return 0;
}
