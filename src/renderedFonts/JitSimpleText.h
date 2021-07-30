#ifndef JITTERLIB_TILESETVIEWERMAIN_JITSIMPLETEXT_H
#define JITTERLIB_TILESETVIEWERMAIN_JITSIMPLETEXT_H

#include "../IJitFontRenderedBase.h"

class JitSimpleText : public IJitFontRenderedBase {
public:
    JitSimpleText() = delete;

    JitSimpleText(SDL_Texture *texture, SDL_Renderer *renderer, unsigned int w, unsigned int h) :
            IJitFontRenderedBase(texture, renderer, w, h) {
        isLoaded = texture;
    };

    ~JitSimpleText() override = default;

    void renderAt(const flat::IntegerCoordinate &coordinate, long double currentTime) override;

    void renderWithin(const flat::CaR &area, long double currentTime) override;

    [[nodiscard]] bool isTextureLoaded() const override;

    void free() override;

    [[nodiscard]] flat::Rectangle getSize() const override;

private:
    bool isLoaded;
};


#endif //JITTERLIB_TILESETVIEWERMAIN_JITSIMPLETEXT_H
