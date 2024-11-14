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

#ifndef BPX_RAMP_HPP
#define BPX_RAMP_HPP

#include "algorithm.hpp"
#include "color.hpp"
#include <vector>

namespace bpx {

/**
 * @brief A class for creating and managing a color ramp with multiple color stops.
 * 
 * The ColorRamp class allows you to define a gradient or "ramp" between multiple colors.
 * Colors can be retrieved at any position along the ramp, with interpolation applied
 * between defined color stops.
 */
class ColorRamp
{
public:
    /**
     * @brief Struct representing a color point in the ramp.
     * 
     * Each Point contains a color and a position along the ramp, where position is a
     * normalized value between 0 and 1.
     */
    struct Point {
        Color color;    ///< The color at this point.
        float position; ///< The position along the ramp (0.0 to 1.0).
    };

public:
    /**
     * @brief Constructor to initialize the ColorRamp with a list of color points.
     * 
     * This constructor allows initialization of the ColorRamp using an initializer list of
     * Point structures. Each point in the list defines a color and its position along the ramp.
     * 
     * @param points Initializer list of Points for defining the initial color ramp.
     */
    ColorRamp(std::initializer_list<Point> points) {
        for (const auto& point : points) {
            add(point.color, point.position);
        }
    }

    // Delete copy constructor and copy assignment
    ColorRamp(const ColorRamp&) = delete;               ///< Copy constructor (deleted).
    ColorRamp& operator=(const ColorRamp&) = delete;    ///< Copy assignment operator (deleted).

    /**
     * @brief Move constructor to transfer ownership from another ColorRamp instance.
     * 
     * This constructor transfers the data from the provided ColorRamp, leaving it in a valid,
     * empty state.
     * 
     * @param other Another ColorRamp instance to move from.
     */
    ColorRamp(ColorRamp&& other) noexcept 
        : m_points(std::move(other.m_points))
    { }

    /**
     * @brief Move assignment operator to transfer ownership from another ColorRamp instance.
     * 
     * This operator transfers the data from the provided ColorRamp, leaving it in a valid,
     * empty state. Ensures no action is taken if the same instance is assigned to itself.
     * 
     * @param other Another ColorRamp instance to move from.
     * @return A reference to the updated ColorRamp.
     */
    ColorRamp& operator=(ColorRamp&& other) noexcept {
        if (this != &other) {
            m_points = std::move(other.m_points);
        }
        return *this;
    }

    /**
     * @brief Adds a color point to the ramp at a specified position.
     * 
     * Adds a new color to the ramp at the given position. Positions are expected to be
     * normalized between 0.0 and 1.0. The ramp automatically sorts points by position
     * to ensure correct interpolation order.
     * 
     * @param color The color to add to the ramp.
     * @param position The position of the color along the ramp (0.0 to 1.0).
     */
    void add(const Color& color, float position) {
        if (position < 0.0f || position > 1.0f) return;
        m_points.emplace_back(color, position);
        std::sort(m_points.begin(), m_points.end(), 
                  [](const Point& a, const Point& b) {
                      return a.position < b.position;
                  });
    }

    /**
     * @brief Retrieves the color at a given position on the ramp.
     * 
     * This method returns the interpolated color at position t along the ramp.
     * If t is before the first color or after the last color, the color at the
     * nearest edge is returned. Otherwise, linear interpolation is applied.
     * 
     * @param t The normalized position (0.0 to 1.0) to retrieve the color for.
     * @return The interpolated Color at position t.
     */
    Color get(float t) const {
        if (m_points.empty()) return Color(0, 0, 0, 255);

        // Clamp t to the [0, 1] range
        t = std::clamp(t, 0.0f, 1.0f);

        // Handle the edge cases
        if (t <= m_points.front().position) return m_points.front().color;
        if (t >= m_points.back().position) return m_points.back().color;

        // Find the two color points surrounding t
        for (size_t i = 1; i < m_points.size(); ++i) {
            if (t <= m_points[i].position) {
                // Linear interpolation between m_points[i-1] and m_points[i]
                const Point& cp1 = m_points[i - 1];
                const Point& cp2 = m_points[i];
                float blend = (t - cp1.position) / (cp2.position - cp1.position);
                return lerp(cp1.color, cp2.color, blend);
            }
        }

        return m_points.back().color; // Fallback
    }

private:
    std::vector<Point> m_points; ///< Container for color points in the ramp.
};

} // namespace bpx

#endif // BPX_RAMP_HPP
