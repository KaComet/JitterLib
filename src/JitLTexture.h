#ifndef LTEXTURE_H
#define LTEXTURE_H

#include <SDL.h>
#include <string>
#include <SDL_image.h>
#include <iostream>
#include <optional>

namespace Jit {
    class JitLTexture {
    public:
        JitLTexture();

        ~JitLTexture();

        JitLTexture(const JitLTexture& other); // Copy constructor

        JitLTexture& operator=(const JitLTexture& other); // Copy assignment

        //Loads image at specified path
        bool loadFromFile(const std::string &path);

        //Renders texture at given point
        void render(int x, int y) const;

        void renderPortion(SDL_Rect &portion, SDL_Rect &renderSection, double angle = 0.00, SDL_RendererFlip flip = SDL_FLIP_NONE) const;

        //Gets image dimensions
        [[nodiscard]] int getWidth() const;

        [[nodiscard]] int getHeight() const;

        [[nodiscard]] SDL_Color getModColor() const;

        bool setRenderer(SDL_Renderer *renderer);

        void setModColor(const SDL_Color &color);

    private:
        //Deallocates texture
        void free();

        void LoadFromSurface(const std::string& path);

        //The actual hardware texture
        SDL_Texture *mTexture;
        SDL_Surface *mSurface;
        SDL_Renderer *mRenderer;

        //Image dimensions
        int mWidth;
        int mHeight;
    };
}

#endif