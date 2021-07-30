#include "JitFontTextureFactory.h"

namespace Jit {
    bool JitFontTextureFactory::loadFont(const std::string &path, unsigned int sizePts) {
        // Determine the path of the resource.
        const std::string realPath = getResourcePath("") + path;

        // Attempt to load the font at path from file system.
        font = TTF_OpenFont(realPath.c_str(), (int) sizePts);

        // Check if there was an error while loading.
        if (font == nullptr) {
            printf("JitFontTextureFactory: Failed to load ttf! SDL_ttf Error: %s\n", TTF_GetError());
            return false;
        }

        return true;
    }

    bool Jit::JitFontTextureFactory::loadRenderer(SDL_Renderer *render) {
        if (render) {
            renderer = render;
            return true;
        }

        return false;
    }


    JitFontTextureFactory::JitFontTextureFactory() : font(nullptr), renderer(nullptr) {

    }

    std::optional<std::unique_ptr<IJitFontRenderedBase>> JitFontTextureFactory::makeSimpleText(const std::string &text, SDL_Color color) {
        std::optional<std::unique_ptr<IJitFontRenderedBase>> result = std::nullopt;
        SDL_Texture *texture = nullptr;
        unsigned int w = 0, h = 0;

        SDL_Surface *textSurface = TTF_RenderText_Solid(font, text.c_str(), color);
        if (textSurface == nullptr) {
            printf("JitFontTextureFactory: Unable to render text surface! SDL_ttf Error: %s\n", TTF_GetError());
        } else {
            //Create texture from surface pixels
            texture = SDL_CreateTextureFromSurface(renderer, textSurface);
            if (texture == nullptr) {
                printf("JitFontTextureFactory: Unable to create texture from rendered text! SDL Error: %s\n",
                       SDL_GetError());
            } else {
                //Get image dimensions
                w = textSurface->w;
                h = textSurface->h;

                result = std::optional<std::unique_ptr<IJitFontRenderedBase>>(std::make_unique<JitSimpleText>(texture, renderer, w, h));
            }

            //Get rid of old surface
            SDL_FreeSurface(textSurface);
        }

        return result;
    }
}
