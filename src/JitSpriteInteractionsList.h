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
#include <FlatMath.h>
#include "JitResource.h"
#include "JitBasics.h"
#include "JitDisplayTile.h"
#include "JitDisplayMaterial.h"
#include "JitUIBasics.h"
#include "JitFrameDefMap.h"

namespace Jit {
    enum InteractionEnum {
        cccc = 0,
        cccW = 1,
        ccEc = 2,
        ccEW = 3,
        cScc = 4,
        cScW = 5,
        NccW = 9,
        NScW = 13,
        Nccc = 8,
        cSEc = 6,
        NcEc = 10,
        NSEc = 14,
        NScc = 12,
        cSEW = 7,
        NcEW = 11,
        NSEW = 15
    };

    class JitSpriteInteractionsList {
    public:
        JitSpriteInteractionsList() = default;

        /* Clears the current definitions and loads the definitions
         *   in the given file. Returns true if successful. */
        bool loadSpriteInteractions(const std::string &fileName, const std::string &tag);

        /* When given a DisplayArray and a coordinate, this function will determine the correct
         *   sprite to display for the given tile. */
        /*std::optional<FrameDef> get(const flat::IntegerCoordinate &cord,
                                    Jit::JitDisplayData &dis,
                                    JitFrameDefMap &frameDefs,
                                    bool gettingBackground);
                                    */

        FrameDef &getFrameDef(const std::string &name, JitFrameDefMap &frameDefs, Jit::FrameDef &errorFrame,
                              InteractionEnum interactions = InteractionEnum::cccc);

    private:
        const std::string LOADING_START_STRING = "$BEGIN_SPRITE_INTERACTIONS_DEF";
        const std::string LOADING_END_STRING = "$END_SPRITE_INTERACTIONS_DEF";
        const std::string SI_COMMAND_STR = "|--SpriteInteraction-->";

        // Returns the SpriteInteraction for the given material.
        [[nodiscard]] std::optional<Jit::SpriteInteraction> getSI(const JitDisplayMaterial &material);

        /*
        [[nodiscard]] static FrameDef getInteraction(const JitDisplayMaterial &material,
                                                     const Jit::SpriteInteraction &spriteInteraction,
                                                     Jit::JitDisplayData &dis,
                                                     JitFrameDefMap &frameDefs,
                                                     const flat::IntegerCoordinate &cord,
                                                     bool gettingBackground);
                                                     */

        [[nodiscard]] static std::optional<Jit::SpriteInteraction> getFullSpriteInteraction(const std::string &input);

        [[nodiscard]] static Jit::SpriteInteraction getSimpleSpriteInteraction(const std::string &input);

        [[nodiscard]] static std::string getSI_Key(const std::string &input);

        [[nodiscard]] static bool isSimpleSI(const std::string &input);

        static bool isFullyDefined(const Jit::SpriteInteraction &SI);

        static bool isMultiLine(const std::string &input);

        static std::optional<Jit::SpriteInteraction> loadSI(const std::string &input);

        std::unordered_map<std::string, Jit::SpriteInteraction> SI_list;
    };
}

#endif //WELT_SPRITEINTERACTIONSLIST_H
