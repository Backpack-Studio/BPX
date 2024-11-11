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

#include "BPX/algorithm.hpp"

#include <algorithm>
#include <stdexcept>
#include <utility>
#include <cstring>

#define STB_IMAGE_RESIZE_IMPLEMENTATION

#define STBIR_MALLOC(sz,_)         std::malloc(sz)
#define STBIR_FREE(p,_)            std::free(p)

#include <stb_image_resize2.h>

#define STB_IMAGE_WRITE_IMPLEMENTATION

#define STBIW_MALLOC(sz)         std::malloc(sz)
#define STBIW_REALLOC(p,newsz)   std::realloc(p,newsz)
#define STBIW_FREE(p)            std::free(p)

#include <stb_image_write.h>

/* Macros */

#define PF_LINE_TRAVEL(PIXEL_CODE)                                                          \
    if (clip_line(&x1, &y1, &x2, &y2, 0, 0, image.width() - 1, image.height() - 1) == 0) {  \
        return image;                                                                       \
    }                                                                                       \
    int_fast8_t y_longer = 0;                                                               \
    int short_len = y2 - y1;                                                                \
    int long_len = x2 - x1;                                                                 \
    /*                                                                                      \
        Determine if the line is more vertical than horizontal                              \
    */                                                                                      \
    if (abs(short_len) > abs(long_len)) {                                                   \
        std::swap(short_len, long_len);                                                     \
        y_longer = 1;                                                                       \
    }                                                                                       \
    /*                                                                                      \
        Initialize variables for drawing loop                                               \
    */                                                                                      \
    int end = long_len;                                                                     \
    int_fast8_t sign = 1;                                                                   \
    /*                                                                                      \
        Adjust direction increment based on long_len sign                                   \
    */                                                                                      \
    if (long_len < 0) {                                                                     \
        long_len = -long_len;                                                               \
        sign = -1;                                                                          \
    }                                                                                       \
    /*                                                                                      \
        Calculate fixed-point increment for shorter length                                  \
    */                                                                                      \
    int dec = (long_len == 0) ? 0 : (short_len << 16) / long_len;                           \
    /*                                                                                      \
        Travel the line pixel by pixel                                                      \
    */                                                                                      \
    if (y_longer) {                                                                         \
        /*                                                                                  \
            If line is more vertical, iterate over y-axis                                   \
        */                                                                                  \
        for (int i = 0, j = 0; i != end; i += sign, j += dec) {                             \
            int x = x1 + (j >> 16), y = y1 + i;                                             \
            size_t offset = y * image.width() + x;                                          \
            PIXEL_CODE                                                                      \
        }                                                                                   \
    } else {                                                                                \
        /*                                                                                  \
            If line is more horizontal, iterate over x-axis                                 \
        */                                                                                  \
        for (int i = 0, j = 0; i != end; i += sign, j += dec) {                             \
            int x = x1 + i, y = y1 + (j >> 16);                                             \
            size_t offset = y * image.width() + x;                                          \
            PIXEL_CODE                                                                      \
        }                                                                                   \
    }

#define PF_LINE_THICK_TRAVEL(LINE_CODE)                                                     \
    /*                                                                                      \
        Calculate differences in x and y coordinates                                        \
    */                                                                                      \
    int dx = x2 - x1;                                                                       \
    int dy = y2 - y1;                                                                       \
    /*                                                                                      \
        Draw the main line between (x1, y1) and (x2, y2)                                    \
    */                                                                                      \
    LINE_CODE                                                                               \
    /*                                                                                      \
        Determine if the line is more horizontal or vertical                                \
    */                                                                                      \
    if (dx != 0 && abs(dy/dx) < 1) {                                                        \
        int y1_copy = y1;                                                                   \
        int y2_copy = y2;                                                                   \
        /*                                                                                  \
            Line is more horizontal                                                         \
            Calculate half the width of the line                                            \
        */                                                                                  \
        int wy = (thick - 1) * sqrtf((float)(dx*dx + dy*dy)) / (2*abs(dx));                 \
        /*                                                                                  \
            Draw additional lines above and below the main line                             \
        */                                                                                  \
        for (int i = 1; i <= wy; ++i) {                                                     \
            y1 = y1_copy - i;                                                               \
            y2 = y2_copy - i;                                                               \
            {                                                                               \
                LINE_CODE                                                                   \
            }                                                                               \
            y1 = y1_copy + i;                                                               \
            y2 = y2_copy + i;                                                               \
            {                                                                               \
                LINE_CODE                                                                   \
            }                                                                               \
        }                                                                                   \
    } else if (dy != 0) {                                                                   \
        int x1_copy = x1;                                                                   \
        int x2_copy = x2;                                                                   \
        /*                                                                                  \
            Line is more vertical or perfectly horizontal                                   \
            Calculate half the width of the line                                            \
        */                                                                                  \
        int wx = (thick - 1) * sqrtf((float)(dx*dx + dy*dy)) / (2*abs(dy));                 \
        /*                                                                                  \
            Draw additional lines to the left and right of the main line                    \
        */                                                                                  \
        for (int i = 1; i <= wx; ++i) {                                                     \
            x1 = x1_copy - i;                                                               \
            x2 = x2_copy - i;                                                               \
            {                                                                               \
                LINE_CODE                                                                   \
            }                                                                               \
            x1 = x1_copy + i;                                                               \
            x2 = x2_copy + i;                                                               \
            {                                                                               \
                LINE_CODE                                                                   \
            }                                                                               \
        }                                                                                   \
    }

#define PF_CIRCLE_TRAVEL(PIXEL_CODE)                                                        \
    int x = 0;                                                                              \
    int y = radius;                                                                         \
    int d = 3 - 2 * radius;                                                                 \
    while (y >= x) {                                                                        \
        for (int i = cx - x; i <= cx + x; ++i) {                                            \
            if (i >= 0 && i < image.width()) {                                              \
                if (cy + y >= 0 && cy + y < image.height()) {                               \
                    size_t offset = (cy + y) * image.width() + i;                           \
                    PIXEL_CODE                                                              \
                }                                                                           \
                if (cy - y >= 0 && cy - y < image.height()) {                               \
                    size_t offset = (cy - y) * image.width() + i;                           \
                    PIXEL_CODE                                                              \
                }                                                                           \
            }                                                                               \
        }                                                                                   \
        for (int i = cx - y; i <= cx + y; ++i) {                                            \
            if (i >= 0 && i < image.width()) {                                              \
                if (cy + x >= 0 && cy + x < image.height()) {                               \
                    size_t offset = (cy + x) * image.width() + i;                           \
                    PIXEL_CODE                                                              \
                }                                                                           \
                if (cy - x >= 0 && cy - x < image.height()) {                               \
                    size_t offset = (cy - x) * image.width() + i;                           \
                    PIXEL_CODE                                                              \
                }                                                                           \
            }                                                                               \
        }                                                                                   \
        ++x;                                                                                \
        if (d > 0) {                                                                        \
            --y;                                                                            \
            d = d + 4 * (x - y) + 10;                                                       \
        } else {                                                                            \
            d = d + 4 * x + 6;                                                              \
        }                                                                                   \
    }

#define PF_CIRCLE_TRAVEL_EX(PC_A, PC_B, PC_C, PC_D)                                         \
    int x = 0;                                                                              \
    int y = radius;                                                                         \
    int d = 3 - 2 * radius;                                                                 \
    while (y >= x) {                                                                        \
        for (int i = cx - x; i <= cx + x; ++i) {                                            \
            if (i >= 0 && i < image.width()) {                                              \
                if (cy + y >= 0 && cy + y < image.height()) {                               \
                    size_t offset = (cy + y) * image.width() + i;                           \
                    PC_A                                                                    \
                }                                                                           \
                if (cy - y >= 0 && cy - y < image.height()) {                               \
                    size_t offset = (cy - y) * image.width() + i;                           \
                    PC_B                                                                    \
                }                                                                           \
            }                                                                               \
        }                                                                                   \
        for (int i = cx - y; i <= cx + y; ++i) {                                            \
            if (i >= 0 && i < image.width()) {                                              \
                if (cy + x >= 0 && cy + x < image.height()) {                               \
                    size_t offset = (cy + x) * image.width() + i;                           \
                    PC_C                                                                    \
                }                                                                           \
                if (cy - x >= 0 && cy - x < image.height()) {                               \
                    size_t offset = (cy - x) * image.width() + i;                           \
                    PC_D                                                                    \
                }                                                                           \
            }                                                                               \
        }                                                                                   \
        ++x;                                                                                \
        if (d > 0) {                                                                        \
            --y;                                                                            \
            d = d + 4 * (x - y) + 10;                                                       \
        } else {                                                                            \
            d = d + 4 * x + 6;                                                              \
        }                                                                                   \
    }

#define PF_CIRCLE_LINE_TRAVEL(PIXEL_CODE)                                                   \
    int x = 0;                                                                              \
    int y = radius;                                                                         \
    int d = 3 - 2 * radius;                                                                 \
    while (y >= x) {                                                                        \
        int px1 = cx + x, px2 = cx - x;                                                     \
        int py1 = cy + y, py2 = cy - y;                                                     \
        int px3 = cx + y, px4 = cx - y;                                                     \
        int py3 = cy + x, py4 = cy - x;                                                     \
        if (px1 >= 0 && px1 < image.width()) {                                              \
            if (py1 >= 0 && py1 < image.height()) {                                         \
                size_t offset = py1 * image.width() + px1;                                  \
                PIXEL_CODE                                                                  \
            }                                                                               \
            if (py2 >= 0 && py2 < image.height()) {                                         \
                size_t offset = py2 * image.width() + px1;                                  \
                PIXEL_CODE                                                                  \
            }                                                                               \
        }                                                                                   \
        if (px2 >= 0 && px2 < image.width()) {                                              \
            if (py1 >= 0 && py1 < image.height()) {                                         \
                size_t offset = py1 * image.width() + px2;                                  \
                PIXEL_CODE                                                                  \
            }                                                                               \
            if (py2 >= 0 && py2 < image.height()) {                                         \
                size_t offset = py2 * image.width() + px2;                                  \
                PIXEL_CODE                                                                  \
            }                                                                               \
        }                                                                                   \
        if (px3 >= 0 && px3 < image.width()) {                                              \
            if (py3 >= 0 && py3 < image.height()) {                                         \
                size_t offset = py3 * image.width() + px3;                                  \
                PIXEL_CODE                                                                  \
            }                                                                               \
            if (py4 >= 0 && py4 < image.height()) {                                         \
                size_t offset = py4 * image.width() + px3;                                  \
                PIXEL_CODE                                                                  \
            }                                                                               \
        }                                                                                   \
        if (px4 >= 0 && px4 < image.width()) {                                              \
            if (py3 >= 0 && py3 < image.height()) {                                         \
                size_t offset = py3 * image.width() + px4;                                  \
                PIXEL_CODE                                                                  \
            }                                                                               \
            if (py4 >= 0 && py4 < image.height()) {                                         \
                size_t offset = py4 * image.width() + px4;                                  \
                PIXEL_CODE                                                                  \
            }                                                                               \
        }                                                                                   \
        ++x;                                                                                \
        if (d > 0) {                                                                        \
            --y;                                                                            \
            d = d + 4 * (x - y) + 10;                                                       \
        } else {                                                                            \
            d = d + 4 * x + 6;                                                              \
        }                                                                                   \
    }

#define PF_RECT_TRAVEL(PIXEL_CODE)                                                          \
    for (int y = ymin; y <= ymax; ++y) {                                                    \
        size_t y_offset = y * image.width();                                                \
        for (int x = xmin; x <= xmax; ++x) {                                                \
            size_t offset = y_offset + x;                                                   \
            PIXEL_CODE                                                                      \
        }                                                                                   \
    }


/* Internal */

namespace {

enum ClipCode : uint_fast8_t
{
    CLIP_INSIDE = 0x00, // 0000
    CLIP_LEFT   = 0x01, // 0001
    CLIP_RIGHT  = 0x02, // 0010
    CLIP_BOTTOM = 0x04, // 0100
    CLIP_TOP    = 0x08, // 1000
};

uint_fast8_t encode_point(int x, int y, int xmin, int ymin, int xmax, int ymax)
{
    uint_fast8_t code = CLIP_INSIDE;
    if (x < xmin) code |= CLIP_LEFT;
    if (x > xmax) code |= CLIP_RIGHT;
    if (y < ymin) code |= CLIP_TOP;
    if (y > ymax) code |= CLIP_BOTTOM;
    return code;
}

int_fast8_t clip_line(int* x1, int* y1, int* x2, int* y2, int xmin, int ymin, int xmax, int ymax)
{
    int_fast8_t accept = 0;  // Initialize accept flag to false

    uint8_t code1 = encode_point(*x1, *y1, xmin, ymin, xmax, ymax);
    uint8_t code2 = encode_point(*x2, *y2, xmin, ymin, xmax, ymax);

    int dx = *x2 - *x1;
    int dy = *y2 - *y1;

    // Loop until the line is accepted or rejected
    for (;;) {

        // Accepted if both endpoints lie completely within the clipping rectangle
        if ((code1 | code2) == 0) {
            accept = 1;
            break;
        }

        // Rejected if both endpoints lie outside the same clipping region
        if (code1 & code2) break;

        // Determine which point to clip
        uint8_t code_out = code1 ? code1 : code2;
        int x = *x1, y = *y1;

        if (code_out & CLIP_LEFT) {
            if (dx) y += dy * (xmin - x) / dx;
            x = xmin;
        } else if (code_out & CLIP_RIGHT) {
            if (dx) y += dy * (xmax - x) / dx;
            x = xmax;
        } else if (code_out & CLIP_BOTTOM) {
            if (dy) x += dx * (ymax - y) / dy;
            y = ymax;
        } else /* code_out & CLIP_TOP */ {
            if (dy) x += dx * (ymin - y) / dy;
            y = ymin;
        }

        if (code_out == code1) {
            code1 = encode_point(x, y, xmin, ymin, xmax, ymax);
            *x1 = x, *y1 = y;
        } else {
            code2 = encode_point(x, y, xmin, ymin, xmax, ymax);
            *x2 = x, *y2 = y;
        }
    }

    return accept;
}

} // namespace anonymous


/* Implementation */

namespace bpx {

Image& map(Image& image, const Image::Mapper& mapper)
{
    for (int y = 0; y < image.height(); y++) {
        for (int x = 0; x < image.width(); x++) {
            size_t offset = y * image.width() + x;
            Color color = mapper(x, y, image.get_unsafe(offset));
            image.set_unsafe(offset, color);
        }
    }
    return image;
}

Image& map(Image& image, int x_start, int y_start, int width, int height, const Image::Mapper& mapper)
{
    x_start = std::clamp(x_start, 0, image.width());
    y_start = std::clamp(y_start, 0, image.height());

    int x_end = std::min(x_start + width, image.width());
    int y_end = std::min(y_start + height, image.height());

    for (int y = y_start; y < y_end; y++) {
        for (int x = x_start; x < x_end; x++) {
            size_t offset = y * image.width() + x;
            Color color = mapper(x, y, image.get_unsafe(offset));
            image.set_unsafe(offset, color);
        }
    }

    return image;
}

Image& fill(Image& image, const Color& color)
{
    const size_t size = image.size();
    for (size_t i = 0; i < size; i++) {
        image.set_unsafe(i, color);
    }
    return image;
}

Image& point(Image& image, int x, int y, Color color, BlendMode mode)
{
    return image.set(x, y, blend(image.get(x, y), color, mode));
}

Image& line(Image& image, int x1, int y1, int x2, int y2, Color color, BlendMode mode)
{
    PF_LINE_TRAVEL({
        image.set_unsafe(offset, blend(image.get_unsafe(offset), color, mode));
    })

    return image;
}

Image& line(Image& image, int x1, int y1, int x2, int y2, const Image::Mapper& mapper)
{
    PF_LINE_TRAVEL({
        image.set_unsafe(offset, mapper(x, y, image.get_unsafe(offset)));
    });
    return image;
}

Image& line(Image& image, int x1, int y1, int x2, int y2, int thick, Color color, BlendMode mode)
{
    PF_LINE_THICK_TRAVEL({
        line(image, x1, y1, x2, y2, color, mode);
    });
    return image;
}

Image& line(Image& image, int x1, int y1, int x2, int y2, int thick, const Image::Mapper& mapper)
{
    PF_LINE_THICK_TRAVEL({
        line(image, x1, y1, x2, y2, mapper);
    });
    return image;
}

Image& line_gradient(Image& image, int x1, int y1, int x2, int y2, Color c1, Color c2, BlendMode mode)
{
    PF_LINE_TRAVEL({
        image.set_unsafe(offset, blend(image.get_unsafe(offset), lerp(c1, c2, static_cast<float>(i) / end), mode));
    });
    return image;
}

Image& line_gradient(Image& image, int x1, int y1, int x2, int y2, int thick, Color c1, Color c2, BlendMode mode)
{
    PF_LINE_THICK_TRAVEL({
        line_gradient(image, x1, y1, x2, y2, c1, c2, mode);
    });
    return image;
}

Image& rectangle(Image& image, int x1, int y1, int x2, int y2, Color color, BlendMode mode)
{
    if (x1 > x2) std::swap(x1, x2);
    if (y1 > y2) std::swap(y1, y2);

    int xmin = std::clamp(x1, 0, image.width() - 1);
    int ymin = std::clamp(y1, 0, image.height() - 1);
    int xmax = std::clamp(x2, 0, image.width() - 1);
    int ymax = std::clamp(y2, 0, image.height() - 1);

    PF_RECT_TRAVEL({
        image.set_unsafe(offset, blend(image.get_unsafe(offset), color, mode));
    });

    return image;
}

Image& rectangle(Image& image, int x1, int y1, int x2, int y2, const Image::Mapper& mapper)
{
    if (x1 > x2) std::swap(x1, x2);
    if (y1 > y2) std::swap(y1, y2);

    int xmin = std::clamp(x1, 0, image.width() - 1);
    int ymin = std::clamp(y1, 0, image.height() - 1);
    int xmax = std::clamp(x2, 0, image.width() - 1);
    int ymax = std::clamp(y2, 0, image.height() - 1);

    PF_RECT_TRAVEL({
        image.set_unsafe(offset, mapper(x, y, image.get_unsafe(offset)));
    });

    return image;
}

Image& rectangle_gradient(Image& image, int x1, int y1, int x2, int y2,
                          Color col_tl, Color col_tr,
                          Color col_br, Color col_bl,
                          BlendMode mode)
{
    if (x1 > x2) std::swap(x1, x2);
    if (y1 > y2) std::swap(y1, y2);

    int w = abs(x2 - x1);
    int h = abs(y2 - y1);

    int xmin = std::clamp(x1, 0, image.width() - 1);
    int ymin = std::clamp(y1, 0, image.height() - 1);
    int xmax = std::clamp(x2, 0, image.width() - 1);
    int ymax = std::clamp(y2, 0, image.height() - 1);

    PF_RECT_TRAVEL({
        int ix = x - x1;
        int iy = y - y1;
        Color col_top = lerp(col_tl, col_tr, static_cast<float>(ix) / w);
        Color col_bottom = lerp(col_bl, col_br, static_cast<float>(ix) / w);
        Color col_final = lerp(col_top, col_bottom, static_cast<float>(iy) / h);
        image.set_unsafe(offset, blend(image.get_unsafe(offset), col_final, mode));
    });

    return image;
}

Image& rectangle_lines(Image& image, int x1, int y1, int x2, int y2, Color color, BlendMode mode)
{
    line(image, x1, y1, x2, y1, color, mode);
    line(image, x2, y1, x2, y2, color, mode);
    line(image, x2, y2, x1, y2, color, mode);
    line(image, x1, y2, x1, y1, color, mode);

    return image;
}

Image& rectangle_lines(Image& image, int x1, int y1, int x2, int y2, const Image::Mapper& mapper)
{
    line(image, x1, y1, x2, y1, mapper);
    line(image, x2, y1, x2, y2, mapper);
    line(image, x2, y2, x1, y2, mapper);
    line(image, x1, y2, x1, y1, mapper);

    return image;
}

Image& rectangle_lines(Image& image, int x1, int y1, int x2, int y2, int thick, Color color, BlendMode mode)
{
    line(image, x1, y1, x2, y1, thick, color, mode);
    line(image, x2, y1, x2, y2, thick, color, mode);
    line(image, x2, y2, x1, y2, thick, color, mode);
    line(image, x1, y2, x1, y1, thick, color, mode);

    return image;
}

Image& rectangle_lines(Image& image, int x1, int y1, int x2, int y2, int thick, const Image::Mapper& mapper)
{
    line(image, x1, y1, x2, y1, thick, mapper);
    line(image, x2, y1, x2, y2, thick, mapper);
    line(image, x2, y2, x1, y2, thick, mapper);
    line(image, x1, y2, x1, y1, thick, mapper);

    return image;
}

Image& rectangle_lines_gradient(Image& image, int x1, int y1, int x2, int y2,
                                Color col_tl, Color col_tr,
                                Color col_br, Color col_bl,
                                BlendMode mode)
{
    line_gradient(image, x1, y1, x2, y1, col_tl, col_tr, mode);
    line_gradient(image, x2, y1, x2, y2, col_tr, col_br, mode);
    line_gradient(image, x2, y2, x1, y2, col_br, col_bl, mode);
    line_gradient(image, x1, y2, x1, y1, col_bl, col_tl, mode);

    return image;
}

Image& rectangle_lines_gradient(Image& image, int x1, int y1, int x2, int y2, int thick,
                         Color col_tl, Color col_tr, Color col_br, Color col_bl,
                         BlendMode mode)
{
    line_gradient(image, x1, y1, x2, y1, thick, col_tl, col_tr, mode);
    line_gradient(image, x2, y1, x2, y2, thick, col_tr, col_br, mode);
    line_gradient(image, x2, y2, x1, y2, thick, col_br, col_bl, mode);
    line_gradient(image, x1, y2, x1, y1, thick, col_bl, col_tl, mode);

    return image;
}

Image& circle(Image& image, int cx, int cy, int radius, Color color, BlendMode mode)
{
    PF_CIRCLE_TRAVEL({
        image.set_unsafe(offset, blend(image.get_unsafe(offset), color, mode));
    })
    return image;
}

Image& circle(Image& image, int cx, int cy, int radius, const Image::Mapper& mapper)
{
    PF_CIRCLE_TRAVEL({
        image.set_unsafe(offset, mapper(x, y, image.get_unsafe(offset)));
    })
    return image;
}

Image& circle_gradient(Image& image, int cx, int cy, int radius, Color c1, Color c2, BlendMode mode)
{
    PF_CIRCLE_TRAVEL_EX(
        { image.set_unsafe(offset, blend(image.get_unsafe(offset), lerp(c1, c2, sqrtf((i - cx) * (i - cx) + (cy + y - cy) * (cy + y - cy)) / radius), mode)); },
        { image.set_unsafe(offset, blend(image.get_unsafe(offset), lerp(c1, c2, sqrtf((i - cx) * (i - cx) + (cy - y - cy) * (cy - y - cy)) / radius), mode)); },
        { image.set_unsafe(offset, blend(image.get_unsafe(offset), lerp(c1, c2, sqrtf((i - cx) * (i - cx) + (cy + x - cy) * (cy + x - cy)) / radius), mode)); },
        { image.set_unsafe(offset, blend(image.get_unsafe(offset), lerp(c1, c2, sqrtf((i - cx) * (i - cx) + (cy - x - cy) * (cy - x - cy)) / radius), mode)); }
    );
    return image;
}

Image& circle_lines(Image& image, int cx, int cy, int radius, Color color, BlendMode mode)
{
    PF_CIRCLE_LINE_TRAVEL({
        image.set_unsafe(offset, blend(image.get_unsafe(offset), color, mode));
    });
    return image;
}

Image& circle_lines(Image& image, int cx, int cy, int radius, const Image::Mapper& mapper)
{
    PF_CIRCLE_LINE_TRAVEL({
        image.set_unsafe(offset, mapper(x, y, image.get_unsafe(offset)));
    });
    return image;
}

Image& circle_lines(Image& image, int cx, int cy, int radius, int thick, Color color, BlendMode mode)
{
    int ht = thick/2;
    for (int i = -ht; i <= ht; ++i) {
        circle_lines(image, cx, cy, radius + i, color, mode);
    }
    return image;
}

Image& circle_lines(Image& image, int cx, int cy, int radius, int thick, const Image::Mapper& mapper)
{
    int ht = thick/2;
    for (int i = -ht; i <= ht; ++i) {
        circle_lines(image, cx, cy, radius + i, mapper);
    }
    return image;
}

Image& draw(Image& dst, int x_dst, int y_dst, int w_dst, int h_dst,
            Image& src, int x_src, int y_src, int w_src, int h_src,
            BlendMode mode)
{
    x_dst = std::clamp(x_dst, 0, dst.width());
    y_dst = std::clamp(y_dst, 0, dst.height());
    w_dst = std::clamp(w_dst, 0, dst.width());
    h_dst = std::clamp(h_dst, 0, dst.height());

    x_src = std::clamp(x_src, 0, dst.width());
    y_src = std::clamp(y_src, 0, dst.height());
    w_src = std::clamp(w_src, 0, dst.width());
    h_src = std::clamp(h_src, 0, dst.height());

    const float scale_x = static_cast<float>(w_src) / w_dst;
    const float scale_y = static_cast<float>(h_src) / h_dst;

    for (int y = 0; y < h_dst; y++) {
        const int src_y = y_src + static_cast<int>(y * scale_y);
        const int dst_y = y_dst + y;

        for (int x = 0; x < w_dst; x++) {
            const int src_x = x_src + static_cast<int>(x * scale_x);
            const int dst_x = x_dst + x;

            Color col_src = src.get_unsafe(src_x, src_y);
            Color col_dst = dst.get_unsafe(dst_x, dst_y);
            dst.set_unsafe(dst_x, dst_y, blend(col_dst, col_src, mode));
        }
    }
    
    return dst;
}

Image& saturation(Image& image, float factor)
{
    const size_t size = image.width() * image.height();
    for (size_t i = 0; i < size; i++) {
        Color color = image.get_unsafe(i);
        image.set_unsafe(i, bpx::saturation(color, factor));
    }
    return image;
}

Image& brightness(Image& image, float factor)
{
    const size_t size = image.size();
    for (size_t i = 0; i < size; i++) {
        Color color = image.get_unsafe(i);
        image.set_unsafe(i, bpx::brightness(color, factor));
    }
    return image;
}

Image& contrast(Image& image, float factor)
{
    const size_t size = image.size();
    for (size_t i = 0; i < size; i++) {
        Color color = image.get_unsafe(i);
        image.set_unsafe(i, bpx::contrast(color, factor));
    }
    return image;
}

Image& opacity(Image& image, float alpha)
{
    const size_t size = image.size();
    for (size_t i = 0; i < size; i++) {
        Color color = image.get_unsafe(i);
        image.set_unsafe(i, bpx::alpha(color, alpha));
    }
    return image;
}

Image& invert(Image& image)
{
    const size_t size = image.size();
    for (size_t i = 0; i < size; i++) {
        Color color = image.get_unsafe(i);
        image.set_unsafe(i, bpx::invert(color));
    }
    return image;
}

Image& flip_horizontal(Image& image)
{
    size_t pitch = image.pitch();
    std::vector<uint8_t> row_buffer(pitch);
    for (int y = 0; y < image.height(); y++) {
        std::memcpy(row_buffer.data(), (uint8_t*)image.data() + y * pitch, pitch);
        for (int x = 0; x < image.width(); x++) {
            size_t src_offset = y * image.width() + x;
            size_t dst_offset = y * image.width() + (image.width() - x - 1);
            if (src_offset != dst_offset) {
                Color src_color = image.get_unsafe(src_offset);
                image.set_unsafe(dst_offset, src_color);
            }
        }
    }
    return image;
}

Image& flip_vertical(Image& image)
{
    size_t pitch = image.pitch();
    for (int y = 0; y < image.height() / 2; y++) {
        for (int x = 0; x < image.width(); x++) {
            size_t top_offset = y * image.width() + x;
            size_t bottom_offset = (image.height() - y - 1) * image.width() + x;
            Color top_color = image.get_unsafe(top_offset);
            Color bottom_color = image.get_unsafe(bottom_offset);
            image.set_unsafe(top_offset, bottom_color);
            image.set_unsafe(bottom_offset, top_color);
        }
    }
    return image;
}

Image& rotate_90(Image& image)
{
    // Check if the image is square
    if (image.width() == image.height()) {
        int n = image.width();
        
        // Rotate in place in a quarter turn (90 degrees) for a square image
        for (int layer = 0; layer < n / 2; layer++) {
            int first = layer;
            int last = n - 1 - layer;

            for (int i = first; i < last; i++) {
                int offset = i - first;

                // Save top left corner pixel
                Color top = image.get_unsafe(first, i);

                // Move pixel from bottom left to top left
                image.set_unsafe(first, i, image.get_unsafe(last - offset, first));

                // Move pixel from bottom right corner to bottom left corner
                image.set_unsafe(last - offset, first, image.get_unsafe(last, last - offset));

                // Move pixel from top right corner to bottom right corner
                image.set_unsafe(last, last - offset, image.get_unsafe(i, last));

                // Put the saved pixel (top) at the upper right corner
                image.set_unsafe(i, last, top);
            }
        }
    }
    else {
        uint8_t* new_data = (uint8_t*)std::malloc(image.data_size());
        if (new_data == nullptr) {
            throw std::bad_alloc();
        }
        for (int y = 0; y < image.height(); y++) {
            for (int x = 0; x < image.width(); x++) {
                size_t src_offset = y * image.width() + x;
                size_t dst_offset = (image.width() - 1 - x) * image.height() + y;
                Color color = image.get_unsafe(src_offset);
                new_data[dst_offset] = color;
            }
        }
        image = Image(
            new_data, image.height(), image.width(),
            image.format(), false, true
        );
    }

    return image;
}

Image& rotate_180(Image& image)
{
    for (int y = 0; y < image.height(); y++) {
        for (int x = 0; x < image.width(); x++) {
            size_t src_offset = y * image.width() + x;
            size_t dst_offset = (image.height() - 1 - y) * image.width() + (image.width() - 1 - x);
            if (src_offset != dst_offset) {
                Color color = image.get_unsafe(src_offset);
                image.set_unsafe(dst_offset, color);
            }
        }
    }
    return image;
}

Image copy(const Image& image)
{
    return Image(image.data(), image.width(), image.height(), image.format());
}

Image convert(const Image& image, PixelFormat new_format)
{
    Image new_image(Color::BLANK, image.width(), image.height(), new_format);

    const size_t size = image.size();
    for (size_t i = 0; i < size; i++) {
        Color src_color = image.get_unsafe(i);
        new_image.set_unsafe(i, src_color);
    }

    return new_image;
}

Image resize_canvas(const Image& image, int new_w, int new_h, bool centered)
{
    if (new_w <= 0 || new_h <= 0) {
        throw std::invalid_argument("The new dimensions must be positive");
    }

    Image new_image(Color::BLANK, new_w, new_h, image.format());

    int offset_x = 0;
    int offset_y = 0;

    if (centered) {
        offset_x = (new_w - image.width()) / 2;
        offset_y = (new_h - image.height()) / 2;
    }

    for (int y = 0; y < image.height(); y++) {
        for (int x = 0; x < image.width(); x++) {
            int new_x = x + offset_x;
            int new_y = y + offset_y;
            if (new_x >= 0 && new_x < new_w && new_y >= 0 && new_y < new_h) {
                new_image.set_unsafe(new_x, new_y, image.get_unsafe(x, y));
            }
        }
    }

    return new_image;
}

Image resize(const Image& image, int new_w, int new_h)
{
    int comp = -1;
    bool is_float = false;
    switch (image.format()) {

        case PixelFormat::L_U8:
            comp = STBIR_1CHANNEL;
            break;
        case PixelFormat::LA_U8:
            comp = STBIR_2CHANNEL;
            break;
        case PixelFormat::RGB_U8:
            comp = STBIR_RGB;
            break;
        case PixelFormat::BGR_U8:
            comp = STBIR_BGR;
            break;
        case PixelFormat::RGBA_U8:
            comp = STBIR_RGBA;
            break;
        case PixelFormat::BGRA_U8:
            comp = STBIR_BGRA;
            break;

        case PixelFormat::L_F32:
            comp = STBIR_1CHANNEL;
            is_float = true;
            break;
        case PixelFormat::LA_F32:
            comp = STBIR_2CHANNEL;
            is_float = true;
            break;
        case PixelFormat::RGB_F32:
            comp = STBIR_RGB;
            is_float = true;
            break;
        case PixelFormat::BGR_F32:
            comp = STBIR_BGR;
            is_float = true;
            break;
        case PixelFormat::RGBA_F32:
            comp = STBIR_RGBA;
            is_float = true;
            break;
        case PixelFormat::BGRA_F32:
            comp = STBIR_BGRA;
            is_float = true;
            break;

        default:
            break;
    }

    if (comp == -1) {
        throw std::runtime_error("Unsupported data type for resizing");
    }

    void *new_data = nullptr;

    if (is_float) {
        new_data = stbir_resize_float_linear(
            static_cast<const float*>(image.data()),
            image.width(), image.height(), 0,
            static_cast<float*>(new_data),
            new_w, new_h, 0,
            STBIR_1CHANNEL
        );
    } else {
        new_data = stbir_resize_uint8_linear(
            static_cast<const uint8_t*>(image.data()),
            image.width(), image.height(), 0,
            static_cast<uint8_t*>(new_data),
            new_w, new_h, 0,
            STBIR_1CHANNEL
        );
    }

    return Image(new_data, new_w, new_h,
                 image.format(), false, true);
}

bool write_png(const Image& image, const std::string& path)
{
    int result = stbi_write_png(path.c_str(), image.width(), image.height(),
                                pixel_comp(image.format()), image.data(),
                                image.pitch());
    return result != 0;
}

bool write_bmp(const Image& image, const std::string& path)
{
    int result = stbi_write_bmp(path.c_str(), image.width(), image.height(),
                                pixel_comp(image.format()), image.data());
    return result != 0;
}

bool write_tga(const Image& image, const std::string& path)
{
    int result = stbi_write_tga(path.c_str(), image.width(), image.height(),
                                pixel_comp(image.format()), image.data());
    return result != 0;
}

bool write_jpg(const Image& image, const std::string& path, int quality)
{
    int result = stbi_write_jpg(path.c_str(), image.width(), image.height(),
                                pixel_comp(image.format()), image.data(),
                                quality);
    return result != 0;
}

} // namespace bpx
