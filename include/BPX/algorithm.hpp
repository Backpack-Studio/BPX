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

#ifndef BPX_ALGORITHM_HPP
#define BPX_ALGORITHM_HPP

#include "./image.hpp"
#include "./color.hpp"
#include <cstdint>

namespace bpx {

// Forward declarations

class ColorRamp;

/**
 * @brief Applies a mapping function to each pixel in the image.
 *
 * This function iterates over every pixel in the image and applies the specified
 * mapping function, allowing transformation of pixel values based on their coordinates.
 *
 * @param image The image to modify.
 * @param mapper A function that takes the x and y coordinates along with the current
 *        pixel color, and returns the new color to apply to that pixel.
 * @return A reference to the modified image.
 */
void map(Image& image, const Image::Mapper& mapper);

/**
 * @brief Applies a mapping function to a specified rectangular region in the image.
 *
 * This function allows selective transformation of pixels in a defined region within 
 * the image by applying the specified mapping function to each pixel in that region.
 *
 * @param image The image to modify.
 * @param x_start The x-coordinate of the top-left corner of the region to modify.
 * @param y_start The y-coordinate of the top-left corner of the region to modify.
 * @param width The width of the region to modify.
 * @param height The height of the region to modify.
 * @param mapper A function that takes the x and y coordinates along with the current
 *        pixel color, and returns the new color to apply to that pixel.
 * @return A reference to the modified image.
 */
void map(Image& image, int x_start, int y_start, int width, int height, const Image::Mapper& mapper);

/**
 * @brief Fills the entire image with a specified color.
 *
 * This function sets every pixel in the image to the specified color, effectively 
 * creating a uniform image of that color.
 *
 * @param image The image to fill.
 * @param color The color to apply to every pixel in the image.
 * @return A reference to the modified image.
 */
void fill(Image& image, Color color);

/**
 * @brief Draws a single point on the image at the specified coordinates.
 *
 * This function sets the color of a single pixel at `(x, y)` in the image, using
 * the specified blend mode to combine the new color with the existing color.
 *
 * @param image The image to modify.
 * @param x The x-coordinate of the point to draw.
 * @param y The y-coordinate of the point to draw.
 * @param color The color to set at the specified point.
 * @param mode The blending mode to use when applying the color. Defaults to `BlendMode::REPLACE`.
 * @return A reference to the modified image.
 */
void point(Image& image, int x, int y, Color color, BlendMode mode = BlendMode::REPLACE);

/**
 * @brief Draws a straight line between two points on the image using a specified color.
 *
 * This function uses a line-drawing algorithm (e.g., Bresenham's) to draw a line
 * from `(x1, y1)` to `(x2, y2)` with the specified color, applying the specified blend mode.
 *
 * @param image The image to modify.
 * @param x1 The x-coordinate of the starting point of the line.
 * @param y1 The y-coordinate of the starting point of the line.
 * @param x2 The x-coordinate of the ending point of the line.
 * @param y2 The y-coordinate of the ending point of the line.
 * @param color The color to use for the line.
 * @param mode The blending mode to use when applying the color. Defaults to `BlendMode::REPLACE`.
 * @return A reference to the modified image.
 */
void line(Image& image, int x1, int y1, int x2, int y2, Color color, BlendMode mode = BlendMode::REPLACE);

/**
 * @brief Draws a straight line between two points on the image, applying a mapping function to each pixel.
 *
 * This function draws a line from `(x1, y1)` to `(x2, y2)` and applies a custom mapping function
 * to each pixel along the line, allowing per-pixel color transformations based on position.
 *
 * @param image The image to modify.
 * @param x1 The x-coordinate of the starting point of the line.
 * @param y1 The y-coordinate of the starting point of the line.
 * @param x2 The x-coordinate of the ending point of the line.
 * @param y2 The y-coordinate of the ending point of the line.
 * @param mapper A function that takes the x and y coordinates and returns the color to apply at that point.
 * @return A reference to the modified image.
 */
void line(Image& image, int x1, int y1, int x2, int y2, const Image::Mapper& mapper);

/**
 * @brief Draws a thick line between two points on the image using a specified color.
 *
 * This function draws a line from `(x1, y1)` to `(x2, y2)` with the specified thickness
 * and color, applying the specified blend mode to each pixel in the line.
 *
 * @param image The image to modify.
 * @param x1 The x-coordinate of the starting point of the line.
 * @param y1 The y-coordinate of the starting point of the line.
 * @param x2 The x-coordinate of the ending point of the line.
 * @param y2 The y-coordinate of the ending point of the line.
 * @param thick The thickness of the line in pixels.
 * @param color The color to use for the line.
 * @param mode The blending mode to use when applying the color. Defaults to `BlendMode::REPLACE`.
 * @return A reference to the modified image.
 */
void line(Image& image, int x1, int y1, int x2, int y2, int thick, Color color, BlendMode mode = BlendMode::REPLACE);

/**
 * @brief Draws a thick line between two points on the image, applying a mapping function to each pixel.
 *
 * This function draws a line from `(x1, y1)` to `(x2, y2)` with the specified thickness,
 * and applies a custom mapping function to determine the color of each pixel along the line.
 * This allows for dynamic color effects based on the pixel position.
 *
 * @param image The image to modify.
 * @param x1 The x-coordinate of the starting point of the line.
 * @param y1 The y-coordinate of the starting point of the line.
 * @param x2 The x-coordinate of the ending point of the line.
 * @param y2 The y-coordinate of the ending point of the line.
 * @param thick The thickness of the line in pixels.
 * @param mapper A function that takes the x and y coordinates and returns the color to apply at that point.
 * @return A reference to the modified image.
 */
void line(Image& image, int x1, int y1, int x2, int y2, int thick, const Image::Mapper& mapper);

/**
 * @brief Draws a gradient line between two points on the image, transitioning between colors from a color ramp.
 *
 * This function draws a line from `(x1, y1)` to `(x2, y2)`, with the color gradually transitioning 
 * along the line according to the `ColorRamp` provided. The ramp defines multiple color points, and
 * the color will interpolate between the corresponding points based on the position along the line. 
 * The specified blend mode controls how the gradient interacts with existing colors in the image.
 *
 * @param image The image to modify.
 * @param x1 The x-coordinate of the starting point of the line.
 * @param y1 The y-coordinate of the starting point of the line.
 * @param x2 The x-coordinate of the ending point of the line.
 * @param y2 The y-coordinate of the ending point of the line.
 * @param ramp The `ColorRamp` that defines the color transition along the line.
 * @param mode The blending mode to use when applying the gradient. Defaults to `BlendMode::REPLACE`.
 * @return A reference to the modified image.
 */
void line_gradient(Image& image, int x1, int y1, int x2, int y2,
                   const ColorRamp& ramp, BlendMode mode = BlendMode::REPLACE);

/**
 * @brief Draws a thick gradient line between two points on the image, transitioning between colors from a color ramp.
 *
 * This function draws a line from `(x1, y1)` to `(x2, y2)` with the specified thickness, where 
 * the color gradually transitions along the line according to the `ColorRamp` provided. The ramp 
 * defines multiple color points, and the color will interpolate between the corresponding points 
 * based on the position along the line. The specified blend mode controls how the gradient interacts 
 * with existing colors in the image.
 *
 * @param image The image to modify.
 * @param x1 The x-coordinate of the starting point of the line.
 * @param y1 The y-coordinate of the starting point of the line.
 * @param x2 The x-coordinate of the ending point of the line.
 * @param y2 The y-coordinate of the ending point of the line.
 * @param thick The thickness of the line in pixels.
 * @param ramp The `ColorRamp` that defines the color transition along the line.
 * @param mode The blending mode to use when applying the gradient. Defaults to `BlendMode::REPLACE`.
 * @return A reference to the modified image.
 */
void line_gradient(Image& image, int x1, int y1, int x2, int y2, int thick,
                   const ColorRamp& ramp, BlendMode mode = BlendMode::REPLACE);

/**
 * @brief Draws a solid rectangle on the image with a specified color.
 *
 * This function fills a rectangular area defined by the top-left corner at `(x, y)` 
 * and the width `w` and height `h` with a specified color. The color is applied 
 * using the given blending mode.
 *
 * @param image The image to modify.
 * @param x The x-coordinate of the top-left corner of the rectangle.
 * @param y The y-coordinate of the top-left corner of the rectangle.
 * @param w The width of the rectangle.
 * @param h The height of the rectangle.
 * @param color The color to fill the rectangle with.
 * @param mode The blending mode to use when applying the color. Defaults to `BlendMode::REPLACE`.
 * @return A reference to the modified image.
 */
void rectangle(Image& image, int x, int y, int w, int h, Color color, BlendMode mode = BlendMode::REPLACE);

/**
 * @brief Draws a rectangle on the image using a mapping function to define each pixel's color.
 *
 * This function fills a rectangular area defined by the top-left corner at `(x, y)` 
 * and the width `w` and height `h`, applying a custom color mapping function to determine 
 * the color of each pixel. This allows for complex color effects within the rectangle.
 *
 * @param image The image to modify.
 * @param x The x-coordinate of the top-left corner of the rectangle.
 * @param y The y-coordinate of the top-left corner of the rectangle.
 * @param w The width of the rectangle.
 * @param h The height of the rectangle.
 * @param mapper A function that takes the x and y coordinates and returns the color to apply at that point.
 * @return A reference to the modified image.
 */
void rectangle(Image& image, int x, int y, int w, int h, const Image::Mapper& mapper);

/**
 * @brief Draws a gradient-filled rectangle on the image with specified gradient from a color ramp.
 *
 * This function fills a rectangular area defined by the top-left corner at `(x, y)` 
 * and the width `w` and height `h`, creating a linear gradient that interpolates between
 * colors defined in the provided `ColorRamp`. The gradient is applied along the specified direction
 * from `(x_start, y_start)` to `(x_end, y_end)` based on the positions within the color ramp.
 *
 * @param image The image to modify.
 * @param x The x-coordinate of the top-left corner of the rectangle.
 * @param y The y-coordinate of the top-left corner of the rectangle.
 * @param w The width of the rectangle.
 * @param h The height of the rectangle.
 * @param x_start The x-coordinate of the starting point of the gradient.
 * @param y_start The y-coordinate of the starting point of the gradient.
 * @param x_end The x-coordinate of the ending point of the gradient.
 * @param y_end The y-coordinate of the ending point of the gradient.
 * @param ramp The `ColorRamp` that defines the color transitions along the gradient.
 * @param mode The blending mode to use when applying the gradient. Defaults to `BlendMode::REPLACE`.
 * @return A reference to the modified image.
 */
void rectangle_gradient_linear(Image& image, int x, int y, int w, int h,
                               int x_start, int y_start, int x_end, int y_end,
                               const ColorRamp& ramp, BlendMode mode = BlendMode::REPLACE);

/**
 * @brief Draws a gradient-filled rectangle on the image with a radial gradient from a color ramp.
 *
 * This function fills a rectangular area defined by the top-left corner at `(x, y)` 
 * and the width `w` and height `h`, creating a radial gradient that interpolates between
 * colors defined in the provided `ColorRamp`. The gradient is applied radially, with the center
 * of the gradient starting at `(x_start, y_start)` and the outer edge at `(x_end, y_end)`.
 *
 * @param image The image to modify.
 * @param x The x-coordinate of the top-left corner of the rectangle.
 * @param y The y-coordinate of the top-left corner of the rectangle.
 * @param w The width of the rectangle.
 * @param h The height of the rectangle.
 * @param x_start The x-coordinate of the center of the radial gradient.
 * @param y_start The y-coordinate of the center of the radial gradient.
 * @param x_end The x-coordinate of the outer edge of the radial gradient.
 * @param y_end The y-coordinate of the outer edge of the radial gradient.
 * @param ramp The `ColorRamp` that defines the color transitions along the radial gradient.
 * @param mode The blending mode to use when applying the gradient. Defaults to `BlendMode::REPLACE`.
 * @return A reference to the modified image.
 */
void rectangle_gradient_radial(Image& image, int x, int y, int w, int h,
                               int x_start, int y_start, int x_end, int y_end,
                               const ColorRamp& ramp, BlendMode mode = BlendMode::REPLACE);

/**
 * @brief Draws the outline of a rectangle on the image using a specified color.
 *
 * This function draws the border of a rectangle defined by the top-left corner at `(x, y)` 
 * and the width `w` and height `h`. Only the outline of the rectangle is drawn, without filling
 * the interior.
 *
 * @param image The image to modify.
 * @param x The x-coordinate of the top-left corner of the rectangle.
 * @param y The y-coordinate of the top-left corner of the rectangle.
 * @param w The width of the rectangle.
 * @param h The height of the rectangle.
 * @param color The color to use for the rectangle outline.
 * @param mode The blending mode to use when applying the outline. Defaults to `BlendMode::REPLACE`.
 * @return A reference to the modified image.
 */
void rectangle_lines(Image& image, int x, int y, int w, int h,
                     Color color, BlendMode mode = BlendMode::REPLACE);

/**
 * @brief Draws the outline of a rectangle on the image, applying a mapping function to define the color of each point.
 *
 * This function draws the border of a rectangle defined by the top-left corner at `(x, y)` 
 * and the width `w` and height `h`. A custom mapping function is used to determine the color 
 * of each pixel along the outline, allowing for color variations or patterns along the rectangle's border.
 *
 * @param image The image to modify.
 * @param x The x-coordinate of the top-left corner of the rectangle.
 * @param y The y-coordinate of the top-left corner of the rectangle.
 * @param w The width of the rectangle.
 * @param h The height of the rectangle.
 * @param mapper A function that takes the x and y coordinates and returns the color to apply at that point on the outline.
 * @return A reference to the modified image.
 */
void rectangle_lines(Image& image, int x, int y, int w, int h, const Image::Mapper& mapper);

/**
 * @brief Draws a thick outline of a rectangle on the image with a specified color.
 *
 * This function draws a thick border around a rectangle defined by the top-left corner at `(x, y)` 
 * and the width `w` and height `h`. The outline is drawn with a specified thickness and color, 
 * and uses the given blend mode.
 *
 * @param image The image to modify.
 * @param x The x-coordinate of the top-left corner of the rectangle.
 * @param y The y-coordinate of the top-left corner of the rectangle.
 * @param w The width of the rectangle.
 * @param h The height of the rectangle.
 * @param thick The thickness of the outline in pixels.
 * @param color The color to use for the rectangle outline.
 * @param mode The blending mode to use when applying the outline. Defaults to `BlendMode::REPLACE`.
 * @return A reference to the modified image.
 */
void rectangle_lines(Image& image, int x, int y, int w, int h, int thick,
                     Color color, BlendMode mode = BlendMode::REPLACE);

/**
 * @brief Draws a thick outline of a rectangle on the image using a mapping function to determine the color of each pixel.
 *
 * This function draws a thick border around a rectangle defined by the top-left corner at `(x, y)` 
 * and the width `w` and height `h`. The color of each pixel along the border is determined by a custom
 * mapping function, allowing for color variations or patterns. The outline is drawn with a specified thickness.
 *
 * @param image The image to modify.
 * @param x The x-coordinate of the top-left corner of the rectangle.
 * @param y The y-coordinate of the top-left corner of the rectangle.
 * @param w The width of the rectangle.
 * @param h The height of the rectangle.
 * @param thick The thickness of the outline in pixels.
 * @param mapper A function that takes the x and y coordinates and returns the color to apply at that point on the outline.
 * @return A reference to the modified image.
 */
void rectangle_lines(Image& image, int x, int y, int w, int h, int thick, const Image::Mapper& mapper);

/**
 * @brief Draws a filled circle on the image using a specified color.
 *
 * This function draws a filled circle centered at `(cx, cy)` with a given radius. The circle is 
 * filled with a specified color, and the blending mode controls how the color is applied over 
 * the existing pixels.
 *
 * @param image The image to modify.
 * @param cx The x-coordinate of the center of the circle.
 * @param cy The y-coordinate of the center of the circle.
 * @param radius The radius of the circle in pixels.
 * @param color The color to fill the circle.
 * @param mode The blending mode to use when drawing the circle. Defaults to `BlendMode::REPLACE`.
 * @return A reference to the modified image.
 */
void circle(Image& image, int cx, int cy, int radius, Color color, BlendMode mode = BlendMode::REPLACE);

/**
 * @brief Draws a filled circle on the image using a mapping function to determine the color of each pixel.
 *
 * This function draws a filled circle centered at `(cx, cy)` with a given radius. Each pixel within 
 * the circle is assigned a color according to the provided `mapper` function, allowing for variations 
 * or patterns across the circle’s fill.
 *
 * @param image The image to modify.
 * @param cx The x-coordinate of the center of the circle.
 * @param cy The y-coordinate of the center of the circle.
 * @param radius The radius of the circle in pixels.
 * @param mapper A function that takes x and y coordinates and returns the color to apply at that point in the circle.
 * @return A reference to the modified image.
 */
void circle(Image& image, int cx, int cy, int radius, const Image::Mapper& mapper);

/**
 * @brief Draws a circle with a gradient fill on the image, transitioning between two colors.
 *
 * This function draws a filled circle with a smooth color gradient, transitioning from the color
 * defined in the `ColorRamp` at the center of the circle to the color at the edge. The circle is centered 
 * at `(cx, cy)` with the specified `radius`. The color of each pixel inside the circle is interpolated 
 * based on the distance from the center, creating a smooth gradient effect.
 *
 * @param image The image to modify.
 * @param cx The x-coordinate of the center of the circle.
 * @param cy The y-coordinate of the center of the circle.
 * @param radius The radius of the circle in pixels.
 * @param ramp The `ColorRamp` that defines the color gradient from the center to the edge of the circle.
 * @param mode The blending mode to use when applying the gradient. Defaults to `BlendMode::REPLACE`.
 * @return A reference to the modified image.
 */
void circle_gradient(Image& image, int cx, int cy, int radius,
                     const ColorRamp& ramp, BlendMode mode = BlendMode::REPLACE);

/**
 * @brief Draws a circle outline on the image with a specified color.
 *
 * This function draws a circle with a single pixel-wide outline around the area defined by the center `(cx, cy)` 
 * and the given `radius`. The outline is drawn using a specified color, and the blending mode is applied.
 *
 * @param image The image to modify.
 * @param cx The x-coordinate of the center of the circle.
 * @param cy The y-coordinate of the center of the circle.
 * @param radius The radius of the circle in pixels.
 * @param color The color to use for the circle outline.
 * @param mode The blending mode to use when drawing the circle outline. Defaults to `BlendMode::REPLACE`.
 * @return A reference to the modified image.
 */
void circle_lines(Image& image, int cx, int cy, int radius, Color color,
                  BlendMode mode = BlendMode::REPLACE);

/**
 * @brief Draws a circle outline on the image with a mapping function to determine the color of each pixel.
 *
 * This function draws a circle outline using a custom color mapping function, allowing each pixel along 
 * the edge of the circle to have a different color. The circle is centered at `(cx, cy)` with the given `radius`.
 *
 * @param image The image to modify.
 * @param cx The x-coordinate of the center of the circle.
 * @param cy The y-coordinate of the center of the circle.
 * @param radius The radius of the circle in pixels.
 * @param mapper A function that takes x and y coordinates and returns the color to apply at each point along the outline.
 * @return A reference to the modified image.
 */
void circle_lines(Image& image, int cx, int cy, int radius, const Image::Mapper& mapper);

/**
 * @brief Draws a thick circle outline on the image with a specified color.
 *
 * This function draws a circle with a specified thickness around the area defined by the center `(cx, cy)` 
 * and the given `radius`. The circle is outlined with the specified `color`, and the blending mode is applied.
 * The circle's outline is thicker than a standard single-pixel line, and the thickness is specified by the `thick` parameter.
 *
 * @param image The image to modify.
 * @param cx The x-coordinate of the center of the circle.
 * @param cy The y-coordinate of the center of the circle.
 * @param radius The radius of the circle in pixels.
 * @param thick The thickness of the circle's outline in pixels.
 * @param color The color of the circle's outline.
 * @param mode The blending mode to use when drawing the circle outline. Defaults to `BlendMode::REPLACE`.
 * @return A reference to the modified image.
 */
void circle_lines(Image& image, int cx, int cy, int radius, int thick,
                  Color color, BlendMode mode = BlendMode::REPLACE);

/**
 * @brief Draws a thick circle outline on the image using a mapping function for color.
 *
 * This function draws a circle outline with a specified thickness, where the color of each pixel along the circle's outline 
 * is determined by the provided `mapper` function. The circle is centered at `(cx, cy)` with the specified `radius`, and 
 * the thickness of the outline is controlled by the `thick` parameter.
 *
 * @param image The image to modify.
 * @param cx The x-coordinate of the center of the circle.
 * @param cy The y-coordinate of the center of the circle.
 * @param radius The radius of the circle in pixels.
 * @param thick The thickness of the circle's outline in pixels.
 * @param mapper A function that takes x and y coordinates and returns the color to apply at each point along the outline.
 * @return A reference to the modified image.
 */
void circle_lines(Image& image, int cx, int cy, int radius, int thick, const Image::Mapper& mapper);

/**
 * @brief Draws a portion of one image onto another image using a specified blend mode.
 *
 * This function copies a rectangular portion of the source image `src` defined by `(x, y)` as the top-left corner 
 * and `(x + w, y + h)` as the bottom-right corner, and draws it onto the destination image `dst`. The image portion 
 * is drawn using the specified blending mode, which determines how the source image is combined with the destination image.
 *
 * @param dst The destination image to modify.
 * @param x The x-coordinate of the top-left corner of the portion to be copied from the source image.
 * @param y The y-coordinate of the top-left corner of the portion to be copied from the source image.
 * @param w The width of the portion to be copied from the source image.
 * @param h The height of the portion to be copied from the source image.
 * @param src The source image from which the portion is copied.
 * @param mode The blending mode to use when applying the source image to the destination image.
 * @return A reference to the modified destination image.
 */
void draw(Image& dst, int x, int y, int w, int h, Image& src, BlendMode mode = BlendMode::REPLACE);

/**
 * @brief Draws a section of a source image onto a destination image with optional blending.
 *
 * This function draws a portion of the `src` image onto the `dst` image, where the section of the source image 
 * to be drawn is defined by `(x_src, y_src)` as the top-left corner and `(w_src, h_src)` as the width and height.
 * The destination position and size are defined by `(x_dst, y_dst)` for the top-left corner and `(w_dst, h_dst)` for the width and height.
 * The drawing process uses the specified `BlendMode` to combine the source and destination colors.
 *
 * @param dst The destination image to modify.
 * @param x_dst The x-coordinate of the top-left corner of the destination area.
 * @param y_dst The y-coordinate of the top-left corner of the destination area.
 * @param w_dst The width of the area to draw in the destination image.
 * @param h_dst The height of the area to draw in the destination image.
 * @param src The source image to take the portion from.
 * @param x_src The x-coordinate of the top-left corner of the source area.
 * @param y_src The y-coordinate of the top-left corner of the source area.
 * @param w_src The width of the area to copy from the source image.
 * @param h_src The height of the area to copy from the source image.
 * @param mode The blending mode to use when drawing the image section. Defaults to `BlendMode::REPLACE`.
 * @return A reference to the modified destination image.
 */
void draw(Image& dst, int x_dst, int y_dst, int w_dst, int h_dst,
          Image& src, int x_src, int y_src, int w_src, int h_src,
          BlendMode mode = BlendMode::REPLACE);

/**
 * @brief Adjusts the saturation of the image.
 *
 * This function modifies the saturation of all pixels in the image by multiplying the color components 
 * (RGB) of each pixel by the specified `factor`. A factor greater than 1 increases saturation, while 
 * a factor less than 1 reduces it. A factor of 1 leaves the saturation unchanged.
 *
 * @param image The image to modify.
 * @param factor The saturation factor. Values greater than 1 increase saturation, values between 0 and 1 decrease it.
 * @return A reference to the modified image.
 */
void saturation(Image& image, float factor);

/**
 * @brief Adjusts the brightness of the image.
 *
 * This function modifies the brightness of all pixels in the image by adding the specified `factor` to 
 * the color components (RGB) of each pixel. Positive values of `factor` increase brightness, while negative 
 * values decrease it. A factor of 0 leaves the brightness unchanged.
 *
 * @param image The image to modify.
 * @param factor The brightness factor. Positive values increase brightness, negative values decrease it.
 * @return A reference to the modified image.
 */
void brightness(Image& image, float factor);

/**
 * @brief Adjusts the contrast of the image.
 *
 * This function modifies the contrast of all pixels in the image. The contrast adjustment works by scaling 
 * the difference between each pixel's color value and the average color intensity of the image. A factor 
 * greater than 1 increases contrast, while values between 0 and 1 reduce it. A factor of 1 leaves the contrast unchanged.
 *
 * @param image The image to modify.
 * @param factor The contrast factor. Values greater than 1 increase contrast, values between 0 and 1 reduce it.
 * @return A reference to the modified image.
 */
void contrast(Image& image, float factor);

/**
 * @brief Adjusts the saturation of the image.
 *
 * This function modifies the saturation of all pixels in the image by multiplying the color components 
 * (RGB) of each pixel by the specified `factor`. A factor greater than 1 increases saturation, while 
 * a factor less than 1 reduces it. A factor of 1 leaves the saturation unchanged.
 *
 * @param image The image to modify.
 * @param factor The saturation factor. Values greater than 1 increase saturation, values between 0 and 1 decrease it.
 * @return A reference to the modified image.
 */
void saturation(Image& image, float factor);

/**
 * @brief Adjusts the brightness of the image.
 *
 * This function modifies the brightness of all pixels in the image by adding the specified `factor` to 
 * the color components (RGB) of each pixel. Positive values of `factor` increase brightness, while negative 
 * values decrease it. A factor of 0 leaves the brightness unchanged.
 *
 * @param image The image to modify.
 * @param factor The brightness factor. Positive values increase brightness, negative values decrease it.
 * @return A reference to the modified image.
 */
void brightness(Image& image, float factor);

/**
 * @brief Adjusts the contrast of the image.
 *
 * This function modifies the contrast of all pixels in the image. The contrast adjustment works by scaling 
 * the difference between each pixel's color value and the average color intensity of the image. A factor 
 * greater than 1 increases contrast, while values between 0 and 1 reduce it. A factor of 1 leaves the contrast unchanged.
 *
 * @param image The image to modify.
 * @param factor The contrast factor. Values greater than 1 increase contrast, values between 0 and 1 reduce it.
 * @return A reference to the modified image.
 */
void contrast(Image& image, float factor);

/**
 * @brief Flips the image horizontally.
 *
 * This function flips the image along its vertical axis (left to right). Each pixel is moved from its 
 * original position to a mirrored position on the opposite side of the image. This operation swaps 
 * the columns of the image while keeping the row positions intact.
 *
 * @param image The image to modify.
 * @return A reference to the modified image with horizontal flip.
 */
void flip_horizontal(Image& image);

/**
 * @brief Flips the image vertically.
 *
 * This function flips the image along its horizontal axis (top to bottom). Each pixel is moved from its 
 * original position to a mirrored position on the opposite side of the image. This operation swaps 
 * the rows of the image while keeping the column positions intact.
 *
 * @param image The image to modify.
 * @return A reference to the modified image with vertical flip.
 */
void flip_vertical(Image& image);

/**
 * @brief Rotates the image by 90 degrees clockwise.
 *
 * This function rotates the image by 90 degrees clockwise. If the image is rectangular, a new memory 
 * allocation is performed to accommodate the new dimensions. For square images, the function performs 
 * the rotation in-place without additional memory allocation.
 *
 * The pixel data is rearranged in such a way that the first column of the original image becomes the first 
 * row of the rotated image, and so on.
 *
 * @param image The image to rotate.
 * @return A reference to the rotated image.
 */
void rotate_90(Image& image);

/**
 * @brief Rotates the image by 180 degrees.
 *
 * This function rotates the image by 180 degrees, effectively flipping the image both horizontally 
 * and vertically. The pixel data is rearranged so that the last pixel in the original image moves 
 * to the first position, and all other pixels are similarly relocated.
 *
 * @param image The image to rotate.
 * @return A reference to the rotated image.
 */
void rotate_180(Image& image);

/**
 * @brief Creates a copy of the given image.
 *
 * This function creates a deep copy of the input image, including all pixel data and attributes 
 * such as format and dimensions. The copied image will have identical content and properties to 
 * the original image but will reside in a different memory location.
 *
 * @param image The image to copy.
 * @return A new image that is a copy of the original.
 */
Image copy(const Image& image);

/**
 * @brief Converts the image to a new pixel format.
 *
 * This function converts the image from its current pixel format to the specified `new_format`. 
 * It processes the pixel data and reassigns each pixel's color information according to the new format. 
 * The pixel format may change the way pixel colors are stored (e.g., from RGB to RGBA, or from 8-bit to 16-bit).
 *
 * @param image The image to convert.
 * @param new_format The target pixel format for the conversion.
 * @return A new image with the specified pixel format.
 */
Image convert(const Image& image, PixelFormat new_format);

/**
 * @brief Resizes the canvas of the image without altering its content.
 *
 * This function resizes the canvas of the image to the specified width (`new_w`) and height (`new_h`), 
 * preserving the original content. The image's content is centered in the new canvas, and the empty space 
 * is filled with a blank color (such as transparent or black). The original image's pixels are not resized 
 * or stretched, they are just repositioned within the new canvas size.
 *
 * @param image The image to resize.
 * @param new_w The new width of the canvas.
 * @param new_h The new height of the canvas.
 * @return A new image with the resized canvas.
 */
Image resize_canvas(const Image& image, int new_w, int new_h);

/**
 * @brief Resizes the image to the specified dimensions.
 *
 * This function resizes the entire image to the new specified width (`new_w`) and height (`new_h`).
 * The image content will be scaled to fit the new dimensions, which may result in distortion if the 
 * aspect ratio is not preserved. The scaling algorithm used can vary (e.g., nearest neighbor, bilinear, etc.).
 *
 * @param image The image to resize.
 * @param new_w The new width of the image.
 * @param new_h The new height of the image.
 * @return A new image with the resized content.
 */
Image resize(const Image& image, int new_w, int new_h);

/**
 * @brief Writes the image to a PNG file.
 *
 * This function saves the provided image to a file in PNG format. The PNG format supports lossless 
 * compression and is widely used for high-quality image storage.
 *
 * @param image The image to write to a file.
 * @param path The file path where the PNG image will be saved.
 * @return `true` if the image was successfully saved, `false` otherwise.
 */
bool write_png(const Image& image, const std::string& path);

/**
 * @brief Writes the image to a BMP file.
 *
 * This function saves the provided image to a file in BMP format. BMP is a simple image format that 
 * typically uses uncompressed pixel data and is compatible with a wide range of image editing software.
 *
 * @param image The image to write to a file.
 * @param path The file path where the BMP image will be saved.
 * @return `true` if the image was successfully saved, `false` otherwise.
 */
bool write_bmp(const Image& image, const std::string& path);

/**
 * @brief Writes the image to a TGA file.
 *
 * This function saves the provided image to a file in TGA (Targa) format. TGA supports both 
 * uncompressed and compressed pixel data, and is commonly used in computer graphics and video games.
 *
 * @param image The image to write to a file.
 * @param path The file path where the TGA image will be saved.
 * @return `true` if the image was successfully saved, `false` otherwise.
 */
bool write_tga(const Image& image, const std::string& path);

/**
 * @brief Writes the image to a JPG file with specified quality.
 *
 * This function saves the provided image to a JPG file format, with an optional quality parameter 
 * controlling the compression level. JPG is a lossy compression format commonly used for photographs 
 * and web images. The quality parameter ranges from 0 (lowest quality, highest compression) to 100 
 * (highest quality, lowest compression).
 *
 * @param image The image to write to a file.
 * @param path The file path where the JPG image will be saved.
 * @param quality The quality level of the JPG image (default is 90).
 * @return `true` if the image was successfully saved, `false` otherwise.
 */
bool write_jpg(const Image& image, const std::string& path, int quality = 90);

/**
 * @brief Adjusts the saturation of a color.
 *
 * This function adjusts the saturation of the input color by the specified factor. A factor of 1.0 
 * means no change, values greater than 1.0 increase the saturation, and values between 0.0 and 1.0 
 * decrease the saturation. The function works by converting the color to the HSV color space and 
 * modifying the saturation component.
 *
 * @param color The color to adjust the saturation for.
 * @param factor The factor to adjust the saturation by. Values greater than 1.0 increase the saturation, 
 *               while values between 0.0 and 1.0 decrease it.
 * @return A new color with the adjusted saturation.
 */
constexpr Color saturation(Color color, float factor) {
    auto [ h, s, v ] = color.to_hsv();
    return Color::from_hsv(h, factor, v);
}

/**
 * @brief Adjusts the brightness of a color.
 *
 * This function adjusts the brightness of the input color by the specified factor. A factor of 0.0 
 * results in black, and a factor of 1.0 leaves the color unchanged. Values above 1.0 will increase 
 * the brightness, while values between 0.0 and 1.0 will decrease it. The function works by scaling 
 * the RGB components of the color accordingly.
 *
 * @param color The color to adjust the brightness for.
 * @param factor The factor to adjust the brightness by. Values between 0.0 and 1.0 decrease the brightness, 
 *               values above 1.0 increase it, and negative values darken the color.
 * @return A new color with the adjusted brightness.
 */
constexpr Color brightness(Color color, float factor) {
    factor = std::clamp(factor, -1.0f, 1.0f);
    float rf = color.r;
    float gf = color.g;
    float bf = color.b;
    if (factor < 0.0f) {
        factor = 1.0f + factor;
        rf *= factor;
        gf *= factor;
        bf *= factor;
    } else {
        rf = (255 - rf) * factor + rf;
        gf = (255 - gf) * factor + gf;
        bf = (255 - bf) * factor + bf;
    }
    return {
        static_cast<uint8_t>(rf),
        static_cast<uint8_t>(gf),
        static_cast<uint8_t>(bf),
        color.a
    };
}

/**
 * @brief Adjusts the contrast of a color.
 *
 * This function adjusts the contrast of the input color by the specified factor. The contrast adjustment 
 * is done by modifying the RGB components of the color. A factor of `0.0` results in a gray color, 
 * while a factor of `1.0` leaves the color unchanged. Negative values will reduce the contrast, 
 * while positive values will increase it. The contrast is applied by mapping the color values to a 
 * scale between 0.0 and 1.0, adjusting them by the factor, and then mapping them back to the 0-255 range.
 *
 * @param color The color to adjust the contrast for.
 * @param factor The factor to adjust the contrast by. Values between -1.0 and 1.0 are clamped.
 *               A factor of 0.0 results in a gray color, and a factor of 1.0 leaves the color unchanged. 
 *               Positive values increase the contrast, and negative values decrease it.
 * @return A new color with the adjusted contrast.
 */
constexpr Color contrast(Color color, float factor) {
    factor = std::clamp(factor, -1.0f, 1.0f);
    factor = (1.0f + factor);
    factor *= factor;

    float rf = color.r / 255.0f;
    rf -= 0.5f; rf *= factor; rf += 0.5f;
    rf = std::clamp(rf, 0.0f, 1.0f);

    float gf = color.g / 255.0f;
    gf -= 0.5f; gf *= factor; gf += 0.5f;
    gf = std::clamp(gf, 0.0f, 1.0f);

    float bf = color.b / 255.0f;
    bf -= 0.5f; bf *= factor; bf += 0.5f;
    bf = std::clamp(bf, 0.0f, 1.0f);

    return {
        static_cast<uint8_t>(rf * 255),
        static_cast<uint8_t>(bf * 255),
        static_cast<uint8_t>(gf * 255),
        color.a
    };
}

/**
 * @brief Inverts the colors of the given color.
 *
 * This function inverts the RGB components of the input color. The inversion is done by subtracting 
 * each color component (Red, Green, and Blue) from 255. The alpha component is left unchanged.
 * This results in the complementary color for each channel (i.e., red becomes cyan, green becomes magenta, 
 * and blue becomes yellow). The function returns a new color with the inverted RGB values and the same alpha.
 *
 * @param color The color to invert.
 * @return A new color with inverted RGB values while maintaining the same alpha.
 */
constexpr Color invert(Color color) {
    return {
        static_cast<uint8_t>(255 - color.r),
        static_cast<uint8_t>(255 - color.g),
        static_cast<uint8_t>(255 - color.b),
        color.a
    };
}

/**
 * @brief Adjusts the alpha (opacity) of the given color.
 *
 * This function adjusts the alpha (opacity) of the input color by scaling it with a given factor.
 * The factor is expected to be between `0.0` (completely transparent) and `1.0` (completely opaque).
 * The RGB components are unchanged, and only the alpha value is adjusted.
 *
 * @param color The color whose alpha will be adjusted.
 * @param alpha The new alpha value, represented as a floating point between 0.0 and 1.0.
 *              A value of 1.0 represents full opacity, while 0.0 represents full transparency.
 * @return A new color with the adjusted alpha value while keeping the same RGB components.
 */
constexpr Color alpha(Color color, float alpha) {
    return {
        color.r,
        color.g,
        color.b,
        static_cast<uint8_t>(alpha * 255)
    };
}

/**
 * @brief Converts a color to grayscale by averaging its RGB components.
 *
 * This function calculates the grayscale value by averaging the red, green, and blue components of the 
 * input color. The resulting grayscale value is applied to all three RGB components, while the alpha 
 * (opacity) remains unchanged. This is a simple method to convert a color to grayscale using the average 
 * of the color channels.
 *
 * @param color The color to be converted to grayscale.
 * @return A new color where the RGB components are equal to the average of the original color's RGB values, 
 *         and the alpha remains the same.
 */
constexpr Color grayscale(Color color) {
    uint8_t gray = static_cast<int>(color.r + color.g + color.b) / 3;
    return Color { gray, gray, gray, color.a };
}

/**
 * @brief Returns the grayscale value by averaging the RGB components of the color.
 *
 * This function calculates the grayscale value of a color by averaging its red, green, and blue components.
 * The result is a single value representing the intensity of the color in grayscale. The function does not 
 * modify the alpha (opacity) component.
 *
 * @param color The color to get the grayscale value from.
 * @return The grayscale value as an integer, which is the average of the red, green, and blue components of the color.
 */
constexpr uint8_t grayscale_value(Color color) {
    return static_cast<int>(color.r + color.g + color.b) / 3;
}

/**
 * @brief Converts a color to its luminance value and applies it to the RGB components.
 *
 * This function calculates the luminance of the input color based on the standard luminance formula
 * for RGB colors: 
 * - Luminance = 0.299 * R + 0.587 * G + 0.114 * B
 * The resulting luminance value is applied to the red, green, and blue components, while the alpha 
 * (opacity) remains unchanged. This is a more perceptually accurate method for converting a color to 
 * grayscale, as it gives more weight to the green component, which the human eye is more sensitive to.
 *
 * @param color The color to be converted to luminance.
 * @return A new color where the RGB components are set to the luminance value, and the alpha remains the same.
 */
constexpr Color luminance(Color color) {
    uint8_t lum = 0.299f * color.r + 0.587f * color.g + 0.114f * color.b;
    return Color { lum, lum, color.a };
}

/**
 * @brief Calculates the luminance value of a color.
 *
 * This function calculates the luminance of a color using the standard formula for RGB to luminance conversion:
 * Luminance = 0.299 * R + 0.587 * G + 0.114 * B.
 * This method provides a measure of the perceived brightness of the color, which is weighted based on human 
 * vision sensitivity to the red, green, and blue channels.
 *
 * @param color The color from which to calculate the luminance.
 * @return The luminance value, a single integer representing the brightness of the color.
 */
constexpr uint8_t luminance_value(Color color) {
    return 0.299f * color.r + 0.587f * color.g + 0.114f * color.b;
}

/**
 * @brief Performs linear interpolation (lerp) between two colors.
 *
 * This function calculates a color that is a linear interpolation between two input colors based on a parameter t. 
 * The value of t should be between 0 and 1, where 0 returns the first color (a), and 1 returns the second color (b).
 * The result is a color that smoothly transitions from `a` to `b` based on the interpolation factor `t`. 
 * This is useful for creating gradual color transitions or fading effects.
 *
 * @param a The first color (start color) in the interpolation.
 * @param b The second color (end color) in the interpolation.
 * @param t The interpolation factor. It is typically in the range [0, 1].
 * @return The interpolated color.
 */
constexpr Color lerp(Color a, Color b, float t) {
    return {
        static_cast<uint8_t>(a.r + t * (b.r - a.r)),
        static_cast<uint8_t>(a.g + t * (b.g - a.g)),
        static_cast<uint8_t>(a.b + t * (b.b - a.b)),
        static_cast<uint8_t>(a.a + t * (b.a - a.a))
    };
}

/**
 * @brief Blends two colors based on a specified blend mode.
 *
 * This function performs a color blending operation between a destination color (`dst`) and a source color (`src`)
 * using a specified blend mode. The result is the color that results from combining the two input colors according 
 * to the chosen blending algorithm. The `BlendMode` defines how the blending should be done, with different modes 
 * providing various visual effects such as replacing colors, adjusting transparency, or mixing colors in various ways.
 *
 * Available blend modes include:
 * - `REPLACE`: Replaces the destination color with the source color.
 * - `ALPHA`: Uses the alpha value of the source color to blend the two colors.
 * - `ADD`: Adds the color values of the source to the destination.
 * - `SUB`: Subtracts the source color values from the destination.
 * - `MUL`: Multiplies the color values of the source and destination.
 * - `SCREEN`: Inverse of multiplication for a lightening effect.
 * - `DARKEN`: Selects the darker color between the source and destination.
 * - `LIGHTEN`: Selects the lighter color between the source and destination.
 * - `DIFFERENCE`: Subtracts the source color from the destination, using absolute values.
 * - `EXCLUSION`: A variation of `DIFFERENCE` with less contrast.
 * - `DODGE`: Brightens the destination color based on the source.
 * - `BURN`: Darkens the destination color based on the source.
 *
 * @param dst The destination color to be blended.
 * @param src The source color to blend with the destination color.
 * @param mode The blend mode to use for the blending operation.
 * @return The resulting blended color.
 */
constexpr Color blend(Color dst, Color src, BlendMode mode) noexcept {
    constexpr auto clamp_ubyte = [](int value) -> uint8_t {
        return static_cast<uint8_t>(std::max(0, std::min(255, value)));
    };
    switch (mode) {
        case BlendMode::REPLACE:
            return src;

        case BlendMode::ALPHA: {
            float src_alpha = src.a / 255.0f;
            float dst_alpha = dst.a / 255.0f * (1.0f - src_alpha);
            float out_alpha = src_alpha + dst_alpha;

            return {
                clamp_ubyte(static_cast<int>((src.r * src_alpha + dst.r * dst_alpha) / out_alpha)),
                clamp_ubyte(static_cast<int>((src.g * src_alpha + dst.g * dst_alpha) / out_alpha)),
                clamp_ubyte(static_cast<int>((src.b * src_alpha + dst.b * dst_alpha) / out_alpha)),
                clamp_ubyte(static_cast<int>(out_alpha * 255))
            };
        }

        case BlendMode::ADD:
            return {
                clamp_ubyte(dst.r + src.r),
                clamp_ubyte(dst.g + src.g),
                clamp_ubyte(dst.b + src.b),
                dst.a
            };

        case BlendMode::SUB:
            return {
                clamp_ubyte(dst.r - src.r),
                clamp_ubyte(dst.g - src.g),
                clamp_ubyte(dst.b - src.b),
                dst.a
            };

        case BlendMode::MUL:
            return {
                clamp_ubyte((dst.r * src.r) / 255),
                clamp_ubyte((dst.g * src.g) / 255),
                clamp_ubyte((dst.b * src.b) / 255),
                dst.a
            };

        case BlendMode::SCREEN:
            return {
                clamp_ubyte(255 - ((255 - dst.r) * (255 - src.r) / 255)),
                clamp_ubyte(255 - ((255 - dst.g) * (255 - src.g) / 255)),
                clamp_ubyte(255 - ((255 - dst.b) * (255 - src.b) / 255)),
                dst.a
            };

        case BlendMode::DARKEN:
            return {
                std::min(dst.r, src.r),
                std::min(dst.g, src.g),
                std::min(dst.b, src.b),
                dst.a
            };

        case BlendMode::LIGHTEN:
            return {
                std::max(dst.r, src.r),
                std::max(dst.g, src.g),
                std::max(dst.b, src.b),
                dst.a
            };

        case BlendMode::DIFFERENCE:
            return {
                clamp_ubyte(std::abs(dst.r - src.r)),
                clamp_ubyte(std::abs(dst.g - src.g)),
                clamp_ubyte(std::abs(dst.b - src.b)),
                dst.a
            };

        case BlendMode::EXCLUSION:
            return {
                clamp_ubyte(dst.r + src.r - 2 * dst.r * src.r / 255),
                clamp_ubyte(dst.g + src.g - 2 * dst.g * src.g / 255),
                clamp_ubyte(dst.b + src.b - 2 * dst.b * src.b / 255),
                dst.a
            };

        case BlendMode::DODGE:
            return {
                clamp_ubyte((src.r == 255) ? 255 : std::min(255, (dst.r * 255) / (255 - src.r))),
                clamp_ubyte((src.g == 255) ? 255 : std::min(255, (dst.g * 255) / (255 - src.g))),
                clamp_ubyte((src.b == 255) ? 255 : std::min(255, (dst.b * 255) / (255 - src.b))),
                dst.a
            };

        case BlendMode::BURN:
            return {
                clamp_ubyte((src.r == 0) ? 0 : std::max(0, 255 - (255 - dst.r) * 255 / src.r)),
                clamp_ubyte((src.g == 0) ? 0 : std::max(0, 255 - (255 - dst.g) * 255 / src.g)),
                clamp_ubyte((src.b == 0) ? 0 : std::max(0, 255 - (255 - dst.b) * 255 / src.b)),
                dst.a
            };
    }

    return dst;
}

} // bpx

#endif // BPX_ALGORITHM_HPP
