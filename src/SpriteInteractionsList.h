#ifndef WELT_SPRITEINTERACTIONSLIST_H
#define WELT_SPRITEINTERACTIONSLIST_H

#include <string>
#include <map>
#include <cctype>
#include <fstream>
#include <algorithm>
#include <stdexcept>
#include <optional>
#include <FlatTypes.h>
#include "resource.h"
#include "DisplayData.h"
#include "Basics.h"
#include "Tile.h"
#include "Material.h"
#include "UIBasics.h"
#include "FrameDefMap.h"


const unsigned int CACHE_SIZE = 5;

// Stores mutable SpriteInteractions for easy use.

union SI_Test {

};

class SpriteInteractionsList {
public:
    SpriteInteractionsList() = default;

    /* Clears the current definitions and loads the definitions
     *   in the given file. Returns true if successful. */
    bool loadSpriteInteractions(const std::string &fileName, const std::string &tag);

    /* When given a DisplayArray and a coordinate, this function will determine the correct
     *   sprite to display for the given tile. */
    std::optional<FrameDef> get(const flat::IntegerCoordinate& cord,
                                walt::Display::DisplayData &dis,
                                FrameDefMap &frameDefs,
                                bool gettingBackground);

    std::optional<FrameDef> getFrameDef(const std::string &name, FrameDefMap &frameDefs);

private:
    // Returns the SpriteInteraction for the given material.
    [[nodiscard]] std::optional<walt::UI::SpriteInteraction> getSI(const Material &material);

    [[nodiscard]] static FrameDef getInteraction(const Material &material,
                                                 const walt::UI::SpriteInteraction &spriteInteraction,
                                                 walt::Display::DisplayData &dis,
                                                 FrameDefMap &frameDefs,
                                                 const flat::IntegerCoordinate& cord,
                                                 bool gettingBackground);

    [[nodiscard]] static std::optional<walt::UI::SpriteInteraction> getFullSpriteInteraction(const std::string &input);

    [[nodiscard]] static walt::UI::SpriteInteraction getSimpleSpriteInteraction(const std::string &input);

    [[nodiscard]] static std::string getSI_Key(const std::string &input);

    [[nodiscard]] static bool isSimpleSI(const std::string &input);

    static bool isFullyDefined(const walt::UI::SpriteInteraction &SI);

    static bool isMultiLine(const std::string &input);

    static std::optional<walt::UI::SpriteInteraction> loadSI(const std::string &input);

    std::unordered_map<std::string, walt::UI::SpriteInteraction> SI_list;
};


#endif //WELT_SPRITEINTERACTIONSLIST_H
