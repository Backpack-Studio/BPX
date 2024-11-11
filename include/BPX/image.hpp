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

#ifndef BPX_IMAGE_HPP
#define BPX_IMAGE_HPP

#include "./color.hpp"
#include "./pixel.hpp"

#include <functional>
#include <string>

namespace bpx {

/**
 * @class Image
 * @brief A class that represents an image with pixel data.
 */
class Image
{
public:
    /**
     * @typedef Mapper
     * @brief A type alias for a function that maps pixel color at given coordinates.
     *
     * The `Mapper` type is a function that can be used to apply color transformations to
     * individual pixels of an image. It takes the `x` and `y` coordinates of the pixel
     * and the current color of the pixel, returning a new color.
     */
    using Mapper = std::function<Color(int x, int y, Color color)>;

public:
    /**
     * @brief Constructs an image from a file.
     *
     * This constructor loads an image from the specified file path. Optionally, it can
     * flip the image vertically after loading.
     *
     * @param file_path The path to the image file.
     * @param flip_vertically Whether to flip the image vertically after loading.
     */
    Image(const std::string& file_path, bool flip_vertically = false);

    /**
     * @brief Constructs an image from a single color.
     *
     * This constructor creates a new image of the specified size and pixel format, where
     * all pixels are initialized to the specified color.
     *
     * @param color The color to initialize all pixels with.
     * @param w The width of the image in pixels.
     * @param h The height of the image in pixels.
     * @param format The pixel format to use for the image.
     */
    Image(Color color, int w, int h, PixelFormat format = PixelFormat::RGBA_U8);

    /**
     * @brief Constructs an image from raw pixel data.
     *
     * This constructor creates a new image using the provided raw pixel data.
     *
     * @param pixels A pointer to the raw pixel data.
     * @param w The width of the image in pixels.
     * @param h The height of the image in pixels.
     * @param format The pixel format to use for the image.
     */
    Image(const void* pixels, int w, int h, PixelFormat format);

    /**
     * @brief Constructs an image from raw pixel data with ownership control.
     *
     * This constructor creates an image using the provided pixel data. It allows the caller
     * to control whether the `Image` class should take ownership of the pixel data and 
     * whether the data should be freed when the image is destroyed.
     *
     * @param pixels A pointer to the raw pixel data.
     * @param w The width of the image in pixels.
     * @param h The height of the image in pixels.
     * @param format The pixel format to use for the image.
     * @param copy Whether to copy the pixel data or not.
     * @param should_be_free Whether to free the pixel data upon image destruction.
     */
    Image(void* pixels, int w, int h, PixelFormat format, bool copy, bool should_be_free);

    /**
     * @brief Destroys the image and frees any allocated resources.
     */
    ~Image();

    // Deleted copy constructor and copy assignment operator to prevent copying.
    Image(const Image&) = delete;
    Image& operator=(const Image&) = delete;

    /**
     * @brief Move constructor.
     *
     * Transfers the ownership and data from another image to this one, leaving the other
     * image in a valid but unspecified state.
     */
    Image(Image&& other) noexcept;

    /**
     * @brief Move assignment operator.
     *
     * Transfers the ownership and data from another image to this one, leaving the other
     * image in a valid but unspecified state.
     */
    Image& operator=(Image&& other) noexcept;

    /**
     * @brief Gets the color of a pixel at a specific offset (unsafe).
     *
     * This function allows access to a pixel color at a specific offset in memory without
     * bounds checking. It is the caller's responsibility to ensure the offset is valid.
     *
     * @param offset The offset in bytes to the pixel data.
     * @return The color of the pixel at the specified offset.
     */
    Color get_unsafe(size_t offset) const;

    /**
     * @brief Gets the color of a pixel at specific coordinates (unsafe).
     *
     * This function allows access to a pixel color at the specified (x, y) coordinates
     * without bounds checking. It is the caller's responsibility to ensure the coordinates are valid.
     *
     * @param x The x-coordinate of the pixel.
     * @param y The y-coordinate of the pixel.
     * @return The color of the pixel at the specified coordinates.
     */
    Color get_unsafe(int x, int y) const {
        return get_unsafe(y * m_w + x);
    }

    /**
     * @brief Sets the color of a pixel at a specific offset (unsafe).
     *
     * This function allows setting the color of a pixel at a specific offset in memory
     * without bounds checking. It is the caller's responsibility to ensure the offset is valid.
     *
     * @param offset The offset in bytes to the pixel data.
     * @param color The color to set the pixel to.
     * @return A reference to the current `Image` object.
     */
    Image& set_unsafe(size_t offset, Color color);

    /**
     * @brief Sets the color of a pixel at specific coordinates (unsafe).
     *
     * This function allows setting the color of a pixel at the specified (x, y) coordinates
     * without bounds checking. It is the caller's responsibility to ensure the coordinates are valid.
     *
     * @param x The x-coordinate of the pixel.
     * @param y The y-coordinate of the pixel.
     * @param color The color to set the pixel to.
     * @return A reference to the current `Image` object.
     */
    Image& set_unsafe(int x, int y, Color color) {
        return set_unsafe(y * m_w + x, color);
    }

    /**
     * @brief Gets the color of a pixel at a specific offset.
     *
     * This function retrieves the color of a pixel at the specified offset in memory.
     * It ensures that the offset is within valid bounds.
     *
     * @param offset The offset in bytes to the pixel data.
     * @return The color of the pixel at the specified offset.
     */
    Color get(size_t offset) const {
        if (offset < size()) {
            return get_unsafe(offset);
        }
        return {};
    }

    /**
     * @brief Gets the color of a pixel at specific coordinates.
     *
     * This function retrieves the color of a pixel at the specified (x, y) coordinates.
     * It ensures that the coordinates are within valid bounds.
     *
     * @param x The x-coordinate of the pixel.
     * @param y The y-coordinate of the pixel.
     * @return The color of the pixel at the specified coordinates.
     */
    Color get(int x, int y) const {
        return get(y * m_w + x);
    }

    /**
     * @brief Sets the color of a pixel at a specific offset.
     *
     * This function sets the color of a pixel at the specified offset in memory.
     * It ensures that the offset is within valid bounds.
     *
     * @param offset The offset in bytes to the pixel data.
     * @param color The color to set the pixel to.
     * @return A reference to the current `Image` object.
     */
    Image& set(size_t offset, Color color) {
        if (offset < size()) {
            set_unsafe(offset, color);
        }
        return *this;
    }

    /**
     * @brief Sets the color of a pixel at specific coordinates.
     *
     * This function sets the color of a pixel at the specified (x, y) coordinates.
     * It ensures that the coordinates are within valid bounds.
     *
     * @param x The x-coordinate of the pixel.
     * @param y The y-coordinate of the pixel.
     * @param color The color to set the pixel to.
     * @return A reference to the current `Image` object.
     */
    Image& set(int x, int y, Color color) {
        return set(y * m_w + x, color);
    }

    /**
     * @brief Gets the width of the image.
     *
     * @return The width of the image in pixels.
     */
    int width() const { return m_w; }

    /**
     * @brief Gets the height of the image.
     *
     * @return The height of the image in pixels.
     */
    int height() const { return m_h; }

    /**
     * @brief Gets the dimensions (width and height) of the image.
     *
     * @return A tuple containing the width and height of the image.
     */
    std::tuple<int, int> dimensions() const {
        return { m_w, m_h };
    }

    /**
     * @brief Gets the total number of pixels in the image.
     *
     * @return The total number of pixels (width * height).
     */
    size_t size() const { return m_w * m_h; }

    /**
     * @brief Gets the pitch (stride) of the image.
     *
     * The pitch is the number of bytes between the start of one row of pixels and the start
     * of the next row, and may be larger than the width of the image in bytes due to padding.
     *
     * @return The pitch of the image in bytes.
     */
    size_t pitch() const { return m_w * pixel_size(m_format); }

    /**
     * @brief Gets the size of the image's data (in bytes).
     *
     * @return The size of the image's pixel data in bytes.
     */
    size_t data_size() const { return m_h * pitch(); }

    /**
     * @brief Gets the pixel format of the image.
     *
     * @return The pixel format used by the image.
     */
    PixelFormat format() const { return m_format; }

    /**
     * @brief Gets the raw pixel data (const version).
     *
     * @return A pointer to the raw pixel data (const version).
     */
    const void* data() const { return m_pixels; }

    /**
     * @brief Gets the raw pixel data (non-const version).
     *
     * @return A pointer to the raw pixel data (non-const version).
     */
    void* data() { return m_pixels; }
    
private:
    void* m_pixels;         ///< Pointer to the pixel data.
    PixelFormat m_format;   ///< The pixel format of the image.
    int m_w, m_h;           ///< Width and height of the image.
    bool m_owned;           ///< Flag indicating if the image owns the pixel data.
};

} // namespace bpx

#endif // BPX_IMAGE_HPP