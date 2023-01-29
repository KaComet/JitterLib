#ifndef WALT_TILE_H
#define WALT_TILE_H

#include <cstdint>
#include <utility>
#include "JitDisplayMaterial.h"

namespace Jit {

    struct JitDisplayTile {
        JitDisplayMaterial floorMaterial, wallMaterial;

        JitDisplayTile() = default;

        JitDisplayTile(const JitDisplayMaterial& floorMaterial, const JitDisplayMaterial& wallMaterial) : floorMaterial(floorMaterial),
                                                                                                          wallMaterial(wallMaterial) {}
    };
}

#endif //WALT_TILE_H
