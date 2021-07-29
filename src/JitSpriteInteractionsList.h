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
    const unsigned int CACHE_SIZE = 5;

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

        FrameDef &getFrameDef(const std::string &name, JitFrameDefMap &frameDefs, Jit::FrameDef &errorFrame);

    private:
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
