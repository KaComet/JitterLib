#ifndef SHEER_MATERIAL_H
#define SHEER_MATERIAL_H

#include <string>
#include <utility>

namespace Jit {
    struct JitDisplayMaterial {
        std::string materialName;

        JitDisplayMaterial() {
            materialName = "";
        }

        JitDisplayMaterial(std::string materialName) : materialName(std::move(materialName)) {}

        JitDisplayMaterial(const JitDisplayMaterial &other) = default;

        bool operator==(const JitDisplayMaterial &other) const {
            return materialName == other.materialName;
        }
    };

    const JitDisplayMaterial constMaterialOutOfBound = JitDisplayMaterial{"outofbounds"};
    const JitDisplayMaterial constMaterialStone = JitDisplayMaterial{"stone"};
}

#endif //SHEER_MATERIAL_H
