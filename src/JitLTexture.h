#ifndef LTEXTURE_H
#define LTEXTURE_H

#include <SDL.h>
#include <string>
#include <SDL_image.h>
#include <iostream>
namespace Jit {
    class JitLTexture {
    public:
        JitLTexture();

        ~JitLTexture();

        //Loads image at specified path
        bool loadFromFile(const std::string &path);

        //Renders texture at given point
        void render(int x, int y) const;

        void renderPortion(SDL_Rect &portion, SDL_Rect &renderSection) const;

        //Gets image dimensions
        [[nodiscard]] int getWidth() const;

        [[nodiscard]] int getHeight() const;

        [[nodiscard]] SDL_Color getModColor() const;

        bool setRenderer(SDL_Renderer *renderer);

        void setModColor(const SDL_Color &color);

    private:
        //Deallocates texture
        void free();

        //The actual hardware texture
        SDL_Texture *mTexture;
        SDL_Renderer *mRenderer;

        //Image dimensions
        int mWidth;
        int mHeight;
    };
}

#endif