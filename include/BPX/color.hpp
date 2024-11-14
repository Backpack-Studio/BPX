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

#ifndef BPX_COLOR_HPP
#define BPX_COLOR_HPP

#include <algorithm>
#include <cstdint>
#include <tuple>
#include <cmath>

namespace bpx {

/**
 * @enum BlendMode
 * @brief Enum representing different blending modes for combining colors.
 *
 * Each blending mode specifies a different way to combine a source color 
 * with a base color in order to produce the final result.
 */
enum class BlendMode
{
    /** 
     * REPLACE: Replaces the base color with the source color.
     */
    REPLACE,

    /** 
     * ALPHA: Uses the alpha channel of the source to blend the colors.
     * The resulting color is a mix between the source and base colors,
     * weighted by the source alpha value.
     */
    ALPHA,

    /** 
     * ADD: Adds the color values of the source and base.
     * Useful for creating lighter colors and emphasizing bright areas.
     */
    ADD,

    /** 
     * SUB: Subtracts the color values of the source from the base.
     * Can be used to darken or mute the base color based on the source.
     */
    SUB,

    /** 
     * MUL: Multiplies the color values of the source and base.
     * This blending mode produces darker results and is useful for shadows.
     */
    MUL,

    /** 
     * SCREEN: Inverts, multiplies, and inverts the colors again for a 
     * lightening effect. Useful for softening colors and highlights.
     */
    SCREEN,

    /** 
     * DARKEN: Chooses the darker color between the source and base.
     * Helps to produce shadowed or muted colors.
     */
    DARKEN,

    /** 
     * LIGHTEN: Chooses the lighter color between the source and base.
     * Useful for emphasizing highlights.
     */
    LIGHTEN,

    /** 
     * DIFFERENCE: Computes the absolute difference between the source 
     * and base color values. Produces a high-contrast effect.
     */
    DIFFERENCE,

    /** 
     * EXCLUSION: A softer version of DIFFERENCE with less contrast,
     * creating a subtle effect with muted colors.
     */
    EXCLUSION,

    /** 
     * DODGE: Brightens the base color depending on the source color.
     * Commonly used to create highlight effects.
     */
    DODGE,

    /** 
     * BURN: Darkens the base color based on the source color.
     * Often used to add depth and shadow effects.
     */
    BURN
};

/**
 * @struct color
 * @brief A structure representing a color with various constructors and color manipulation methods.
 *
 * The Color struct encapsulates color information, providing constructors to create colors
 * from various representations and methods to manipulate and convert color values.
 */
struct Color
{
    uint8_t r;       ///< The red component of the color, as a byte in the range [0, 255].
    uint8_t g;       ///< The green component of the color, as a byte in the range [0, 255].
    uint8_t b;       ///< The blue component of the color, as a byte in the range [0, 255].
    uint8_t a;       ///< The alpha (transparency) component of the color, as a byte in the range [0, 255].

    /**
     * @brief Convert HSV (Hue, Saturation, Value) components to RGB and create a color.
     *
     * This static function converts HSV color space to RGB color space based on the provided
     * Hue, Saturation, and Value components. The resulting RGB values are used to create a color.
     *
     * @param hue The hue component of the color in degrees (0 to 360).
     * @param saturation The saturation component of the color as a float in the range [0.0, 1.0].
     * @param value The value component of the color as a float in the range [0.0, 1.0].
     * @param alpha The alpha component of the color as a float in the range [0.0, 1.0].
     * @return The resulting color after conversion from HSV components.
     */
    static constexpr Color from_hsv(float hue, float saturation = 1.0f, float value = 1.0f, float alpha = 1.0f) {
        float c = value * saturation;
        float x = c * (1 - std::abs(std::fmod(hue/60, 2) - 1));
        float m = value - c;
        float r = 0, g = 0, b = 0;
        if (hue >= 0 && hue < 60) {
            r = c, g = x, b = 0;
        } else if (hue >= 60 && hue < 120) {
            r = x, g = c, b = 0;
        } else if (hue >= 120 && hue < 180) {
            r = 0, g = c, b = x;
        } else if (hue >= 180 && hue < 240) {
            r = 0, g = x, b = c;
        } else if (hue >= 240 && hue < 300) {
            r = x, g = 0, b = c;
        } else if (hue >= 300 && hue < 360) {
            r = c, g = 0, b = x;
        }
        return {
            static_cast<uint8_t>((r + m) * 255),
            static_cast<uint8_t>((g + m) * 255),
            static_cast<uint8_t>((b + m) * 255),
            static_cast<uint8_t>(alpha * 255)
        };
    }

    /**
     * @brief Create a color from individual RGB and alpha float components.
     *
     * This static function creates a color by directly converting the provided
     * RGB and alpha float components (ranging from 0.0 to 255.0) to an 8-bit format.
     * Each component is scaled and converted to an 8-bit integer.
     *
     * @param r The red component of the color as a float in the range [0.0, 255.0].
     * @param g The green component of the color as a float in the range [0.0, 255.0].
     * @param b The blue component of the color as a float in the range [0.0, 255.0].
     * @param a The alpha (transparency) component of the color as a float in the range [0.0, 255.0] (default is 255).
     * @return The resulting color created from the specified RGB and alpha components.
     */
    static constexpr Color from_floats(float r, float g, float b, float a = 1.0f) {
        return {
            static_cast<uint8_t>(r / 255),
            static_cast<uint8_t>(g / 255),
            static_cast<uint8_t>(b / 255),
            static_cast<uint8_t>(a / 255)
        };
    }

    /**
     * @brief Create a grayscale color with optional alpha transparency.
     *
     * This static function creates a grayscale color by taking a single gray-level float component
     * and applying it to each RGB channel. The gray value is scaled and converted to an 8-bit format.
     *
     * @param gray The gray component of the color as a float in the range [0.0, 255.0], applied to each RGB channel.
     * @param alpha The alpha (transparency) component of the color as a float in the range [0.0, 255.0] (default is 255).
     * @return The resulting grayscale color created from the specified gray and alpha components.
     */
    static constexpr Color from_gray(float gray, float alpha = 1.0f) {
        uint8_t g = gray / 255;
        return {
            g, g, g,
            static_cast<uint8_t>(alpha / 255)
        };
    }

    /**
     * @brief Default constructor for color, initializes the color as transparent black.
     */
    constexpr Color()
        : r(0)
        , g(0)
        , b(0)
        , a(0)
    { }

    /**
     * @brief Constructor for color with RGB and optional alpha values.
     *
     * @param r The red component of the color.
     * @param g The green component of the color.
     * @param b The blue component of the color.
     * @param a The optional alpha (transparency) component of the color (default is 255, fully opaque).
     */
    constexpr Color(uint8_t r, uint8_t g, uint8_t b, uint8_t a = 255)
        : r(r)
        , g(g)
        , b(b)
        , a(a)
    { }

    /**
     * @brief Constructor for color from a 32-bit color value.
     *
     * @param color The 32-bit color value.
     */
    constexpr Color(uint32_t color)
        : r((color & 0x000000FF))
        , g((color & 0x0000FF00) >> 8)
        , b((color & 0x00FF0000) >> 16)
        , a((color & 0xFF000000) >> 24)
    { }

    /**
     * @brief Conversion operator to Uint32.
     *
     * This conversion operator allows implicit conversion of the color object to a 32-bit ARGB color value.
     * It uses the ToUint32() method to obtain the equivalent Uint32 representation of the color.
     *
     * @return The color represented as a 32-bit ARGB color value.
     */
    constexpr operator uint32_t() const {
        return (static_cast<uint32_t>(a) << 24)
             | (static_cast<uint32_t>(b) << 16)
             | (static_cast<uint32_t>(g) << 8)
             | (static_cast<uint32_t>(r));
    }

    /**
     * @brief Add two color objects element-wise.
     *
     * This operator performs element-wise addition of the RGBA components of two color objects.
     * It returns a new color with the sum of corresponding components.
     *
     * @param other The color to add.
     * @return The resulting color after element-wise addition.
     */
    constexpr Color operator+(const Color& other) const {
        return {
            static_cast<uint8_t>(std::min(r + other.r, 255)),
            static_cast<uint8_t>(std::min(g + other.g, 255)),
            static_cast<uint8_t>(std::min(b + other.b, 255)),
            static_cast<uint8_t>(std::min(a + other.a, 255))
        };
    }

    /**
     * @brief Add a color object to the current color object element-wise.
     *
     * This operator performs element-wise addition of the RGBA components of a color object to the current color object.
     * It modifies the current color object by adding the corresponding components of the provided color object.
     *
     * @param other The color to add.
     * @return A reference to the modified color object after element-wise addition.
     */
    constexpr const Color& operator+=(const Color& other) {
        r = static_cast<uint8_t>(std::min(r + other.r, 255));
        g = static_cast<uint8_t>(std::min(g + other.g, 255));
        b = static_cast<uint8_t>(std::min(b + other.b, 255));
        a = static_cast<uint8_t>(std::min(a + other.a, 255));
        return *this;
    }

    /**
     * @brief Subtract one color object from another element-wise.
     *
     * This operator performs element-wise subtraction of the RGBA components of two color objects.
     * It returns a new color with the difference of corresponding components.
     *
     * @param other The color to subtract.
     * @return The resulting color after element-wise subtraction.
     */
    constexpr Color operator-(const Color& other) const {
        return {
            static_cast<uint8_t>(std::max(r - other.r, 0)),
            static_cast<uint8_t>(std::max(g - other.g, 0)),
            static_cast<uint8_t>(std::max(b - other.b, 0)),
            static_cast<uint8_t>(std::max(a - other.a, 0))
        };
    }

    /**
     * @brief Subtract one color object from another element-wise.
     *
     * This operator performs element-wise subtraction of the RGBA components of two color objects.
     * It modifies the current color object by subtracting the corresponding components of the provided color object.
     *
     * @param other The color to subtract.
     * @return A reference to the modified color object after element-wise subtraction.
     */
    constexpr const Color& operator-=(const Color& other) {
        r = static_cast<uint8_t>(std::max(r - other.r, 0));
        g = static_cast<uint8_t>(std::max(g - other.g, 0));
        b = static_cast<uint8_t>(std::max(b - other.b, 0));
        a = static_cast<uint8_t>(std::max(a - other.a, 0));
        return *this;
    }

    /**
     * @brief Multiply two color objects element-wise.
     *
     * This operator performs element-wise multiplication of the RGBA components of two color objects.
     *
     * @param other The color to multiply with.
     * @return The resulting color after element-wise multiplication.
     */
    constexpr Color operator*(const Color& other) const {
        return {
            static_cast<uint8_t>((r * other.r) / 255),
            static_cast<uint8_t>((g * other.g) / 255),
            static_cast<uint8_t>((b * other.b) / 255),
            static_cast<uint8_t>((a * other.a) / 255)
        };
    }

    /**
     * @brief Multiply the current color object by another color object element-wise.
     *
     * This operator performs element-wise multiplication of the RGBA components of the current color object by another color object.
     * It modifies the current color object by multiplying its corresponding components with the components of the provided color object.
     *
     * @param other The color to multiply with.
     * @return A reference to the modified color object after element-wise multiplication.
     */
    constexpr const Color& operator*=(const Color& other) {
        r = (r * other.r) / 255;
        g = (g * other.g) / 255;
        b = (b * other.b) / 255;
        a = (a * other.a) / 255;
        return *this;
    }

    /**
     * @brief Multiplication operator for scaling the color by a value.
     *
     * This operator multiplies each channel (red, green, blue, alpha) of the color by the specified value.
     * The result is a new color object with the scaled channel values.
     *
     * @param value The value to scale the color channels by.
     * @return A new color object with scaled channel values.
     */
    template <typename T>
    constexpr Color operator*(T value) const {
        return {
            static_cast<uint8_t>(std::clamp(r * value, T(0), T(255))),
            static_cast<uint8_t>(std::clamp(g * value, T(0), T(255))),
            static_cast<uint8_t>(std::clamp(b * value, T(0), T(255))),
            static_cast<uint8_t>(std::clamp(a * value, T(0), T(255)))
        };
    }

    /**
     * @brief Multiply each channel of the current color object by a specified value.
     *
     * This operator multiplies each channel (red, green, blue, alpha) of the current color object by the specified value.
     * It modifies the current color object by scaling its channel values.
     *
     * @tparam T The type of value to scale the color channels by.
     * @param value The value to scale the color channels by.
     * @return A reference to the modified color object with scaled channel values.
     */
    template <typename T>
    constexpr const Color& operator*=(T value) {
        r = std::clamp(r * value, T(0), T(255));
        g = std::clamp(g * value, T(0), T(255));
        b = std::clamp(b * value, T(0), T(255));
        a = std::clamp(a * value, T(0), T(255));
        return *this;
    }

    /**
     * @brief Division operator for scaling down the color by a value.
     *
     * This operator divides each channel (red, green, blue, alpha) of the color by the specified value.
     * The result is a new color object with the scaled-down channel values.
     *
     * @param value The value to divide the color channels by.
     * @return A new color object with scaled-down channel values.
     */
    template <typename T>
    constexpr Color operator/(T value) const {
        return {
            std::clamp(r / value, T(0), T(255)),
            std::clamp(g / value, T(0), T(255)),
            std::clamp(b / value, T(0), T(255)),
            std::clamp(a / value, T(0), T(255))
        };
    }

    /**
     * @brief Divide each channel of the current color object by a specified value.
     *
     * This operator divides each channel (red, green, blue, alpha) of the current color object by the specified value.
     * It modifies the current color object by scaling down its channel values.
     *
     * @tparam T The type of value to divide the color channels by.
     * @param value The value to divide the color channels by.
     * @return A reference to the modified color object with scaled-down channel values.
     */
    template <typename T>
    constexpr const Color& operator/=(T value) {
        r = std::clamp(r / value, T(0), T(255));
        g = std::clamp(g / value, T(0), T(255));
        b = std::clamp(b / value, T(0), T(255));
        a = std::clamp(a / value, T(0), T(255));
        return *this;
    }

    /**
     * @brief Check if two color objects are equal.
     *
     * This operator checks equality by comparing the RGBA components of two color objects.
     *
     * @param other The color to compare with.
     * @return True if the colors are equal, false otherwise.
     */
    constexpr bool operator==(const Color& other) const {
        return (r == other.r)
            && (g == other.g)
            && (b == other.b)
            && (a == other.a);
    }

    /**
     * @brief Check if two color objects are not equal.
     *
     * This operator checks inequality by comparing the RGBA components of two color objects.
     *
     * @param other The color to compare with.
     * @return True if the colors are not equal, false otherwise.
     */
    constexpr bool operator!=(const Color& other) const {
        return (r != other.r)
            || (g != other.g)
            || (b != other.b)
            || (a != other.a);
    }

    /**
     * @brief Convert the color to HSV (Hue, Saturation, Value) components.
     *
     * This function calculates the HSV (Hue, Saturation, Value) representation of the color,
     * where:
     * - Hue (H) is the angle in degrees in the range [0, 360].
     * - Saturation (S) is in the range [0.0, 1.0] and represents the intensity of the color.
     * - Value (V) is in the range [0.0, 1.0] and represents the brightness.
     * 
     * @return The color represented as HSV components (Hue in degrees, Saturation in the range [0.0, 1.0], Value in the range [0.0, 1.0]).
     */
    constexpr std::tuple<float, float, float> to_hsv() const {
        float cmax = std::max(r, std::max(g, b)) / 255.0f;
        float cmin = std::min(r, std::min(g, b)) / 255.0f;
        float delta = cmax - cmin;
        float h = 0, s = 0, v = 0;

        // Hue calculation
        if (delta == 0) {
            h = 0;
        } else if (cmax == r) {
            h = 60 * std::fmod((g - b) / delta, 6);
        } else if (cmax == g) {
            h = 60 * ((b - r) / delta + 2);
        } else {
            h = 60 * ((r - b) / delta + 4);
        }

        // Saturation calculation
        if (cmax == 0) s = 0;
        else s = delta / cmax;

        // Value
        v = cmax;

        return { h, s, v };
    }
};

/* Default Colors */

constexpr Color WHITE        = Color(255, 255, 255, 255);
constexpr Color BLACK        = Color(0, 0, 0, 255);
constexpr Color BLANK        = Color(0, 0, 0, 0);
constexpr Color LIGHTGRAY    = Color(211, 211, 211, 255);
constexpr Color SILVER       = Color(192, 192, 192, 255);
constexpr Color GRAY         = Color(127, 127, 127, 255);
constexpr Color DARKGRAY     = Color(80, 80, 80, 255);
constexpr Color YELLOW       = Color(255, 255, 0, 255);
constexpr Color GOLD         = Color(255, 215, 0, 255);
constexpr Color ORANGE       = Color(255, 165, 0, 255);
constexpr Color PINK         = Color(255, 105, 180, 255);
constexpr Color RED          = Color(255, 0, 0, 255);
constexpr Color MAROON       = Color(128, 0, 0, 255);
constexpr Color GREEN        = Color(0, 255, 0, 255);
constexpr Color DARKGREEN    = Color(0, 100, 0, 255);
constexpr Color SKYBLUE      = Color(135, 206, 235, 255);
constexpr Color BLUE         = Color(0, 0, 255, 255);;
constexpr Color DARKBLUE     = Color(0, 0, 100, 255);
constexpr Color PURPLE       = Color(127, 0, 127, 255);
constexpr Color VIOLET       = Color(238, 130, 238, 255);
constexpr Color DARKPURPLE   = Color(79, 0, 79, 255);
constexpr Color BEIGE        = Color(255, 198, 153, 255);
constexpr Color BROWN        = Color(139, 69, 19, 255);
constexpr Color DARKBROWN    = Color(92, 64, 51, 255);
constexpr Color MAGENTA      = Color(255, 0, 255, 255);
constexpr Color CYAN         = Color(0, 255, 255, 255);

} // namesapce bpx

#endif // BPX_COLOR_HPP
