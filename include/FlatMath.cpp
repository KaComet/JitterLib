#include "FlatMath.h"

// Returns true if the coordinate is within the provided CoordinateAndRectangle.
bool flat::isInBound(const IntegerCoordinate &coordinate, const CoordinateAndRectangle &coordinateAndRectangle) {
    return (coordinateAndRectangle.x <= coordinate.x) && (coordinateAndRectangle.y <= coordinate.y) &&
           (coordinate.x < (coordinateAndRectangle.x + coordinateAndRectangle.width)) &&
           (coordinate.y < (coordinateAndRectangle.y + coordinateAndRectangle.height));
}

flat::IntegerCoordinate flat::makeUnit(const IntegerCoordinate &cord) {
    IntegerCoordinate result;
    if (cord.x != 0) {
        if (cord.x > 0)
            result.x = 1;
        else
            result.x = -1;
    } else
        result.x = 0;

    if (cord.y != 0) {
        if (cord.y > 0)
            result.y = 1;
        else
            result.y = -1;
    } else
        result.y = 0;

    return result;
}

bool flat::cordIsUnidirectional(const IntegerCoordinate &cord) {
    if ((cord.x != 0) && (cord.y == 0))
        return true;

    if ((cord.x == 0) && (cord.y != 0))
        return true;

    return false;
}