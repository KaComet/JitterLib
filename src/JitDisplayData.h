#ifndef SHEER_DISPLAYDATA_H
#define SHEER_DISPLAYDATA_H

#include "JitBasics.h"
#include "JitDisplayTile.h"
#include <stdexcept>
#include <HappyArray.h>
#include <vector>
#include <FlatTypes.h>
#include <FlatMath.h>

namespace Jit {

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
        std::vector<Status> statuses;
        RelativeFlag flag;
    };

    class JitDisplayData {
    public:
        JitDisplayData();

        explicit JitDisplayData(flat::CoordinateAndRectangle area);

        JitDisplayData &operator=(const JitDisplayData &other);

        // Returns the tile at the given coordinate.
        Jit::JitDisplayTile &at(flat::IntegerCoordinate cord);

        [[nodiscard]] flat::CoordinateAndRectangle area() const noexcept;

        [[nodiscard]] flat::CoordinateAndRectangle bufferedArea() const noexcept;

        [[nodiscard]] std::vector<entityDisplayInfo> &entities() noexcept;

    private:
        flat::CoordinateAndRectangle _area;
        std::vector<entityDisplayInfo> _entities;
        flat::HappyArray<Jit::JitDisplayTile> _tiles;
        flat::CoordinateAndRectangle _bufferedArea;
        unsigned int _totalArea;
    };
}


#endif //SHEER_DISPLAYDATA_H
