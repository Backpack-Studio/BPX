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

#include "BPX/image.hpp"
#include "BPX/algorithm.hpp"

#include <stdexcept>
#include <cstdint>
#include <cstring>
#include <cstddef>
#include <string>

#define STB_IMAGE_IMPLEMENTATION

#define STBI_MALLOC(sz)         std::malloc(sz)
#define STBI_REALLOC(p,newsz)   std::realloc(p,newsz)
#define STBI_FREE(p)            std::free(p)

#include <stb_image.h>

namespace {

static inline uint16_t
float_to_half_i(uint32_t ui)
{
    int s = (ui >> 16) & 0x8000;
    int em = ui & 0x7fffffff;

    // bias exponent and round to nearest; 112 is relative exponent bias (127-15)
    int h = (em - (112 << 23) + (1 << 12)) >> 13;

    // underflow: flush to zero; 113 encodes exponent -14
    h = (em < (113 << 23)) ? 0 : h;

    // overflow: infinity; 143 encodes exponent 16
    h = (em >= (143 << 23)) ? 0x7c00 : h;

    // NaN; note that we convert all types of NaN to qNaN
    h = (em > (255 << 23)) ? 0x7e00 : h;

    return (uint16_t)(s | h);
}

static inline uint32_t
half_to_float_i(uint16_t h)
{
    uint32_t s = (unsigned)(h & 0x8000) << 16;
    int em = h & 0x7fff;

    // bias exponent and pad mantissa with 0; 112 is relative exponent bias (127-15)
    int r = (em + (112 << 10)) << 13;

    // denormal: flush to zero
    r = (em < (1 << 10)) ? 0 : r;

    // infinity/NaN; note that we preserve NaN payload as a byproduct of unifying inf/nan cases
    // 112 is an exponent bias fixup; since we already applied it once, applying it twice converts 31 to 255
    r += (em >= (31 << 10)) ? (112 << 23) : 0;

    return s | r;
}

static inline uint32_t
float_to_half(float i)
{
    union { float f; uint32_t i; } v;
    v.f = i;
    return float_to_half_i(v.i);
}

static inline uint32_t
half_to_float(uint16_t y)
{
    union { float f; uint32_t i; } v;
    v.i = half_to_float_i(y);
    return v.f;
}

} // namespace anonymous


/* Image Implementation */

namespace bpx {

Image::Image(const std::string& filePath, bool flip_vertically)
    : m_owned(true)
{
    stbi_set_flip_vertically_on_load(flip_vertically);

    int channels{};
    stbi_uc* data = stbi_load(filePath.c_str(), &m_w, &m_h, &channels, 0);

    if (!data) {
        throw std::runtime_error("Fail to load image " + std::string(filePath));
    }

    switch (channels) {
        case 1:
            m_format = PixelFormat::L_U8;
            break;
        case 2:
            m_format = PixelFormat::LA_U8;
            break;
        case 3:
            m_format = PixelFormat::RGB_U8;
            break;
        case 4:
            m_format = PixelFormat::RGBA_U8;
            break;
        default:
            stbi_image_free(data);
            throw std::runtime_error(
                "Unsupported number of channels ("
                + std::to_string(channels)
                + ") in image ("
                + std::string(filePath)
                + ")");
            break;
    }

    m_pixels = data;
}

Image::Image(int w, int h, Color color, PixelFormat format)
    : m_w(w), m_h(h), m_format(format), m_owned(true)
{
    size_t size = w * h;
    m_pixels = std::malloc(size * pixel_size(format));
    if (m_pixels == nullptr) {
        std::bad_alloc();
    }
    for (size_t i = 0; i < size; i++) {
        set_unsafe(i, color);
    }
}

Image::Image(const void* pixels, int w, int h, PixelFormat format)
    : m_w(w), m_h(h), m_format(format), m_owned(true)
{
    size_t size = w * h * pixel_size(format);
    m_pixels = std::malloc(size);
    if (m_pixels == nullptr) {
        std::bad_alloc();
    }
    std::memcpy(m_pixels, pixels, size);
}

Image::Image(void* pixels, int w, int h, PixelFormat format, bool owned)
    : m_pixels(pixels), m_format(format), m_w(w), m_h(h), m_owned(owned)
{ }

Image::~Image()
{
    if (m_owned && m_pixels) {
        std::free(m_pixels);
        m_pixels = nullptr;
    }
}

Image::Image(Image&& other) noexcept
    : m_pixels(other.m_pixels)
    , m_format(other.m_format)
    , m_w(other.m_w)
    , m_h(other.m_h)
    , m_owned(other.m_owned)
{
    other.m_pixels = nullptr;
    other.m_owned = false;
}

Image& Image::operator=(Image&& other) noexcept
{
    if (this != &other) {
        m_pixels = other.m_pixels;
        m_format = other.m_format;
        m_w = other.m_w;
        m_h = other.m_h;
        m_owned = other.m_owned;

        other.m_pixels = nullptr;
        other.m_owned = false;
    }
    return *this;
}

Color Image::get_unsafe(size_t offset) const
{
    Color result{};

    switch (m_format) {

        case PixelFormat::L_U8: {
            uint8_t gray = ((uint8_t*)m_pixels)[offset];
            result = { gray, gray, gray, 255 };
        } break;

        case PixelFormat::L_F16: {
            uint8_t gray = 255 * half_to_float(((uint16_t*)m_pixels)[offset]);
            result = { gray, gray, gray, 255 };
        } break;

        case PixelFormat::L_F32: {
            uint8_t gray = 255 * ((float*)m_pixels)[offset];
            result = { gray, gray, gray, 255 };
        } break;

        case PixelFormat::LA_U8: {
            const uint8_t *pixel = (uint8_t*)m_pixels + 2 * offset;
            uint8_t gray = pixel[0], alpha = pixel[1];
            result = { gray, gray, gray, alpha };
        } break;

        case PixelFormat::LA_F16: {
            const uint16_t *pixel = (uint16_t*)m_pixels + 2 * offset;
            uint8_t gray = 255 * half_to_float(pixel[0]);
            uint8_t alpha = 255 * half_to_float(pixel[1]);
            result = { gray, gray, gray, alpha };
        } break;

        case PixelFormat::LA_F32: {
            const float *pixel = (float*)m_pixels + 2 * offset;
            uint8_t gray = 255 * pixel[0], alpha = 255 * pixel[1];
            result = { gray, gray, gray, alpha };
        } break;

        case PixelFormat::RGB_565: {
            uint16_t pixel = ((uint16_t*)m_pixels)[offset];
            result = {
                static_cast<uint8_t>(((pixel & 0xF800) >> 11) * (255 / 31)),  // 0b1111100000000000
                static_cast<uint8_t>(((pixel & 0x7E0) >> 5) * (255 / 63)),    // 0b0000011111100000
                static_cast<uint8_t>((pixel & 0x1F) * (255 / 31)),            // 0b0000000000011111
                255
            };
        } break;

        case PixelFormat::BGR_565: {
            uint16_t pixel = ((uint16_t*)m_pixels)[offset];
            result = {
                static_cast<uint8_t>((pixel & 0x1F) * (255 / 31)),            // 0b0000000000011111
                static_cast<uint8_t>(((pixel & 0x7E0) >> 5) * (255 / 63)),    // 0b0000011111100000
                static_cast<uint8_t>(((pixel & 0xF800) >> 11) * (255 / 31)),  // 0b1111100000000000
                255
            };
        } break;

        case PixelFormat::RGB_U8: {
            const uint8_t* pixel = (uint8_t*)m_pixels + 3 * offset;
            result = { pixel[0], pixel[1], pixel[2], 255 };
        } break;

        case PixelFormat::BGR_U8: {
            const uint8_t* pixel = (uint8_t*)m_pixels + 3 * offset;
            result = { pixel[2], pixel[1], pixel[0], 255 };
        } break;

        case PixelFormat::RGB_F16: {
            const uint16_t *pixel = (uint16_t*)m_pixels + 3 * offset;
            result = {
                static_cast<uint8_t>(255 * half_to_float(pixel[0])),
                static_cast<uint8_t>(255 * half_to_float(pixel[1])),
                static_cast<uint8_t>(255 * half_to_float(pixel[2])),
                255
            };
        } break;

        case PixelFormat::BGR_F16: {
            const uint16_t *pixel = (uint16_t*)m_pixels + 3 * offset;
            result = {
                static_cast<uint8_t>(255 * half_to_float(pixel[2])),
                static_cast<uint8_t>(255 * half_to_float(pixel[1])),
                static_cast<uint8_t>(255 * half_to_float(pixel[0])),
                255
            };
        } break;

        case PixelFormat::RGB_F32: {
            const float *pixel = (float*)m_pixels + 3 * offset;
            result = {
                static_cast<uint8_t>(255 * pixel[0]),
                static_cast<uint8_t>(255 * pixel[1]),
                static_cast<uint8_t>(255 * pixel[2]),
                255
            };
        } break;

        case PixelFormat::BGR_F32: {
            const float *pixel = (float*)m_pixels + 3 * offset;
            result = {
                static_cast<uint8_t>(255 * pixel[2]),
                static_cast<uint8_t>(255 * pixel[1]),
                static_cast<uint8_t>(255 * pixel[0]),
                255
            };
        } break;

        case PixelFormat::RGBA_5551: {
            uint16_t pixel = ((uint16_t*)m_pixels)[offset];
            result = {
                static_cast<uint8_t>(((pixel & 0xF800) >> 11) * (255 / 31)),    // 0b1111100000000000
                static_cast<uint8_t>(((pixel & 0x7C0) >> 6) * (255 / 31)),      // 0b0000011111000000
                static_cast<uint8_t>(((pixel & 0x3E) >> 1) * (255 / 31)),       // 0b0000000000111110
                static_cast<uint8_t>((pixel & 0x1) * 255)                       // 0b0000000000000001
            };
        } break;

        case PixelFormat::BGRA_5551: {
            uint16_t pixel = ((uint16_t*)m_pixels)[offset];
            result = {
                static_cast<uint8_t>(((pixel & 0x3E) >> 1) * (255 / 31)),       // 0b0000000000111110
                static_cast<uint8_t>(((pixel & 0x7C0) >> 6) * (255 / 31)),      // 0b0000011111000000
                static_cast<uint8_t>(((pixel & 0xF800) >> 11) * (255 / 31)),    // 0b1111100000000000
                static_cast<uint8_t>((pixel & 0x1) * 255)                       // 0b0000000000000001
            };
        } break;

        case PixelFormat::RGBA_4444: {
            uint16_t pixel = ((uint16_t*)m_pixels)[offset];
            result = {
                static_cast<uint8_t>(((pixel & 0xF000) >> 12) * (255 / 15)),  // 0b1111000000000000
                static_cast<uint8_t>(((pixel & 0xF00) >> 8) * (255 / 15)),    // 0b0000111100000000
                static_cast<uint8_t>(((pixel & 0xF0) >> 4) * (255 / 15)),     // 0b0000000011110000
                static_cast<uint8_t>((pixel & 0xF) * (255 / 15))              // 0b0000000000001111
            };
        } break;

        case PixelFormat::BGRA_4444: {
            uint16_t pixel = ((uint16_t*)m_pixels)[offset];
            result = {
                static_cast<uint8_t>(((pixel & 0xF0) >> 4) * (255 / 15)),     // 0b0000000011110000
                static_cast<uint8_t>(((pixel & 0xF00) >> 8) * (255 / 15)),    // 0b0000111100000000
                static_cast<uint8_t>(((pixel & 0xF000) >> 12) * (255 / 15)),  // 0b1111000000000000
                static_cast<uint8_t>((pixel & 0xF) * (255 / 15))              // 0b0000000000001111
            };
        } break;

        case PixelFormat::RGBA_U8: {
            uint8_t *pixel = (uint8_t*)m_pixels + 4 * offset;
            result = {
                pixel[0],
                pixel[1],
                pixel[2],
                pixel[3]
            };
        } break;

        case PixelFormat::BGRA_U8: {
            uint8_t *pixel = (uint8_t*)m_pixels + 4 * offset;
            result = {
                pixel[2],
                pixel[1],
                pixel[0],
                pixel[3]
            };
        } break;

        case PixelFormat::RGBA_F16: {
            const uint16_t *pixel = (uint16_t*)m_pixels + 4 * offset;
            result = {
                static_cast<uint8_t>(255 * half_to_float(pixel[0])),
                static_cast<uint8_t>(255 * half_to_float(pixel[1])),
                static_cast<uint8_t>(255 * half_to_float(pixel[2])),
                static_cast<uint8_t>(255 * half_to_float(pixel[3]))
            };
        } break;

        case PixelFormat::BGRA_F16: {
            const uint16_t *pixel = (uint16_t*)m_pixels + 4 * offset;
            result = {
                static_cast<uint8_t>(255 * half_to_float(pixel[2])),
                static_cast<uint8_t>(255 * half_to_float(pixel[1])),
                static_cast<uint8_t>(255 * half_to_float(pixel[0])),
                static_cast<uint8_t>(255 * half_to_float(pixel[3]))
            };
        } break;

        case PixelFormat::RGBA_F32: {
            const float *pixel = (float*)m_pixels + 4 * offset;
            result = {
                static_cast<uint8_t>(255 * pixel[0]),
                static_cast<uint8_t>(255 * pixel[1]),
                static_cast<uint8_t>(255 * pixel[2]),
                static_cast<uint8_t>(255 * pixel[3])
            };
        } break;

        case PixelFormat::BGRA_F32: {
            const float *pixel = (float*)m_pixels + 4 * offset;
            result = {
                static_cast<uint8_t>(255 * pixel[2]),
                static_cast<uint8_t>(255 * pixel[1]),
                static_cast<uint8_t>(255 * pixel[0]),
                static_cast<uint8_t>(255 * pixel[3])
            };
        } break;
    }

    return result;
}

Image& Image::set_unsafe(size_t offset, Color color)
{
    switch (m_format) {

        case PixelFormat::L_U8: {
            ((uint8_t*)m_pixels)[offset] = luminance_value(color);
        } break;

        case PixelFormat::L_F16: {
            ((uint16_t*)m_pixels)[offset] = float_to_half(luminance_value(color) / 255.0f);
        } break;

        case PixelFormat::L_F32: {
            ((float*)m_pixels)[offset] = luminance_value(color) / 255.0f;
        } break;

        case PixelFormat::LA_U8: {
            uint8_t *pixel = (uint8_t*)m_pixels + 2 * offset;
            pixel[0] = luminance_value(color);
            pixel[1] = color.a;
        } break;

        case PixelFormat::LA_F16: {
            uint16_t *pixel = (uint16_t*)m_pixels + 2 * offset;
            pixel[0] = float_to_half(luminance_value(color) / 255.0f);
            pixel[1] = float_to_half(color.a / 255.0f);
        } break;

        case PixelFormat::LA_F32: {
            float *pixel = (float*)m_pixels + 2 * offset;
            pixel[0] = luminance_value(color) / 255.0f;
            pixel[1] = color.a / 255.0f;
        } break;

        case PixelFormat::RGB_565: {
            uint16_t r = static_cast<uint16_t>(std::round(color.r * (31.0f / 255)));
            uint16_t g = static_cast<uint16_t>(std::round(color.g * (63.0f / 255)));
            uint16_t b = static_cast<uint16_t>(std::round(color.b * (31.0f / 255)));
            ((uint16_t*)m_pixels)[offset] = (r << 11) | (g << 5) | b;
        } break;

        case PixelFormat::BGR_565: {
            uint16_t r = static_cast<uint16_t>(std::round(color.r * (31.0f / 255)));
            uint16_t g = static_cast<uint16_t>(std::round(color.g * (63.0f / 255)));
            uint16_t b = static_cast<uint16_t>(std::round(color.b * (31.0f / 255)));
            ((uint16_t*)m_pixels)[offset] = (b << 11) | (g << 5) | r;
        } break;

        case PixelFormat::RGB_U8: {
            uint8_t* pixel = (uint8_t*)m_pixels + 3 * offset;
            pixel[0] = color.r;
            pixel[1] = color.g;
            pixel[2] = color.b;
        } break;

        case PixelFormat::BGR_U8: {
            uint8_t* pixel = (uint8_t*)m_pixels + 3 * offset;
            pixel[0] = color.b;
            pixel[1] = color.g;
            pixel[2] = color.r;
        } break;

        case PixelFormat::RGB_F16: {
            uint16_t *pixel = (uint16_t*)m_pixels + 3 * offset;
            pixel[0] = float_to_half(color.r / 255.0f);
            pixel[1] = float_to_half(color.g / 255.0f);
            pixel[2] = float_to_half(color.b / 255.0f);
        } break;

        case PixelFormat::BGR_F16: {
            uint16_t *pixel = (uint16_t*)m_pixels + 3 * offset;
            pixel[0] = float_to_half(color.b / 255.0f);
            pixel[1] = float_to_half(color.g / 255.0f);
            pixel[2] = float_to_half(color.r / 255.0f);
        } break;

        case PixelFormat::RGB_F32: {
            float *pixel = (float*)m_pixels + 3 * offset;
            pixel[0] = color.r / 255.0f;
            pixel[1] = color.g / 255.0f;
            pixel[2] = color.b / 255.0f;
        } break;

        case PixelFormat::BGR_F32: {
            float *pixel = (float*)m_pixels + 3 * offset;
            pixel[0] = color.b / 255.0f;
            pixel[1] = color.g / 255.0f;
            pixel[2] = color.r / 255.0f;
        } break;

        case PixelFormat::RGBA_5551: {
            uint16_t r = static_cast<uint16_t>(std::round(color.r * (31.0f / 255)));
            uint16_t g = static_cast<uint16_t>(std::round(color.g * (31.0f / 255)));
            uint16_t b = static_cast<uint16_t>(std::round(color.b * (31.0f / 255)));
            uint16_t a = (static_cast<uint16_t>(color.a) > 50) ? 255 : 0;
            ((uint16_t*)m_pixels)[offset] = (r << 11) | (g << 6) | (b << 1) | a;
        } break;

        case PixelFormat::BGRA_5551: {
            uint16_t r = static_cast<uint16_t>(std::round(color.r * (31.0f / 255)));
            uint16_t g = static_cast<uint16_t>(std::round(color.g * (31.0f / 255)));
            uint16_t b = static_cast<uint16_t>(std::round(color.b * (31.0f / 255)));
            uint16_t a = (static_cast<uint16_t>(color.a) > 50) ? 255 : 0;
            ((uint16_t*)m_pixels)[offset] = (b << 11) | (g << 6) | (r << 1) | a;
        } break;

        case PixelFormat::RGBA_4444: {
            uint16_t r = static_cast<uint16_t>(std::round(color.r * (15.0f / 255)));
            uint16_t g = static_cast<uint16_t>(std::round(color.g * (15.0f / 255)));
            uint16_t b = static_cast<uint16_t>(std::round(color.b * (15.0f / 255)));
            uint16_t a = static_cast<uint16_t>(std::round(color.a * (15.0f / 255)));
            ((uint16_t*)m_pixels)[offset] = (r << 12) | (g << 8) | (b << 4) | a;
        } break;

        case PixelFormat::BGRA_4444: {
            uint16_t r = static_cast<uint16_t>(std::round(color.r * (15.0f / 255)));
            uint16_t g = static_cast<uint16_t>(std::round(color.g * (15.0f / 255)));
            uint16_t b = static_cast<uint16_t>(std::round(color.b * (15.0f / 255)));
            uint16_t a = static_cast<uint16_t>(std::round(color.a * (15.0f / 255)));
            ((uint16_t*)m_pixels)[offset] = (b << 12) | (g << 8) | (r << 4) | a;
        } break;

        case PixelFormat::RGBA_U8: {
            uint8_t *pixel = (uint8_t*)m_pixels + 4 * offset;
            pixel[0] = color.r;
            pixel[1] = color.g;
            pixel[2] = color.b;
            pixel[3] = color.a;
        } break;

        case PixelFormat::BGRA_U8: {
            uint8_t *pixel = (uint8_t*)m_pixels + 4 * offset;
            pixel[0] = color.b;
            pixel[1] = color.g;
            pixel[2] = color.r;
            pixel[3] = color.a;
        } break;

        case PixelFormat::RGBA_F16: {
            uint16_t *pixel = (uint16_t*)m_pixels + 4 * offset;
            pixel[0] = float_to_half(color.r / 255.0f);
            pixel[1] = float_to_half(color.g / 255.0f);
            pixel[2] = float_to_half(color.b / 255.0f);
            pixel[3] = float_to_half(color.a / 255.0f);
        } break;

        case PixelFormat::BGRA_F16: {
            uint16_t *pixel = (uint16_t*)m_pixels + 4 * offset;
            pixel[0] = float_to_half(color.b / 255.0f);
            pixel[1] = float_to_half(color.g / 255.0f);
            pixel[2] = float_to_half(color.r / 255.0f);
            pixel[3] = float_to_half(color.a / 255.0f);
        } break;

        case PixelFormat::RGBA_F32: {
            float *pixel = (float*)m_pixels + 4 * offset;
            pixel[0] = color.r / 255.0f;
            pixel[1] = color.g / 255.0f;
            pixel[2] = color.b / 255.0f;
            pixel[3] = color.a / 255.0f;
        } break;

        case PixelFormat::BGRA_F32: {
            float *pixel = (float*)m_pixels + 4 * offset;
            pixel[0] = color.b / 255.0f;
            pixel[1] = color.g / 255.0f;
            pixel[2] = color.r / 255.0f;
            pixel[3] = color.a / 255.0f;
        } break;

    }

    return *this;
}

} // namespace bpx
