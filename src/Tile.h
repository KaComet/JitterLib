#ifndef WALT_TILE_H
#define WALT_TILE_H

#include <cstdint>
#include <utility>
#include "Material.h"

namespace walt::core {

    struct Tile {
        Material floorMaterial, wallMaterial;
        uint wallHealth;

        Tile() { wallHealth = 0; }

        Tile(const Material& floorMaterial, const Material& wallMaterial, uint wallHealth) : floorMaterial(floorMaterial),
                                                                               wallMaterial(wallMaterial),
                                                                               wallHealth(wallHealth) {}
    };

    const Tile constTileOutOfBound = Tile(constMaterialOutOfBound,
                                          constMaterialOutOfBound,
                                          constMaterialOutOfBound.defaultHealth);
}

#endif //WALT_TILE_H
