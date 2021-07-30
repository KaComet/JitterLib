#include "JitSimpleText.h"

void JitSimpleText::renderAt(const flat::IntegerCoordinate &coordinate, long double currentTime) {
    if ((texture != nullptr) && (renderer != nullptr)) {
        SDL_Rect renderRectangle = {(int) coordinate.x, (int) coordinate.y, (int) size.width, (int) size.height};
        SDL_RenderCopy(renderer, texture, nullptr, &renderRectangle);
    }
}

void JitSimpleText::renderWithin(const flat::CaR &area, long double currentTime) {
    if ((texture != nullptr) && (renderer != nullptr)) {
        SDL_Rect renderRectangle = {(int) area.x, (int) area.y, (int) area.width, (int) area.height};
        SDL_RenderCopy(renderer, texture, nullptr, &renderRectangle);
    }
}

bool JitSimpleText::isTextureLoaded() const {
    return isLoaded;
}

void JitSimpleText::free() {
    //Free texture if it exists
    if (texture) {
        SDL_DestroyTexture(texture);
        texture = nullptr;
        isLoaded = false;
    }
}

flat::Rectangle JitSimpleText::getSize() const {
    return size;
}
