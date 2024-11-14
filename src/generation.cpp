/**
 * Copyright (c) 2023-2024 Le Juez Victor
 *
 * This software is provided "as-is", without any express or implied warranty. In no event 
 * will the authors be held liable for any damages arising from the use of this software.
 *
 * Permission is granted to anyone to use this software for any purpose, including commercial 
 * applications, and to alter it and redistribute it freely, subject to the following restrictions:
 *
 *   1. The origin of this software must not be misrepresented; you must not claim that you 
 *   wrote the original software. If you use this software in a product, an acknowledgment 
 *   in the product documentation would be appreciated but is not required.
 *
 *   2. Altered source versions must be plainly marked as such, and must not be misrepresented
 *   as being the original software.
 *
 *   3. This notice may not be removed or altered from any source distribution.
 */

#include "BPX/generation.hpp"

namespace bpx {

Image generate_gradient_linear_1d(int width, const ColorRamp& ramp, PixelFormat format)
{
    Image image(width, 1, BLANK, format);

    for (int x = 0; x < width; x++) {
        image.set_unsafe(x, ramp.get(static_cast<float>(x) / width));
    }

    return image;
}

Image generate_gradient_linear_2d(int width, int height, const ColorRamp& ramp,
                                  int x_start, int y_start, int x_end, int y_end,
                                  PixelFormat format) 
{
    Image image(width, height, BLANK, format);

    float dx = x_end - x_start;
    float dy = y_end - y_start;
    float max_distance = std::sqrt(dx * dx + dy * dy);

    for (int y = 0; y < height; y++) {
        for (int x = 0; x < width; x++) {
            float current_dx = x - x_start;
            float current_dy = y - y_start;
            float distance = (current_dx * dx + current_dy * dy) / max_distance;
            float t = std::clamp(distance / max_distance, 0.0f, 1.0f);
            image.set_unsafe(x, y, ramp.get(t));
        }
    }

    return image;
}

Image generate_gradient_radial_2d(int width, int height, const ColorRamp& ramp,
                                  int x_start, int y_start, int x_end, int y_end,
                                  PixelFormat format) 
{
    Image image(width, height, BLANK, format);

    float max_distance = std::sqrt(
        (x_end - x_start) * (x_end - x_start) + 
        (y_end - y_start) * (y_end - y_start)
    );

    for (int y = 0; y < height; y++) {
        for (int x = 0; x < width; x++) {
            float dx = x - x_start;
            float dy = y - y_start;
            float distance = std::sqrt(dx * dx + dy * dy);
            float t = std::clamp(distance / max_distance, 0.0f, 1.0f);
            image.set_unsafe(x, y, ramp.get(t));
        }
    }

    return image;
}

} // namespace bpx
