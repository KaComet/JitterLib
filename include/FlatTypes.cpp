#include "FlatTypes.h"
#include "FlatMath.h"


/*
 * Coordinate functions
 */
flat::IntegerCoordinate::IntegerCoordinate(int_fast64_t x, int_fast64_t y) : x(x), y(y) {

}

bool flat::IntegerCoordinate::operator==(const IntegerCoordinate &b) const noexcept {
    return (y == b.y) && (x == b.x);
}

flat::IntegerCoordinate flat::IntegerCoordinate::operator-(const IntegerCoordinate &b) const noexcept {
    return IntegerCoordinate(x - b.x, y - b.y);
}

flat::IntegerCoordinate flat::IntegerCoordinate::operator+(const IntegerCoordinate &b) const noexcept {
    return IntegerCoordinate(x + b.x, y + b.y);
}

flat::IntegerCoordinate &flat::IntegerCoordinate::operator-=(const IntegerCoordinate &b) noexcept {
    x -= b.x;
    y -= b.y;
    return *this;
}


/*
 * Rectangle functions
 */
flat::Rectangle::Rectangle(int_fast32_t width, int_fast32_t height) : width(width), height(height) {
    if (height < 0)
        height = 0;

    if (width < 0)
        width = 0;
}

bool flat::Rectangle::operator==(const Rectangle &b) const noexcept {
    return (width == b.width) && (height == b.height);
}

[[nodiscard]] bool flat::Rectangle::contains(const IntegerCoordinate &cord) const noexcept {
    return (cord.x < width) && (cord.y < height);
}


/*
 * CoordinateAndRectangle functions
 */
flat::CoordinateAndRectangle::CoordinateAndRectangle(int_fast32_t x, int_fast32_t y, int_fast32_t width,
                                                     int_fast32_t height) : x(x), y(y), width(width), height(height) {
    if (height < 0)
        height = 0;

    if (width < 0)
        width = 0;
}

flat::CoordinateAndRectangle::CoordinateAndRectangle(const flat::IntegerCoordinate &cord, flat::Rectangle rect) : x(cord.x),
                                                                                                                  y(cord.y),
                                                                                                                  width(rect.width),
                                                                                                                  height(rect.height) {

}

bool flat::CoordinateAndRectangle::operator==(const CoordinateAndRectangle &b) const noexcept {
    return (x == b.x) && (y == b.y) && (width == b.width) && (height == b.height);
}
