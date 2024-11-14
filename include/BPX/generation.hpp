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

#ifndef BPX_GENERATION_HPP
#define BPX_GENERATION_HPP

#include "pixel.hpp"
#include "image.hpp"
#include "ramp.hpp"

namespace bpx {

/**
 * @brief Generates a 1D linear gradient image.
 * 
 * This function creates a horizontal gradient of specified width, using colors interpolated
 * from the provided `ColorRamp`. Each pixel’s color is determined by its position along the width.
 * 
 * @param width The width of the gradient image in pixels.
 * @param ramp The `ColorRamp` object defining the colors to interpolate across the gradient.
 * @param format The desired pixel format for the image (default is `PixelFormat::RGBA_U8`).
 * @return An `Image` object containing the generated gradient.
 */
Image generate_gradient_linear_1d(int width, const ColorRamp& ramp, PixelFormat format = PixelFormat::RGBA_U8);

/**
 * @brief Generates a 2D linear gradient image between two points.
 * 
 * This function creates a linear gradient over a 2D area, blending colors between a starting
 * point `(x_start, y_start)` and an ending point `(x_end, y_end)` as defined by the `ColorRamp`.
 * Colors are interpolated in a straight line from the start to the end point.
 * 
 * @param width The width of the gradient image in pixels.
 * @param height The height of the gradient image in pixels.
 * @param ramp The `ColorRamp` object defining the colors to interpolate across the gradient.
 * @param x_start The x-coordinate of the gradient's starting point.
 * @param y_start The y-coordinate of the gradient's starting point.
 * @param x_end The x-coordinate of the gradient's ending point.
 * @param y_end The y-coordinate of the gradient's ending point.
 * @param format The desired pixel format for the image (default is `PixelFormat::RGBA_U8`).
 * @return An `Image` object containing the generated 2D linear gradient.
 */
Image generate_gradient_linear_2d(int width, int height, const ColorRamp& ramp,
                                  int x_start, int y_start, int x_end, int y_end,
                                  PixelFormat format = PixelFormat::RGBA_U8);

/**
 * @brief Generates a 2D radial gradient image between two points.
 * 
 * This function creates a radial gradient that starts at `(x_start, y_start)` and extends outward
 * to `(x_end, y_end)`. Colors are interpolated radially, increasing in distance from the center 
 * point towards the outer edge according to the `ColorRamp`.
 * 
 * @param width The width of the gradient image in pixels.
 * @param height The height of the gradient image in pixels.
 * @param ramp The `ColorRamp` object defining the colors to interpolate across the gradient.
 * @param x_start The x-coordinate of the gradient's center point.
 * @param y_start The y-coordinate of the gradient's center point.
 * @param x_end The x-coordinate of the gradient's outer edge.
 * @param y_end The y-coordinate of the gradient's outer edge.
 * @param format The desired pixel format for the image (default is `PixelFormat::RGBA_U8`).
 * @return An `Image` object containing the generated 2D radial gradient.
 */
Image generate_gradient_radial_2d(int width, int height, const ColorRamp& ramp,
                                  int x_start, int y_start, int x_end, int y_end,
                                  PixelFormat format = PixelFormat::RGBA_U8);

} // namespace bpx

#endif // BPX_GENERATION_HPP
