#ifndef JITTERLIB_TILESETVIEWERMAIN_JITITEXT_H
#define JITTERLIB_TILESETVIEWERMAIN_JITITEXT_H

#include <SDL.h>
#include <FlatTypes.h>

class JitIText {
public:
    JitIText(SDL_Texture *texture, SDL_Renderer *renderer, unsigned int w, unsigned int h) :
            texture(texture), renderer(renderer), size(flat::Rectangle(w, h)) {};

    virtual ~JitIText() {
        SDL_DestroyTexture(texture);
    };

    JitIText(const JitIText &other) = delete;

    JitIText(JitIText &&other) = delete;

    JitIText& operator=(const JitIText &other) = delete;

    JitIText& operator=(JitIText &&other) = delete;

    virtual void renderAt(const flat::IntegerCoordinate &coordinate, long double currentTime, SDL_Color &color) = 0;

    virtual void fillSpace(const flat::IntegerCoordinate &coordinate, SDL_Color color) = 0;

    virtual void renderWithin(const flat::CaR &area, long double currentTime) = 0;

    [[nodiscard]] virtual bool isTextureLoaded() const = 0;

    virtual void free() = 0;

    virtual flat::Rectangle getSize() const = 0;

protected:
    SDL_Texture *texture;
    SDL_Renderer *renderer;
    flat::Rectangle size;
};

#endif //JITTERLIB_TILESETVIEWERMAIN_JITITEXT_H
