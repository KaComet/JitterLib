#ifndef JITTERLIB_TILESETVIEWERMAIN_JITFONTTEXTUREFACTORY_H
#define JITTERLIB_TILESETVIEWERMAIN_JITFONTTEXTUREFACTORY_H

#include <string>
#include <stdexcept>
#include <optional>
#include <memory>
#include <filesystem>
#include <SDL.h>
#include <SDL_ttf.h>

#include "JitIText.h"
#include "renderedFonts/JitSimpleText.h"
#include "JitResource.h"

namespace Jit {
    class JitFontTextureFactory {
    public:
        JitFontTextureFactory();

        bool loadFont(const std::filesystem::path &path, unsigned int sizePts);

        bool loadRenderer(SDL_Renderer *render);

        std::optional<std::unique_ptr<JitIText>> makeSimpleText(const std::string &text, SDL_Color color = SDL_Color{255,255,255,255});

    private:
        SDL_Renderer *renderer;
        TTF_Font *font;
    };
}


#endif //JITTERLIB_TILESETVIEWERMAIN_JITFONTTEXTUREFACTORY_H
