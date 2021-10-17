#include "JitSimpleText.h"

void JitSimpleText::renderAt(const flat::IntegerCoordinate &coordinate, long double currentTime, SDL_Color &color) {
    if ((texture != nullptr) && (renderer != nullptr)) {
        SDL_SetTextureColorMod(texture, color.r, color.g, color.b);
        SDL_Rect renderRectangle = {(int) coordinate.x, (int) coordinate.y, (int) size.width, (int) size.height};
        SDL_RenderCopy(renderer, texture, nullptr, &renderRectangle);
    }
}

void JitSimpleText::fillSpace(const flat::IntegerCoordinate &coordinate, SDL_Color color) {
    if (renderer) {
        SDL_Rect area = SDL_Rect{(int)coordinate.x, (int)coordinate.y, (int)size.width, (int)size.height};
        SDL_SetRenderDrawColor(renderer, color.r,color.g,color.b,color.a);
        SDL_RenderFillRect(renderer, &area);
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
