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

#ifndef BPX_RAMP_HPP
#define BPX_RAMP_HPP

#include "algorithm.hpp"
#include "color.hpp"
#include <stdexcept>
#include <vector>

namespace bpx {

/**
 * @class ColorRamp
 * @brief A class representing a color ramp, which interpolates colors between defined points.
 *
 * A color ramp stores a set of colors and their positions in a range between 0 and 1. The ramp can either use
 * static storage (for exactly two points) or dynamic storage (for more than two points). The color ramp allows
 * for efficient interpolation between colors based on the position.
 */
class ColorRamp
{
public:
    /**
     * @struct Point
     * @brief A struct representing a color point in the ramp.
     *
     * A point holds a color and its associated position along the ramp. The position should be between 0.0 and 1.0.
     */
    struct Point {
        Color color;    ///< The color at this point.
        float position; ///< The position along the ramp (0.0 to 1.0).

        /**
         * @brief Constructs a color point with the given color and position.
         * @param color The color at the point.
         * @param position The position of the point along the ramp (0.0 to 1.0).
         */
        constexpr Point(Color color, float position) noexcept
            : color(color), position(position)
        { }
    };

public:
    /**
     * @brief Constructs a color ramp with two static colors.
     * 
     * This constructor initializes the ramp with two colors, one at position 0.0 and one at position 1.0.
     * The colors are stored in static memory, meaning only these two colors can be used in the ramp.
     * 
     * @param color0 The color at position 0.0.
     * @param color1 The color at position 1.0.
     */
    ColorRamp(Color color0, Color color1) noexcept;

    /**
     * @brief Constructs a color ramp from an initializer list of points.
     *
     * This constructor allows the creation of a color ramp with any number of points. If the initializer list
     * contains more than two points, dynamic storage will be used to store the points. The points will be sorted
     * in ascending order of their position along the ramp.
     * 
     * @param points An initializer list of `Point` objects.
     * @throws std::invalid_argument if the initializer list contains fewer than 2 points.
     */
    ColorRamp(std::initializer_list<Point> points);

    /**
     * @brief Destroys the color ramp and releases any allocated memory.
     */
    ~ColorRamp();

    // Delete copy constructor and copy assignment
    ColorRamp(const ColorRamp&) = delete;               ///< Copy constructor (deleted).
    ColorRamp& operator=(const ColorRamp&) = delete;    ///< Copy assignment operator (deleted).

    /**
     * @brief Move constructor for the color ramp.
     * 
     * This constructor transfers ownership of another `ColorRamp`'s resources to this one, leaving the original
     * ramp in a valid but unspecified state.
     * 
     * @param other The color ramp to move from.
     */
    ColorRamp(ColorRamp&& other) noexcept;

    /**
     * @brief Move assignment operator for the color ramp.
     * 
     * This operator transfers ownership of another `ColorRamp`'s resources to this one, replacing any existing
     * data in this ramp.
     * 
     * @param other The color ramp to move from.
     * @return A reference to this color ramp.
     */
    ColorRamp& operator=(ColorRamp&& other) noexcept;

    /**
     * @brief Adds a new point to the color ramp.
     * 
     * This method adds a new color at a given position. The position is clamped to the range [0.0, 1.0]. If the
     * position already exists in the ramp, the color is updated. If the ramp is using static storage and the
     * position is neither 0.0 nor 1.0, the ramp will transition to dynamic storage.
     * 
     * @param color The color to add.
     * @param position The position at which to add the color (0.0 to 1.0).
     */
    void add(const Color& color, float position);

    /**
     * @brief Retrieves the color at a specified position in the ramp.
     * 
     * This method returns the color corresponding to the given position `t` in the range [0.0, 1.0]. The color is
     * interpolated between the nearest points if necessary.
     * 
     * @param t The position to get the color at (0.0 to 1.0).
     * @return The interpolated color at the specified position.
     */
    Color get(float t) const;

private:
    union {
        std::array<Point, 2> m_static;  ///< Static storage for two points (0.0 and 1.0).
        std::vector<Point> m_dynamic;   ///< Dynamic storage for more than two points.
    };
    bool m_use_dynamic; ///< Flag to determine if dynamic storage is in use.

private:
    /**
     * @brief Destroys the current data in the union and releases any allocated memory.
     */
    void destroy();
};


/* Public implementation */

inline ColorRamp::ColorRamp(Color color0, Color color1) noexcept
    : m_use_dynamic(false)
{
    new (&m_static) std::array<Point, 2> {
        Point(color0, 0.0f),
        Point(color1, 1.0f)
    };
}

inline ColorRamp::ColorRamp(std::initializer_list<Point> points)
{
    if (points.size() < 2) {
        throw std::invalid_argument("ColorRamp requires at least 2 points.");
    }
    if (points.size() > 2) {
        // Use dynamic storage
        m_use_dynamic = true;
        new (&m_dynamic) std::vector<Point>(points); // Direct initialization
        std::sort(m_dynamic.begin(), m_dynamic.end(),
                [](const Point& a, const Point& b) {
                    return a.position < b.position;
                });
    } else {
        // Use static storage
        m_use_dynamic = false;
        auto it = points.begin();
        new (&m_static) std::array<Point, 2>{ *it++, *it++ }; // Exact size guaranteed
    }
}

inline ColorRamp::~ColorRamp()
{
    destroy();
}

inline ColorRamp::ColorRamp(ColorRamp&& other) noexcept
    : m_use_dynamic(other.m_use_dynamic)
{
    if (m_use_dynamic) {
        new (&m_dynamic) std::vector<Point>(std::move(other.m_dynamic));
    } else {
        new (&m_static) std::array<Point, 2>(std::move(other.m_static));
    }
}

inline ColorRamp& ColorRamp::operator=(ColorRamp&& other) noexcept
{
    if (this != &other) {
        destroy();
        m_use_dynamic = other.m_use_dynamic;
        if (m_use_dynamic) {
            new (&m_dynamic) std::vector<Point>(std::move(other.m_dynamic));
        } else {
            new (&m_static) std::array<Point, 2>(std::move(other.m_static));
        }
    }
    return *this;
}

inline void ColorRamp::add(const Color& color, float position)
{
    // Clamp the position to [0, 1]
    position = std::clamp(position, 0.0f, 1.0f);

    if (!m_use_dynamic) {
        // Static storage case
        for (auto& point : m_static) {
            if (point.position == position) {
                point.color = color;
                return;
            }
        }
        // Transition to dynamic storage if position doesn't match
        std::array<Point, 2> temp = m_static;
        destroy();
        m_use_dynamic = true;
        new (&m_dynamic) std::vector<Point>(temp.begin(), temp.end());
        m_dynamic.emplace_back(color, position);
        std::sort(m_dynamic.begin(), m_dynamic.end(),
                [](const Point& a, const Point& b) {
                    return a.position < b.position;
                });
        return;
    }

    // Dynamic storage case
    auto it = std::find_if(m_dynamic.begin(), m_dynamic.end(),
                        [position](const Point& p) { return p.position == position; });
    if (it != m_dynamic.end()) {
        // Replace existing point
        it->color = color;
    } else {
        // Add new point and sort
        m_dynamic.emplace_back(color, position);
        std::sort(m_dynamic.begin(), m_dynamic.end(),
                [](const Point& a, const Point& b) {
                    return a.position < b.position;
                });
    }
}

inline Color ColorRamp::get(float t) const
{
    t = std::clamp(t, 0.0f, 1.0f);

    if (!m_use_dynamic) {
        // Use static points
        if (t <= m_static[0].position) return m_static[0].color;
        if (t >= m_static[1].position) return m_static[1].color;
        return lerp(m_static[0].color, m_static[1].color, t);
    }

    // Use dynamic points
    if (t <= m_dynamic.front().position) return m_dynamic.front().color;
    if (t >= m_dynamic.back().position) return m_dynamic.back().color;

    for (size_t i = 1; i < m_dynamic.size(); ++i) {
        if (t <= m_dynamic[i].position) {
            const Point& cp1 = m_dynamic[i - 1];
            const Point& cp2 = m_dynamic[i];
            float blend = (t - cp1.position) / (cp2.position - cp1.position);
            return lerp(cp1.color, cp2.color, blend);
        }
    }

    return m_dynamic.back().color;
}

/* Private implementation */

inline void ColorRamp::destroy()
{
    if (m_use_dynamic) {
        m_dynamic.~vector<Point>();
    } else {
        m_static.~array<Point, 2>();
    }
}

} // namespace bpx

#endif // BPX_RAMP_HPP
