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

namespace walt {
    typedef uint_fast64_t EID;

    template<class O, typename ID, class SM>
    class ObjectAndData {
    public:
        ObjectAndData() = delete;

        explicit ObjectAndData(std::shared_ptr<O> object, const flat::IntegerCoordinate &position, ID id,
                               std::weak_ptr<bool> lockPtr)
                : _object(object), _position(position), _id(id), _lockPtr(std::move(lockPtr)) {};

        // Returns true if locked
        [[nodiscard]] bool isLocked() const {
            if (auto l = _lockPtr.lock())
                return *l;
            throw std::runtime_error("Lock cannot be found");
        }

        // Tick's the object.
        void tickObject(std::weak_ptr<SM> simPtr, std::weak_ptr<ObjectAndData<O, ID, SM>> self) {
            _object->tick(simPtr, self);
        }

        // Returns a const weak pointer.
        std::weak_ptr<const O> constObject() const noexcept {
            return std::const_pointer_cast<O>(_object);
        }

        // Returns the object's position.
        [[nodiscard]] flat::IntegerCoordinate constPosition() const noexcept {
            return _position;
        }

        // Returns the object's ID.
        ID constId() const noexcept {
            return _id;
        }

        // Returns a weak pointer of the object. Throws if locked.
        std::weak_ptr<O> object() {
            if (!isLocked()) {
                return _object;
            } else {
                throw std::runtime_error("Tried to access locked resource");
            }
        }

        // Returns a reference to the object's position. Throws if locked.
        flat::IntegerCoordinate &position() {
            if (!isLocked()) {
                return (flat::IntegerCoordinate &) _position;
            } else {
                throw std::runtime_error("Tried to access locked resource");
            }
        }

        // Returns a reference to the object's ID. Throws if locked.
        ID &id() {
            if (!isLocked()) {
                return (ID &) _id;
            } else {
                throw std::runtime_error("Tried to access locked resource");
            }
        }

    private:
        std::shared_ptr<O> _object;
        flat::IntegerCoordinate _position;
        ID _id;
        std::weak_ptr<bool> _lockPtr; // Locked when true.
    };

    inline uint getArrayIndex(const flat::IntegerCoordinate &coordinate, const uint arrayWidth) {
        return coordinate.x + (coordinate.y * arrayWidth);
    }
}


#endif //WALT_BASICS_H
