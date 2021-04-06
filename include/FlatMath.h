#ifndef TILESETVIEWER_FLATMATH_H
#define TILESETVIEWER_FLATMATH_H

#include <optional>
#include "FlatTypes.h"

/*
Name: Flat - Flat Math
Date: 8/9/2020 (MM/DD/YYYY)
Version: 1.01
*/

namespace flat {
    template<class T>
    T nonWrappingSubtraction(const T &a, const T &b) {
        if (b > a)
            return T(0);
        else
            return a - b;
    }

    template<class T>
    T atLeast(const T &a, const T &b) {
        if (a < b)
            return b;
        else
            return a;
    }

    template<class T>
    void ensureValue(std::optional<T> &input, std::optional<T> defaultValue) {
        if (input.has_value())
            return;
        else if (defaultValue.has_value()) {
            input = std::optional<T>(defaultValue);
            return;
        } else
            throw std::runtime_error("");
    }

    // Returns true if the coordinate is within the provided CoordinateAndRectangle.
    bool isInBound(const IntegerCoordinate &coordinate, const CoordinateAndRectangle &coordinateAndRectangle);

    IntegerCoordinate makeUnit(const IntegerCoordinate &cord);

    bool cordIsUnidirectional(const IntegerCoordinate &cord);
}

#endif //TILESETVIEWER_FLATMATH_H
