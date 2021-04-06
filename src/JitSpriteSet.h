#ifndef WELT_P_C_SPRITESET_H
#define WELT_P_C_SPRITESET_H

#include <SDL.h>
#include <string>
#include "JitLTexture.h"
#include "JitResource.h"
#include <SDL_image.h>
#include "JitUIBasics.h"

namespace Jit {
    class JitSpriteSet {
    public:
        JitSpriteSet();

        ~JitSpriteSet();

        // Loads image at specified path. Returns the number of tiles loaded.
        bool loadFromFile(SDL_Renderer *renderer, const std::string &path, uint tileWidth, uint tileHeight);

        // Renders texture at given point.
        bool render(unsigned int x, unsigned int y, Jit::FrameID valueToDisplay) const;

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

        //The font texture
        JitLTexture *mBitmap;
        uint fontWidth, fontHeight, nTiles;
    };
}

#endif //WELT_P_C_SPRITESET_H
