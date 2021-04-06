#include "DisplayData.h"

namespace walt::Display {
    DisplayData::DisplayData() : _tiles(flat::HappyArray<walt::core::Tile>(0)) {
        _totalArea = 0;
        _area = flat::CoordinateAndRectangle(0, 0, 0, 0);
        _bufferedArea = flat::CoordinateAndRectangle(0, 0, 0, 0);
    }

    DisplayData::DisplayData(flat::CoordinateAndRectangle area) : _tiles(flat::HappyArray<walt::core::Tile>(0)) {
        _area = area;

        area.x -= 1;
        area.y -= 1;
        area.height += 2;
        area.width += 2;
        _bufferedArea = area;

        // Store the current area (tile²)
        _totalArea = _bufferedArea.height * _bufferedArea.width;

        _tiles = flat::HappyArray<walt::core::Tile>(_totalArea);
    }

    DisplayData &DisplayData::operator=(const DisplayData &other) {
        if (this != &other) {
            // Copy the data.
            _tiles = other._tiles;

            // Reconfigure the settings.
            _area = other._area;
            _bufferedArea = other._bufferedArea;
            _entities = other._entities;
            _totalArea = _bufferedArea.height * _bufferedArea.width;
        }

        return *this;
    }

    // Returns the tile at the given coordinate.
    walt::core::Tile &DisplayData::at(flat::IntegerCoordinate cord) {
        // If the coordinate is outside the bounds of the array, throw out_of_range.
        if ((!flat::isInBound(cord, _bufferedArea)) || (!_tiles.size()) || (!_totalArea))
            throw std::out_of_range("Array index out of bounds.");

        cord.x -= _bufferedArea.x;
        cord.y -= _bufferedArea.y;

        return _tiles[walt::getArrayIndex(cord, _bufferedArea.width)];
    }

    flat::CoordinateAndRectangle DisplayData::area() const noexcept {
        return _area;
    }

    flat::CoordinateAndRectangle DisplayData::bufferedArea() const noexcept {
        return _bufferedArea;
    }

    std::vector<entityDisplayInfo> &DisplayData::entities() noexcept {
        return _entities;
    }
}