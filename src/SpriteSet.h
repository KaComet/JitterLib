#ifndef WELT_P_C_SPRITESET_H
#define WELT_P_C_SPRITESET_H

#include <SDL.h>
#include <string>
#include "LTexture.h"
#include "resource.h"
#include <SDL_image.h>
#include "UIBasics.h"


class SpriteSet {
public:
    SpriteSet();

    ~SpriteSet();

    // Loads image at specified path. Returns the number of tiles loaded.
    bool loadFromFile(SDL_Renderer *renderer, const std::string &path, uint tileWidth, uint tileHeight);

    // Renders texture at given point.
    bool render(unsigned int x, unsigned int y, walt::UI::FrameID valueToDisplay) const;

    // Renders texture at given point in the given color.
    bool render(unsigned int x, unsigned int y, walt::UI::FrameID frameToDisplay, SDL_Color renderColor);

    [[nodiscard]] uint getFontWidth() const;

    [[nodiscard]] uint getFontHeight() const;

    void setColor(const SDL_Color &color);

    [[nodiscard]] uint getNTiles() const;

    void setRenderer(SDL_Renderer *renderer);

private:
    //Deallocates texture
    void free();

    //The font texture
    LTexture *mBitmap;
    uint fontWidth, fontHeight, nTiles;
};


#endif //WELT_P_C_SPRITESET_H
