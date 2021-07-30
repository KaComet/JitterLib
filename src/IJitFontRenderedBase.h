#ifndef JITTERLIB_TILESETVIEWERMAIN_IJITFONTRENDEREDBASE_H
#define JITTERLIB_TILESETVIEWERMAIN_IJITFONTRENDEREDBASE_H

#include <SDL.h>
#include <FlatTypes.h>

class IJitFontRenderedBase {
public:
    IJitFontRenderedBase(SDL_Texture *texture, SDL_Renderer *renderer, unsigned int w, unsigned int h) :
            texture(texture), renderer(renderer), size(flat::Rectangle(w, h)) {};

    virtual ~IJitFontRenderedBase() = default;

    virtual void renderAt(const flat::IntegerCoordinate &coordinate, long double currentTime) = 0;

    virtual void renderWithin(const flat::CaR &area, long double currentTime) = 0;

    [[nodiscard]] virtual bool isTextureLoaded() const = 0;

    virtual void free() = 0;

    virtual flat::Rectangle getSize() const = 0;

protected:
    SDL_Texture *texture;
    SDL_Renderer *renderer;
    flat::Rectangle size;
};


#endif //JITTERLIB_TILESETVIEWERMAIN_IJITFONTRENDEREDBASE_H
