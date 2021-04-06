#ifndef WALT_BASICS_H
#define WALT_BASICS_H

#include <cstdint>
#include <memory>
#include <cmath>
#include <vector>
#include <optional>
#include <stdexcept>
#include <exception>
#include <FlatTypes.h>

namespace Jit {
    template<class X, class Y>
    std::vector<std::pair<X, Y>> getLinePath(float x1, float y1, float x2, float y2) {
        std::vector<std::pair<X, Y>> result;

        // Bresenham's line algorithm
        const bool steep = (fabs(y2 - y1) > fabs(x2 - x1));
        if (steep) {
            std::swap(x1, y1);
            std::swap(x2, y2);
        }

        if (x1 > x2) {
            std::swap(x1, x2);
            std::swap(y1, y2);
        }

        const float dx = x2 - x1;
        const float dy = fabs(y2 - y1);

        float error = dx / 2.0f;
        const int ystep = (y1 < y2) ? 1 : -1;
        int y = (int) y1;

        const int maxX = (int) x2;

        for (int x = (int) x1; x <= maxX; x++) {
            if (steep) {
                result.emplace_back(y, x);
            } else {
                result.emplace_back(x, y);
            }

            error -= dy;
            if (error < 0) {
                y += ystep;
                error += dx;
            }
        }

        return result;
    }

    template<class C>
    std::vector<C> getLinePath(float x1, float y1, float x2, float y2) {
        std::vector<C> result;

        // Bresenham's line algorithm
        const bool steep = (std::abs(y2 - y1) > std::abs(x2 - x1));
        if (steep) {
            std::swap(x1, y1);
            std::swap(x2, y2);
        }

        if (x1 > x2) {
            std::swap(x1, x2);
            std::swap(y1, y2);
        }

        const float dx = x2 - x1;
        const float dy = std::abs(y2 - y1);

        float error = dx / 2.0f;
        const int ystep = (y1 < y2) ? 1 : -1;
        int y = (int) y1;

        const int maxX = (int) x2;

        for (int x = (int) x1; x <= maxX; x++) {
            if (steep) {
                result.emplace_back(y, x);
            } else {
                result.emplace_back(x, y);
            }

            error -= dy;
            if (error < 0) {
                y += ystep;
                error += dx;
            }
        }

        return result;
    }


    inline uint getArrayIndex(const flat::IntegerCoordinate &coordinate, const uint arrayWidth) {
        return coordinate.x + (coordinate.y * arrayWidth);
    }
}

#endif //WALT_BASICS_H
