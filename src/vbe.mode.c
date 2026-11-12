#include "vbe_mode.h"

#include <stdint.h>

static uint64_t absolute_difference(uint64_t a, uint64_t b)
{
    return (a > b) ? (a - b) : (b - a);
}

static bool is_supported_mode(const VbeMode *mode)
{
    if (mode == NULL)
        return false;

    if (!(mode->attributes & VBE_ATTR_LINEAR_FRAMEBUFFER))
        return false;

    return mode->memory_model == VBE_MEMORY_MODEL_PACKED_PIXEL ||
           mode->memory_model == VBE_MEMORY_MODEL_DIRECT_COLOR;
}

static uint64_t resolution_difference(
    const VbeMode *mode,
    const VbeModeRequest *request
)
{
    const uint64_t mode_pixels =
        (uint64_t)mode->width * mode->height;

    const uint64_t requested_pixels =
        (uint64_t)request->width * request->height;

    return absolute_difference(mode_pixels, requested_pixels);
}

static uint32_t depth_difference(
    uint8_t candidate_depth,
    uint8_t requested_depth
)
{
    if (candidate_depth >= requested_depth)
        return candidate_depth - requested_depth;

    return ((uint32_t)requested_depth - candidate_depth) * 2;
}

uint16_t vbe_select_mode(
    const VbeModeRequest *request,
    const VbeMode *modes,
    size_t mode_count
)
{
    if (request == NULL)
        return VBE_MODE_DEFAULT;

    if (request->width == 0 ||
        request->height == 0 ||
        request->bits_per_pixel == 0)
        return VBE_MODE_DEFAULT;

    if (modes == NULL || mode_count == 0) {
        if (request->width == 640 &&
            request->height == 480 &&
            request->bits_per_pixel == 1)
            return VBE_MODE_640_480_MONO;

        return VBE_MODE_DEFAULT;
    }

    uint16_t best_mode = VBE_MODE_DEFAULT;

    /*
     * Preserve the original fallback baseline:
     * 320x200, 8 bits per pixel.
     */
    uint64_t best_pixel_difference = absolute_difference(
        320ULL * 200ULL,
        (uint64_t)request->width * request->height
    );

    uint32_t best_depth_difference =
        request->bits_per_pixel <= 8
            ? 8 - request->bits_per_pixel
            : ((uint32_t)request->bits_per_pixel - 8) * 2;

    for (size_t i = 0; i < mode_count; ++i) {
        const VbeMode *candidate = &modes[i];

        if (!is_supported_mode(candidate))
            continue;

        if (candidate->width == request->width &&
            candidate->height == request->height &&
            candidate->bits_per_pixel == request->bits_per_pixel)
            return candidate->mode_number;

        const uint64_t pixel_difference =
            resolution_difference(candidate, request);

        const uint32_t candidate_depth_difference =
            depth_difference(
                candidate->bits_per_pixel,
                request->bits_per_pixel
            );

        if (pixel_difference < best_pixel_difference ||
            (pixel_difference == best_pixel_difference &&
             candidate_depth_difference < best_depth_difference)) {

            best_mode = candidate->mode_number;
            best_pixel_difference = pixel_difference;
            best_depth_difference = candidate_depth_difference;
        }
    }

    if (best_mode == VBE_MODE_DEFAULT &&
        request->width == 640 &&
        request->height == 480 &&
        request->bits_per_pixel == 1)
        return VBE_MODE_640_480_MONO;

    return best_mode;
}
