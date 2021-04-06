#ifndef SHEER_MATERIAL_H
#define SHEER_MATERIAL_H

#include <string>
#include <utility>

struct Material {
    std::string materialName;
    unsigned int defaultHealth;

    Material() {
        materialName = "";
        defaultHealth = 0;
    }

    Material(std::string materialName, unsigned int defaultHealth) : materialName(std::move(materialName)),
                                                                     defaultHealth(defaultHealth) {}

    Material(const Material &other) = default;

    bool operator==(const Material &other) const {
        return (materialName == other.materialName) && (defaultHealth == other.defaultHealth);
    }
};

const Material constMaterialOutOfBound = Material{"outofbounds", 0};
const Material constMaterialStone = Material{"stone", 100};

#endif //SHEER_MATERIAL_H
