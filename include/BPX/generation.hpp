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
Image generate_gradient_linear(int width, int height, const ColorRamp& ramp,
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
Image generate_gradient_radial(int width, int height, const ColorRamp& ramp,
                                  int x_start, int y_start, int x_end, int y_end,
                                  PixelFormat format = PixelFormat::RGBA_U8);

/**
 * @brief Generate a checkerboard pattern image.
 * 
 * This function generates a checkerboard pattern with alternating squares of 
 * two colors. The checkerboard pattern consists of squares of the specified 
 * size that alternate between the two given colors.
 *
 * @param width The width of the image in pixels.
 * @param height The height of the image in pixels.
 * @param square_w The width of each square in the checkerboard pattern.
 * @param square_h The height of each square in the checkerboard pattern.
 * @param color1 The color of the first set of squares (default is black).
 * @param color2 The color of the second set of squares (default is white).
 * @param format The pixel format for the image (default is RGBA_U8).
 * @return The generated image with the checkerboard pattern.
 */
Image generate_checkerboard(int width, int height, int square_w, int square_h,
                            const Color& color1 = BLACK, const Color& color2 = WHITE,
                            PixelFormat format = PixelFormat::RGBA_U8);

/**
 * @brief Generate a striped pattern image.
 * 
 * This function generates a striped pattern by alternating stripes of two 
 * colors. The stripes can be either vertical or horizontal, depending on 
 * the specified orientation. The width of each stripe is defined by the 
 * `stripe_width` parameter.
 *
 * @param width The width of the image in pixels.
 * @param height The height of the image in pixels.
 * @param stripe_width The width of each stripe in the pattern.
 * @param color1 The color of the first set of stripes (default is black).
 * @param color2 The color of the second set of stripes (default is white).
 * @param vertical A boolean value to specify the orientation of stripes.
 *                If true, the stripes are vertical, otherwise, they are horizontal (default is true).
 * @param format The pixel format for the image (default is RGBA_U8).
 * @return The generated image with the striped pattern.
 */
Image generate_stripes(int width, int height, int stripe_width,
                       const Color& color1 = BLACK, const Color& color2 = WHITE,
                       bool vertical = true, PixelFormat format = PixelFormat::RGBA_U8);

/**
 * @brief Generate a grid pattern image.
 * 
 * This function generates a grid pattern with alternating cells, where the 
 * grid lines are drawn at regular intervals based on the `cell_size` parameter.
 * The color of the grid lines and the fill color of the cells can be customized.
 *
 * @param width The width of the image in pixels.
 * @param height The height of the image in pixels.
 * @param cell_size The size of each cell in the grid.
 * @param line_color The color of the grid lines (default is white).
 * @param fill_color The color of the background cells (default is black).
 * @param format The pixel format for the image (default is RGBA_U8).
 * @return The generated image with the grid pattern.
 */
Image generate_grid(int width, int height, int cell_size,
                    const Color& line_color = WHITE, const Color& fill_color = BLACK,
                    PixelFormat format = PixelFormat::RGBA_U8);

/**
 * @brief Generate a polka dots pattern image.
 * 
 * This function generates a polka dots pattern where each dot is drawn in a 
 * grid layout with a fixed radius and spacing. The color of the dots and the 
 * background color can be customized.
 *
 * @param width The width of the image in pixels.
 * @param height The height of the image in pixels.
 * @param dot_radius The radius of each dot in the pattern.
 * @param spacing The spacing between the centers of adjacent dots.
 * @param dot_color The color of the dots (default is white).
 * @param background_color The background color (default is black).
 * @param format The pixel format for the image (default is RGBA_U8).
 * @return The generated image with the polka dots pattern.
 */
Image generate_polka_dots(int width, int height, int dot_radius, int spacing,
                          const Color& dot_color = WHITE, const Color& background_color = BLACK,
                          PixelFormat format = PixelFormat::RGBA_U8);

} // namespace bpx

#endif // BPX_GENERATION_HPP
