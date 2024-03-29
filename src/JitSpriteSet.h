#ifndef WELT_P_C_SPRITESET_H
#define WELT_P_C_SPRITESET_H

#include <SDL.h>
#include <string>
#include <memory>
#include <filesystem>
#include "JitLTexture.h"
#include "JitResource.h"
#include <SDL_image.h>
#include <optional>
#include "JitUIBasics.h"

namespace Jit {
    class JitSpriteSet {
    public:
        JitSpriteSet();

        ~JitSpriteSet();

        // Loads image at specified path. Returns the number of tiles loaded.
        bool loadFromFile(const std::filesystem::path &path, uint tileWidth, uint tileHeight);

        // Renders texture at given point.
        bool render(unsigned int x, unsigned int y, Jit::FrameID valueToDisplay, double rotation = 0.00, SDL_RendererFlip flip = SDL_FLIP_NONE) const;

        // Renders texture at given point in the given color.
        bool render(unsigned int x, unsigned int y, Jit::FrameID frameToDisplay, SDL_Color renderColor);

        [[nodiscard]] uint getFontWidth() const;

        [[nodiscard]] uint getFontHeight() const;

        void setColor(const SDL_Color &color);

        [[nodiscard]] uint getNTiles() const;

        void setRenderer(SDL_Renderer *renderer);

    private:
        //Deallocates texture
        void free();

        bool load();

        //The font texture
        JitLTexture mBitmap;
        uint fontWidth = 0, fontHeight = 0, nTiles = 0;
    };
}

#endif //WELT_P_C_SPRITESET_H
