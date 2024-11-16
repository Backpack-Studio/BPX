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

#include <stdexcept>
#include <cstddef>
#include <cstdint>
#include <tuple>

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
            return 1;               ///< Luminance (grayscale) with unsigned 8-bit values.

        case PixelFormat::L_F16:
        case PixelFormat::LA_U8:
            return 2;               ///< Luminance (16-bit floating-point) or Luminance + Alpha with 8-bit values.

        case PixelFormat::RGB_U8:
        case PixelFormat::BGR_U8:
            return 3;               ///< RGB or BGR format with unsigned 8-bit values (3 channels).

        case PixelFormat::L_F32:
        case PixelFormat::LA_F16:
        case PixelFormat::RGB_565:
        case PixelFormat::BGR_565:
            return 4;               /*< Luminance (32-bit floating-point), Luminance + Alpha (16-bit),
                                     *  or RGB/BGR formats with 5-6 bits per channel.
                                     */

        case PixelFormat::RGB_F16:
        case PixelFormat::BGR_F16:
            return 6;               ///< RGB or BGR format with 16-bit floating-point values (per channel).

        case PixelFormat::LA_F32:
            return 8;               ///< Luminance + Alpha format with 32-bit floating-point values (per channel).

        case PixelFormat::RGB_F32:
        case PixelFormat::BGR_F32:
            return 12;              ///< RGB or BGR format with 32-bit floating-point values (per channel).

        case PixelFormat::RGBA_5551:
        case PixelFormat::BGRA_5551:
        case PixelFormat::RGBA_4444:
        case PixelFormat::BGRA_4444:
            return 16;              ///< Packed RGBA/BGRA formats with 5/4 bits for each channel.

        case PixelFormat::RGBA_U8:
        case PixelFormat::BGRA_U8:
            return 32;              ///< RGBA/BGRA format with unsigned 8-bit values (per channel).

        case PixelFormat::RGBA_F16:
        case PixelFormat::BGRA_F16:
            return 64;              ///< RGBA/BGRA format with 16-bit floating-point values (per channel).

        case PixelFormat::RGBA_F32:
        case PixelFormat::BGRA_F32:
            return 128;             ///< RGBA/BGRA format with 32-bit floating-point values (per channel).

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
            return 1;

        case PixelFormat::LA_U8:
        case PixelFormat::LA_F16:
        case PixelFormat::LA_F32:
            return 2;

        case PixelFormat::RGB_565:
        case PixelFormat::BGR_565:
        case PixelFormat::RGB_U8:
        case PixelFormat::BGR_U8:
        case PixelFormat::RGB_F16:
        case PixelFormat::BGR_F16:
        case PixelFormat::RGB_F32:
        case PixelFormat::BGR_F32:
            return 3;

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
            return 4;

    }

    return 0;
}

/**
 * @brief Retrieves the OpenGL format, internal format, and data type corresponding to a given pixel format.
 *
 * This function maps a `PixelFormat` enum to the corresponding OpenGL constants for texture creation. 
 * It returns a tuple containing:
 * - The OpenGL format (e.g., `GL_RGB`, `GL_RGBA`, `GL_RED`, etc.).
 * - The OpenGL internal format, typically the same as the format.
 * - The OpenGL data type (e.g., `GL_UNSIGNED_BYTE`, `GL_FLOAT`, etc.).
 *
 * @param format The pixel format to map, specified as a value from the `PixelFormat` enum.
 * 
 * @return A `std::tuple` containing:
 * - The OpenGL format (uint32_t).
 * - The OpenGL internal format (uint32_t).
 * - The OpenGL data type (uint32_t).
 *
 * @throws std::runtime_error if the pixel format is unknown or unsupported for OpenGL.
 *
 * @note This function does not depend on OpenGL headers, and constants are defined explicitly in the implementation.
 *
 * ### Example
 * ```cpp
 * auto [format, internalFormat, dataType] = get_gl_format_info(PixelFormat::RGBA_U8);
 * // format == GL_RGBA
 * // internalFormat == GL_RGBA
 * // dataType == GL_UNSIGNED_BYTE
 * ```
 *
 * ### Supported Pixel Formats
 * - Luminance (Grayscale): `L_U8`, `L_F16`, `L_F32`
 * - Luminance + Alpha: `LA_U8`, `LA_F16`, `LA_F32`
 * - RGB: `RGB_565`, `RGB_U8`, `RGB_F16`, `RGB_F32`
 * - BGR: `BGR_565`, `BGR_U8`, `BGR_F16`, `BGR_F32`
 * - RGBA: `RGBA_5551`, `RGBA_4444`, `RGBA_U8`, `RGBA_F16`, `RGBA_F32`
 * - BGRA: `BGRA_5551`, `BGRA_4444`, `BGRA_U8`, `BGRA_F16`, `BGRA_F32`
 *
 * ### Unsupported Pixel Formats
 * For unsupported formats, the function will throw a `std::runtime_error`.
 */
constexpr std::tuple<uint32_t, uint32_t, uint32_t>
get_gl_format_info(PixelFormat format)
{
    constexpr uint32_t GL_UNSIGNED_BYTE          = 0x1401;
    constexpr uint32_t GL_UNSIGNED_SHORT         = 0x1403;
    constexpr uint32_t GL_UNSIGNED_INT           = 0x1405;
    constexpr uint32_t GL_HALF_FLOAT             = 0x140B;
    constexpr uint32_t GL_FLOAT                  = 0x1406;
    constexpr uint32_t GL_RED                    = 0x1903;
    constexpr uint32_t GL_RG                     = 0x8227;
    constexpr uint32_t GL_RGB                    = 0x1907;
    constexpr uint32_t GL_BGR                    = 0x80E0;
    constexpr uint32_t GL_RGBA                   = 0x1908;
    constexpr uint32_t GL_BGRA                   = 0x80E1;
    constexpr uint32_t GL_UNSIGNED_SHORT_5_6_5   = 0x8363;
    constexpr uint32_t GL_UNSIGNED_SHORT_5_5_5_1 = 0x8034;
    constexpr uint32_t GL_UNSIGNED_SHORT_4_4_4_4 = 0x8033;

    switch (format) {
        case PixelFormat::L_U8:
            return { GL_RED, GL_RED, GL_UNSIGNED_BYTE };
        case PixelFormat::L_F16:
            return { GL_RED, GL_RED, GL_HALF_FLOAT };
        case PixelFormat::L_F32:
            return { GL_RED, GL_RED, GL_FLOAT };
        case PixelFormat::LA_U8:
            return { GL_RG, GL_RG, GL_UNSIGNED_BYTE };
        case PixelFormat::LA_F16:
            return { GL_RG, GL_RG, GL_HALF_FLOAT };
        case PixelFormat::LA_F32:
            return { GL_RG, GL_RG, GL_FLOAT };

        case PixelFormat::RGB_565:
            return { GL_RGB, GL_RGB, GL_UNSIGNED_SHORT_5_6_5 };
        case PixelFormat::BGR_565:
            return { GL_RGB, GL_BGR, GL_UNSIGNED_SHORT_5_6_5 };
        case PixelFormat::RGB_U8:
            return { GL_RGB, GL_RGB, GL_UNSIGNED_BYTE };
        case PixelFormat::BGR_U8:
            return { GL_RGB, GL_BGR, GL_UNSIGNED_BYTE };
        case PixelFormat::RGB_F16:
            return { GL_RGB, GL_RGB, GL_HALF_FLOAT };
        case PixelFormat::BGR_F16:
            return { GL_RGB, GL_BGR, GL_HALF_FLOAT };
        case PixelFormat::RGB_F32:
            return { GL_RGB, GL_RGB, GL_FLOAT };
        case PixelFormat::BGR_F32:
            return { GL_RGB, GL_BGR, GL_FLOAT };

        case PixelFormat::RGBA_5551:
            return { GL_RGBA, GL_RGBA, GL_UNSIGNED_SHORT_5_5_5_1 };
        case PixelFormat::BGRA_5551:
            return { GL_RGBA, GL_BGRA, GL_UNSIGNED_SHORT_5_5_5_1 };
        case PixelFormat::RGBA_4444:
            return { GL_RGBA, GL_RGBA, GL_UNSIGNED_SHORT_4_4_4_4 };
        case PixelFormat::BGRA_4444:
            return { GL_RGBA, GL_BGRA, GL_UNSIGNED_SHORT_4_4_4_4 };
        case PixelFormat::RGBA_U8:
            return { GL_RGBA, GL_RGBA, GL_UNSIGNED_BYTE };
        case PixelFormat::BGRA_U8:
            return { GL_RGBA, GL_BGRA, GL_UNSIGNED_BYTE };
        case PixelFormat::RGBA_F16:
            return { GL_RGBA, GL_RGBA, GL_HALF_FLOAT };
        case PixelFormat::BGRA_F16:
            return { GL_RGBA, GL_BGRA, GL_HALF_FLOAT };
        case PixelFormat::RGBA_F32:
            return { GL_RGBA, GL_RGBA, GL_FLOAT };
        case PixelFormat::BGRA_F32:
            return { GL_RGBA, GL_BGRA, GL_FLOAT };
    }

    throw std::runtime_error("Unknown pixel format for OpenGL");
}

/**
 * @brief Retrieves the Vulkan format, internal format, and component count corresponding to a given pixel format.
 *
 * This function maps a `PixelFormat` enum to the corresponding Vulkan constants for image creation. 
 * It returns a tuple containing:
 * - The Vulkan format (e.g., `VK_FORMAT_R8_UNORM`, `VK_FORMAT_R32_SFLOAT`, etc.).
 * - The Vulkan internal format, typically the same as the format.
 * - The number of components in the pixel format (e.g., 1 for grayscale, 4 for RGBA).
 *
 * @param format The pixel format to map, specified as a value from the `PixelFormat` enum.
 * 
 * @return A `std::tuple` containing:
 * - The Vulkan format (uint32_t).
 * - The Vulkan internal format (uint32_t).
 * - The number of components (uint32_t).
 *
 * @throws std::runtime_error if the pixel format is unknown or unsupported for Vulkan.
 *
 * @note This function does not depend on Vulkan headers, and constants are defined explicitly in the implementation.
 *
 * ### Example
 * ```cpp
 * auto [format, internalFormat, componentCount] = get_vk_format_info(PixelFormat::RGBA_U8);
 * // format == VK_FORMAT_R8G8B8A8_UNORM
 * // internalFormat == VK_FORMAT_R8G8B8A8_UNORM
 * // componentCount == 4
 * ```
 *
 * ### Supported Pixel Formats
 * - Luminance (Grayscale): `L_U8`, `L_F16`, `L_F32`
 * - Luminance + Alpha: `LA_U8`, `LA_F16`, `LA_F32`
 * - RGB: `RGB_565`, `RGB_U8`, `RGB_F16`, `RGB_F32`
 * - BGR: `BGR_565`, `BGR_U8`, `BGR_F16`, `BGR_F32`
 * - RGBA: `RGBA_5551`, `RGBA_4444`, `RGBA_U8`, `RGBA_F16`, `RGBA_F32`
 * - BGRA: `BGRA_5551`, `BGRA_4444`, `BGRA_U8`, `BGRA_F16`, `BGRA_F32`
 *
 * ### Unsupported Pixel Formats
 * For unsupported formats, the function will throw a `std::runtime_error`.
 */
constexpr std::tuple<uint32_t, uint32_t, uint32_t>
get_vk_format_info(PixelFormat format)
{
    constexpr uint32_t VK_FORMAT_R8_UNORM              = 9;
    constexpr uint32_t VK_FORMAT_R16_SFLOAT           = 70;
    constexpr uint32_t VK_FORMAT_R32_SFLOAT           = 100;
    constexpr uint32_t VK_FORMAT_R8G8_UNORM           = 16;
    constexpr uint32_t VK_FORMAT_R16G16_SFLOAT        = 81;
    constexpr uint32_t VK_FORMAT_R32G32_SFLOAT        = 103;
    constexpr uint32_t VK_FORMAT_R5G6B5_UNORM_PACK16  = 23;
    constexpr uint32_t VK_FORMAT_B5G6R5_UNORM_PACK16  = 24;
    constexpr uint32_t VK_FORMAT_R8G8B8_UNORM         = 23;
    constexpr uint32_t VK_FORMAT_B8G8R8_UNORM         = 28;
    constexpr uint32_t VK_FORMAT_R16G16B16_SFLOAT     = 82;
    constexpr uint32_t VK_FORMAT_B16G16R16_SFLOAT     = 84;
    constexpr uint32_t VK_FORMAT_R32G32B32_SFLOAT     = 106;
    constexpr uint32_t VK_FORMAT_B32G32R32_SFLOAT     = 107;
    constexpr uint32_t VK_FORMAT_R5G5B5A1_UNORM_PACK16 = 25;
    constexpr uint32_t VK_FORMAT_B5G5R5A1_UNORM_PACK16 = 26;
    constexpr uint32_t VK_FORMAT_R4G4B4A4_UNORM_PACK16 = 29;
    constexpr uint32_t VK_FORMAT_B4G4R4A4_UNORM_PACK16 = 30;
    constexpr uint32_t VK_FORMAT_R8G8B8A8_UNORM       = 37;
    constexpr uint32_t VK_FORMAT_B8G8R8A8_UNORM       = 44;
    constexpr uint32_t VK_FORMAT_R16G16B16A16_SFLOAT  = 83;
    constexpr uint32_t VK_FORMAT_B16G16R16A16_SFLOAT  = 85;
    constexpr uint32_t VK_FORMAT_R32G32B32A32_SFLOAT  = 108;
    constexpr uint32_t VK_FORMAT_B32G32R32A32_SFLOAT  = 109;

    switch (format) {
        case PixelFormat::L_U8:
            return { VK_FORMAT_R8_UNORM, VK_FORMAT_R8_UNORM, 1 };
        case PixelFormat::L_F16:
            return { VK_FORMAT_R16_SFLOAT, VK_FORMAT_R16_SFLOAT, 1 };
        case PixelFormat::L_F32:
            return { VK_FORMAT_R32_SFLOAT, VK_FORMAT_R32_SFLOAT, 1 };
        case PixelFormat::LA_U8:
            return { VK_FORMAT_R8G8_UNORM, VK_FORMAT_R8G8_UNORM, 2 };
        case PixelFormat::LA_F16:
            return { VK_FORMAT_R16G16_SFLOAT, VK_FORMAT_R16G16_SFLOAT, 2 };
        case PixelFormat::LA_F32:
            return { VK_FORMAT_R32G32_SFLOAT, VK_FORMAT_R32G32_SFLOAT, 2 };

        case PixelFormat::RGB_565:
            return { VK_FORMAT_R5G6B5_UNORM_PACK16, VK_FORMAT_R5G6B5_UNORM_PACK16, 2 };
        case PixelFormat::BGR_565:
            return { VK_FORMAT_B5G6R5_UNORM_PACK16, VK_FORMAT_B5G6R5_UNORM_PACK16, 2 };
        case PixelFormat::RGB_U8:
            return { VK_FORMAT_R8G8B8_UNORM, VK_FORMAT_R8G8B8_UNORM, 3 };
        case PixelFormat::BGR_U8:
            return { VK_FORMAT_B8G8R8_UNORM, VK_FORMAT_B8G8R8_UNORM, 3 };
        case PixelFormat::RGB_F16:
            return { VK_FORMAT_R16G16B16_SFLOAT, VK_FORMAT_R16G16B16_SFLOAT, 3 };
        case PixelFormat::BGR_F16:
            return { VK_FORMAT_B16G16R16_SFLOAT, VK_FORMAT_B16G16R16_SFLOAT, 3 };
        case PixelFormat::RGB_F32:
            return { VK_FORMAT_R32G32B32_SFLOAT, VK_FORMAT_R32G32B32_SFLOAT, 3 };
        case PixelFormat::BGR_F32:
            return { VK_FORMAT_B32G32R32_SFLOAT, VK_FORMAT_B32G32R32_SFLOAT, 3 };

        case PixelFormat::RGBA_5551:
            return { VK_FORMAT_R5G5B5A1_UNORM_PACK16, VK_FORMAT_R5G5B5A1_UNORM_PACK16, 2 };
        case PixelFormat::BGRA_5551:
            return { VK_FORMAT_B5G5R5A1_UNORM_PACK16, VK_FORMAT_B5G5R5A1_UNORM_PACK16, 2 };
        case PixelFormat::RGBA_4444:
            return { VK_FORMAT_R4G4B4A4_UNORM_PACK16, VK_FORMAT_R4G4B4A4_UNORM_PACK16, 2 };
        case PixelFormat::BGRA_4444:
            return { VK_FORMAT_B4G4R4A4_UNORM_PACK16, VK_FORMAT_B4G4R4A4_UNORM_PACK16, 2 };
        case PixelFormat::RGBA_U8:
            return { VK_FORMAT_R8G8B8A8_UNORM, VK_FORMAT_R8G8B8A8_UNORM, 4 };
        case PixelFormat::BGRA_U8:
            return { VK_FORMAT_B8G8R8A8_UNORM, VK_FORMAT_B8G8R8A8_UNORM, 4 };
        case PixelFormat::RGBA_F16:
            return { VK_FORMAT_R16G16B16A16_SFLOAT, VK_FORMAT_R16G16B16A16_SFLOAT, 4 };
        case PixelFormat::BGRA_F16:
            return { VK_FORMAT_B16G16R16A16_SFLOAT, VK_FORMAT_B16G16R16A16_SFLOAT, 4 };
        case PixelFormat::RGBA_F32:
            return { VK_FORMAT_R32G32B32A32_SFLOAT, VK_FORMAT_R32G32B32A32_SFLOAT, 4 };
        case PixelFormat::BGRA_F32:
            return { VK_FORMAT_B32G32R32A32_SFLOAT, VK_FORMAT_B32G32R32A32_SFLOAT, 4 };
    }

    throw std::runtime_error("Unknown pixel format for Vulkan");
}

/**
 * @brief Retrieves the DirectX format, internal format, and component count corresponding to a given pixel format.
 *
 * This function maps a `PixelFormat` enum to the corresponding DXGI constants for use in DirectX-based applications.
 * It returns a tuple containing:
 * - The DXGI format (e.g., `DXGI_FORMAT_R8_UNORM`, `DXGI_FORMAT_R32_FLOAT`, etc.).
 * - The DXGI internal format, typically the same as the format.
 * - The number of components in the pixel format (e.g., 1 for grayscale, 4 for RGBA).
 *
 * @param format The pixel format to map, specified as a value from the `PixelFormat` enum.
 * 
 * @return A `std::tuple` containing:
 * - The DXGI format (uint32_t).
 * - The DXGI internal format (uint32_t).
 * - The number of components (uint32_t).
 *
 * @throws std::runtime_error if the pixel format is unsupported or has no direct DXGI equivalent.
 *
 * @note DXGI format support is limited for certain formats, particularly those without alpha channels or specific bit-depths.
 *
 * ### Example
 * ```cpp
 * auto [format, internalFormat, componentCount] = get_dx_format_info(PixelFormat::RGBA_U8);
 * // format == DXGI_FORMAT_R8G8B8A8_UNORM
 * // internalFormat == DXGI_FORMAT_R8G8B8A8_UNORM
 * // componentCount == 4
 * ```
 *
 * ### Supported Pixel Formats
 * - Luminance (Grayscale): `L_U8`, `L_F16`, `L_F32`
 * - Luminance + Alpha: `LA_U8`, `LA_F16`, `LA_F32`
 * - RGB 565: `RGB_565`, `BGR_565`
 * - RGBA: `RGBA_5551`, `BGRA_5551`, `RGBA_U8`, `BGRA_U8`, `RGBA_F16`, `RGBA_F32`
 *
 * ### Unsupported Pixel Formats
 * The following formats are not natively supported by DXGI:
 * - `RGB_U8`, `BGR_U8`, `RGB_F16`, `BGR_F16`, `RGB_F32`, `BGR_F32`
 * - `RGBA_4444`, `BGRA_4444`
 * - `BGRA_F16`, `BGRA_F32`
 * 
 * For unsupported formats, the function throws a `std::runtime_error` with an appropriate error message.
 */
constexpr std::tuple<uint32_t, uint32_t, uint32_t>
get_dx_format_info(PixelFormat format)
{
    constexpr uint32_t DXGI_FORMAT_R8_UNORM            = 61;
    constexpr uint32_t DXGI_FORMAT_R16_FLOAT           = 54;
    constexpr uint32_t DXGI_FORMAT_R32_FLOAT           = 41;
    constexpr uint32_t DXGI_FORMAT_R8G8_UNORM          = 49;
    constexpr uint32_t DXGI_FORMAT_R16G16_FLOAT        = 35;
    constexpr uint32_t DXGI_FORMAT_R32G32_FLOAT        = 16;
    constexpr uint32_t DXGI_FORMAT_B5G6R5_UNORM        = 85;
    constexpr uint32_t DXGI_FORMAT_B5G5R5A1_UNORM      = 86;
    constexpr uint32_t DXGI_FORMAT_R8G8B8A8_UNORM      = 28;
    constexpr uint32_t DXGI_FORMAT_B8G8R8A8_UNORM      = 87;
    constexpr uint32_t DXGI_FORMAT_R16G16B16A16_FLOAT  = 10;
    constexpr uint32_t DXGI_FORMAT_R32G32B32A32_FLOAT  = 6;

    switch (format) {
        case PixelFormat::L_U8:
            return { DXGI_FORMAT_R8_UNORM, DXGI_FORMAT_R8_UNORM, 1 };
        case PixelFormat::L_F16:
            return { DXGI_FORMAT_R16_FLOAT, DXGI_FORMAT_R16_FLOAT, 2 };
        case PixelFormat::L_F32:
            return { DXGI_FORMAT_R32_FLOAT, DXGI_FORMAT_R32_FLOAT, 4 };
        case PixelFormat::LA_U8:
            return { DXGI_FORMAT_R8G8_UNORM, DXGI_FORMAT_R8G8_UNORM, 2 };
        case PixelFormat::LA_F16:
            return { DXGI_FORMAT_R16G16_FLOAT, DXGI_FORMAT_R16G16_FLOAT, 4 };
        case PixelFormat::LA_F32:
            return { DXGI_FORMAT_R32G32_FLOAT, DXGI_FORMAT_R32G32_FLOAT, 8 };

        case PixelFormat::RGB_565:
            return { DXGI_FORMAT_B5G6R5_UNORM, DXGI_FORMAT_B5G6R5_UNORM, 2 };
        case PixelFormat::BGR_565:
            return { DXGI_FORMAT_B5G6R5_UNORM, DXGI_FORMAT_B5G6R5_UNORM, 2 }; // DXGI ne distingue pas RGB/BGR_565.
        case PixelFormat::RGB_U8:
            throw std::runtime_error("DXGI does not natively support RGB_U8 without alpha.");
        case PixelFormat::BGR_U8:
            throw std::runtime_error("DXGI does not natively support BGR_U8 without alpha.");
        case PixelFormat::RGB_F16:
        case PixelFormat::BGR_F16:
            throw std::runtime_error("DXGI does not natively support RGB_F16/BGR_F16 without alpha.");
        case PixelFormat::RGB_F32:
        case PixelFormat::BGR_F32:
            throw std::runtime_error("DXGI does not natively support RGB_F32/BGR_F32 without alpha.");

        case PixelFormat::RGBA_5551:
            return { DXGI_FORMAT_B5G5R5A1_UNORM, DXGI_FORMAT_B5G5R5A1_UNORM, 2 };
        case PixelFormat::BGRA_5551:
            return { DXGI_FORMAT_B5G5R5A1_UNORM, DXGI_FORMAT_B5G5R5A1_UNORM, 2 }; // DXGI utilise toujours BGRA_5551.
        case PixelFormat::RGBA_4444:
            throw std::runtime_error("DXGI does not natively support RGBA_4444.");
        case PixelFormat::BGRA_4444:
            throw std::runtime_error("DXGI does not natively support BGRA_4444.");
        case PixelFormat::RGBA_U8:
            return { DXGI_FORMAT_R8G8B8A8_UNORM, DXGI_FORMAT_R8G8B8A8_UNORM, 4 };
        case PixelFormat::BGRA_U8:
            return { DXGI_FORMAT_B8G8R8A8_UNORM, DXGI_FORMAT_B8G8R8A8_UNORM, 4 };
        case PixelFormat::RGBA_F16:
            return { DXGI_FORMAT_R16G16B16A16_FLOAT, DXGI_FORMAT_R16G16B16A16_FLOAT, 8 };
        case PixelFormat::BGRA_F16:
            throw std::runtime_error("DXGI does not natively support BGRA_F16.");
        case PixelFormat::RGBA_F32:
            return { DXGI_FORMAT_R32G32B32A32_FLOAT, DXGI_FORMAT_R32G32B32A32_FLOAT, 16 };
        case PixelFormat::BGRA_F32:
            throw std::runtime_error("DXGI does not natively support BGRA_F32.");
    }

    throw std::runtime_error("Unknown pixel format for DirectX");
}

} // namespace bpx

#endif // BPX_PIXEL_HPP
