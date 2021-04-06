#ifndef SHEER_DISPLAYDATA_H
#define SHEER_DISPLAYDATA_H

#include "Basics.h"
#include "Tile.h"
#include <stdexcept>
#include <HappyArray.h>
#include <vector>
#include <FlatTypes.h>
#include <FlatMath.h>

namespace walt::Display {

    enum class Status {
        Dead,
        Sleeping,
        Unconscious,
        injured,
        Sick,
        Catatonic,
        Panicking,
        Hallucinating,
        Exhausted,
        Depressed,
        Intoxicated,
        Stoned,
        Elated,
        Raging,
        Sleepy,
        Hungry,
        Thirsty,
        Starving,
        moving,
        notMoving
    };

    enum class RelativeFlag {
        Unevaluated,
        Friendly,
        Reserved,
        Hostile
    };

    struct entityDisplayInfo {
        std::string Name, Type;
        flat::IntegerCoordinate position;
        EID eid;
        std::vector<Status> statuses;
        RelativeFlag flag;
    };

    class DisplayData {
    public:
        DisplayData();

        explicit DisplayData(flat::CoordinateAndRectangle area);

        DisplayData &operator=(const DisplayData &other);

        // Returns the tile at the given coordinate.
        walt::core::Tile &at(flat::IntegerCoordinate cord);

        [[nodiscard]] flat::CoordinateAndRectangle area() const noexcept;

        [[nodiscard]] flat::CoordinateAndRectangle bufferedArea() const noexcept;

        [[nodiscard]] std::vector<entityDisplayInfo> &entities() noexcept;

    private:
        flat::CoordinateAndRectangle _area;
        std::vector<entityDisplayInfo> _entities;
        flat::HappyArray<walt::core::Tile> _tiles;
        flat::CoordinateAndRectangle _bufferedArea;
        unsigned int _totalArea;
    };
}


#endif //SHEER_DISPLAYDATA_H
