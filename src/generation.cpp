/**
 * Copyright (c) 2024 Le Juez Victor
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

Image generate_gradient_linear(int width, int height, const ColorRamp& ramp,
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

Image generate_gradient_radial(int width, int height, const ColorRamp& ramp,
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

Image generate_checkerboard(int width, int height, int square_size,
                            const Color& color1, const Color& color2,
                            PixelFormat format)
{
    Image image(width, height, color1);

    for (int y = 0; y < height; y += square_size) {
        for (int x = 0; x < width; x += square_size) {
            if (((x / square_size) + (y / square_size)) % 2 == 0) {
                rectangle(image, x, y, x + square_size, y + square_size, color2);
            }
        }
    }

    return image;
}

Image generate_stripes(int width, int height, int stripe_width,
                       const Color& color1, const Color& color2,
                       bool vertical, PixelFormat format)
{
    Image image(width, height, color1);

    if (vertical) {
        for (int y = 0; y < height; y += stripe_width) {
            for (int x = 0; x < width; x += stripe_width) {
                if ((x / stripe_width + y / stripe_width) % 2 == 0) {
                    line(image, x, 0, x, height, stripe_width, color2);
                }
            }
        }
    } else {
        for (int y = 0; y < height; y += stripe_width) {
            for (int x = 0; x < width; x += stripe_width) {
                if ((x / stripe_width + y / stripe_width) % 2 == 0) {
                    line(image, 0, y, width, y, stripe_width, color2);
                }
            }
        }
    }

    return image;
}

Image generate_grid(int width, int height, int cell_size,
                    const Color& line_color, const Color& fill_color,
                    PixelFormat format)
{
    Image image(width, height, fill_color);

    for (int y = 0; y <= height; y += cell_size) {
        line(image, 0, y, width, y, line_color);
    }

    for (int x = 0; x <= width; x += cell_size) {
        line(image, x, 0, x, height, line_color);
    }

    return image;
}

Image generate_polka_dots(int width, int height, int dot_radius, int spacing,
                          const Color& dot_color, const Color& background_color,
                          PixelFormat format)
{
    Image image(width, height);

    for (int y = 0; y < height; ++y) {
        for (int x = 0; x < width; ++x) {
            int cx = (x / spacing) * spacing + spacing / 2;
            int cy = (y / spacing) * spacing + spacing / 2;
            int dx = x - cx;
            int dy = y - cy;
            bool is_dot = (dx * dx + dy * dy) <= (dot_radius * dot_radius);
            image.set_unsafe(x, y, is_dot ? dot_color : background_color);
        }
    }

    return image;
}

} // namespace bpx
