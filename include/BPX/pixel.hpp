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

#ifndef BPX_PIXEL_HPP
#define BPX_PIXEL_HPP

#include <cstddef>

namespace bpx {

/**
 * @brief Enumeration of various pixel formats.
 *
 * This enum defines a collection of pixel formats used for representing image data.
 * Each format specifies how pixel information is stored, including the color channels,
 * bit-depth, and sometimes the alpha channel (transparency). The formats include 
 * integer and floating-point types, as well as different color channel orders (RGB, BGR, etc.).
 *
 * The formats are organized as follows:
 *
 * - **L**: Luminance (grayscale) values with various bit depths (e.g., 8-bit, 16-bit, 32-bit).
 * - **LA**: Luminance and Alpha (grayscale + transparency) with various bit depths.
 * - **RGB**: Red, Green, Blue color channels with various bit depths.
 * - **BGR**: Red, Green, Blue color channels in reverse order (Blue, Green, Red).
 * - **RGBA**: Red, Green, Blue, Alpha color channels (with transparency) with various bit depths.
 * - **BGRA**: Red, Green, Blue, Alpha color channels in reverse order (Blue, Green, Red, Alpha).
 *
 * Different color and alpha channel representations are provided:
 * - `_U8`: Unsigned 8-bit values (per channel).
 * - `_F16`: 16-bit floating-point values (per channel).
 * - `_F32`: 32-bit floating-point values (per channel).
 * - `_5551`: 5 bits for Red, 5 bits for Green, 5 bits for Blue, and 1 bit for Alpha.
 * - `_4444`: 4 bits for Red, 4 bits for Green, 4 bits for Blue, and 4 bits for Alpha.
 *
 * The following pixel formats are supported:
 */
enum class PixelFormat
{
    L_U8,           ///< Luminance (grayscale) format with unsigned 8-bit values.
    L_F16,          ///< Luminance (grayscale) format with 16-bit floating-point values.
    L_F32,          ///< Luminance (grayscale) format with 32-bit floating-point values.
    LA_U8,          ///< Luminance + Alpha format with unsigned 8-bit values.
    LA_F16,         ///< Luminance + Alpha format with 16-bit floating-point values.
    LA_F32,         ///< Luminance + Alpha format with 32-bit floating-point values.

    RGB_565,        ///< RGB format with 5 bits for Red, 6 bits for Green, and 5 bits for Blue.
    BGR_565,        ///< BGR format with 5 bits for Blue, 6 bits for Green, and 5 bits for Red.
    RGB_U8,         ///< RGB format with unsigned 8-bit values (per channel).
    BGR_U8,         ///< BGR format with unsigned 8-bit values (per channel).
    RGB_F16,        ///< RGB format with 16-bit floating-point values (per channel).
    BGR_F16,        ///< BGR format with 16-bit floating-point values (per channel).
    RGB_F32,        ///< RGB format with 32-bit floating-point values (per channel).
    BGR_F32,        ///< BGR format with 32-bit floating-point values (per channel).

    RGBA_5551,      ///< RGBA format with 5 bits for Red, 5 bits for Green, 5 bits for Blue, and 1 bit for Alpha.
    BGRA_5551,      ///< BGRA format with 5 bits for Blue, 5 bits for Green, 5 bits for Red, and 1 bit for Alpha.
    RGBA_4444,      ///< RGBA format with 4 bits for each channel (Red, Green, Blue, Alpha).
    BGRA_4444,      ///< BGRA format with 4 bits for each channel (Blue, Green, Red, Alpha).
    RGBA_U8,        ///< RGBA format with unsigned 8-bit values (per channel).
    BGRA_U8,        ///< BGRA format with unsigned 8-bit values (per channel).
    RGBA_F16,       ///< RGBA format with 16-bit floating-point values (per channel).
    BGRA_F16,       ///< BGRA format with 16-bit floating-point values (per channel).
    RGBA_F32,       ///< RGBA format with 32-bit floating-point values (per channel).
    BGRA_F32,       ///< BGRA format with 32-bit floating-point values (per channel).
};

/**
 * @brief Calculates the size (in bytes) of a single pixel for a given pixel format.
 *
 * This function returns the number of bytes required to store a single pixel in memory
 * for the specified `PixelFormat`. The size depends on the number of channels (such as
 * Red, Green, Blue, Alpha, etc.) and the bit-depth (such as 8-bit, 16-bit, 32-bit, floating-point, etc.)
 * used in the pixel format.
 *
 * @param format The pixel format whose pixel size is to be determined.
 * 
 * @return The size (in bytes) of a single pixel for the given `PixelFormat`.
 */
constexpr size_t pixel_size(PixelFormat format) noexcept {
    switch (format) {

        case PixelFormat::L_U8:
            return 1;  ///< Luminance (grayscale) with unsigned 8-bit values.

        case PixelFormat::L_F16:
        case PixelFormat::LA_U8:
            return 2;  ///< Luminance (16-bit floating-point) or Luminance + Alpha with 8-bit values.

        case PixelFormat::RGB_U8:
        case PixelFormat::BGR_U8:
            return 3;  ///< RGB or BGR format with unsigned 8-bit values (3 channels).

        case PixelFormat::L_F32:
        case PixelFormat::LA_F16:
        case PixelFormat::RGB_565:
        case PixelFormat::BGR_565:
            return 4;  ///< Luminance (32-bit floating-point), Luminance + Alpha (16-bit), or RGB/BGR formats with 5-6 bits per channel.

        case PixelFormat::RGB_F16:
        case PixelFormat::BGR_F16:
            return 6;  ///< RGB or BGR format with 16-bit floating-point values (per channel).

        case PixelFormat::LA_F32:
            return 8;  ///< Luminance + Alpha format with 32-bit floating-point values (per channel).

        case PixelFormat::RGB_F32:
        case PixelFormat::BGR_F32:
            return 12; ///< RGB or BGR format with 32-bit floating-point values (per channel).

        case PixelFormat::RGBA_5551:
        case PixelFormat::BGRA_5551:
        case PixelFormat::RGBA_4444:
        case PixelFormat::BGRA_4444:
            return 16; ///< Packed RGBA/BGRA formats with 5/4 bits for each channel.

        case PixelFormat::RGBA_U8:
        case PixelFormat::BGRA_U8:
            return 32; ///< RGBA/BGRA format with unsigned 8-bit values (per channel).

        case PixelFormat::RGBA_F16:
        case PixelFormat::BGRA_F16:
            return 64; ///< RGBA/BGRA format with 16-bit floating-point values (per channel).

        case PixelFormat::RGBA_F32:
        case PixelFormat::BGRA_F32:
            return 128; ///< RGBA/BGRA format with 32-bit floating-point values (per channel).

    }

    return 0;
}

/**
 * @brief Returns the number of components (channels) in a given pixel format.
 *
 * This function returns the number of components (such as Red, Green, Blue, Alpha, etc.)
 * used to represent a single pixel for the specified `PixelFormat`. This is useful when 
 * determining how many values need to be read or written for a pixel.
 *
 * @param format The pixel format whose number of components is to be determined.
 * 
 * @return The number of components (channels) in the specified `PixelFormat`.
 */
constexpr size_t pixel_comp(PixelFormat format) noexcept {
    switch (format) {

        case PixelFormat::L_U8:
        case PixelFormat::L_F16:
        case PixelFormat::L_F32:
            return 1;  ///< Luminance formats (1 component).

        case PixelFormat::LA_U8:
        case PixelFormat::LA_F16:
        case PixelFormat::LA_F32:
            return 2;  ///< Luminance + Alpha formats (2 components).

        case PixelFormat::RGB_565:
        case PixelFormat::BGR_565:
        case PixelFormat::RGB_U8:
        case PixelFormat::BGR_U8:
        case PixelFormat::RGB_F16:
        case PixelFormat::BGR_F16:
        case PixelFormat::RGB_F32:
        case PixelFormat::BGR_F32:
            return 3;  ///< RGB or BGR formats (3 components).

        case PixelFormat::RGBA_5551:
        case PixelFormat::BGRA_5551:
        case PixelFormat::RGBA_4444:
        case PixelFormat::BGRA_4444:
        case PixelFormat::RGBA_U8:
        case PixelFormat::BGRA_U8:
        case PixelFormat::RGBA_F16:
        case PixelFormat::BGRA_F16:
        case PixelFormat::RGBA_F32:
        case PixelFormat::BGRA_F32:
            return 4;  ///< RGBA or BGRA formats (4 components).

    }

    return 0;
}

} // namespace bpx

#endif // BPX_PIXEL_HPP
