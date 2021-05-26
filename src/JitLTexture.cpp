#include "JitLTexture.h"

namespace Jit {
    JitLTexture::JitLTexture() {
        //Initialize
        mTexture = nullptr;
        mRenderer = nullptr;
        mSurface = nullptr;
        mWidth = 0;
        mHeight = 0;
    }

    JitLTexture::~JitLTexture() {
        //Deallocate the JitSpriteSet
        free();
    }

    bool JitLTexture::loadFromFile(const std::string &path) {
        //Get rid of preexisting texture
        free();

        //Load image at specified path
        mSurface = IMG_Load(path.c_str());

        if (mSurface == nullptr) {
            printf("Unable to load image %s! SDL_image Error: %s\n", path.c_str(), IMG_GetError());
            return false;
        } else {
            LoadFromSurface(path);
        }

        //Return success
        return true;
    }

    void JitLTexture::free() {
        //Free texture if it exists
        if (mTexture) {
            SDL_DestroyTexture(mTexture);
            mTexture = nullptr;
            mWidth = 0;
            mHeight = 0;
        }

        if (mSurface)
            SDL_FreeSurface(mSurface);
    }

    void JitLTexture::render(int x, int y) const {
        //Set rendering space and render to screen
        SDL_Rect renderRectangle = {x, y, mWidth, mHeight};
        SDL_RenderCopy(mRenderer, mTexture, nullptr, &renderRectangle);
    }

    int JitLTexture::getWidth() const {
        return mWidth;
    }

    int JitLTexture::getHeight() const {
        return mHeight;
    }

// Returns the current color mod of the texture.
// If there is no texture loaded/no color mod, the function returns white (255, 255, 255.)
    SDL_Color JitLTexture::getModColor() const {
        // If there is no texture currently loaded, return white.
        if (!mTexture)
            return SDL_Color{0xFF, 0xFF, 0xFF};

        // Get the current color mod. If SDL_GetTextureColorMod() returns anything
        //   besides zero, this indicates an error. In this case, return white.
        Uint8 red, blue, green;
        if (SDL_GetTextureColorMod(mTexture, &red, &green, &blue) == 0)
            return SDL_Color{red, green, blue};
        else
            return SDL_Color{0xFF, 0xFF, 0xFF};
    }


    bool JitLTexture::setRenderer(SDL_Renderer *renderer) {
        if (renderer == nullptr) {
            return false;
        }

        mRenderer = renderer;

        return true;
    }

    void JitLTexture::setModColor(const SDL_Color &color) {
        //Modulate texture
        SDL_SetTextureColorMod(mTexture, color.r, color.g, color.b);
    }

    void JitLTexture::renderPortion(SDL_Rect &portion, SDL_Rect &renderSection, const double angle,
                                    const SDL_RendererFlip flip) const {
        SDL_RenderCopyEx(mRenderer, mTexture, &portion, &renderSection, angle, nullptr, flip);
    }

    JitLTexture::JitLTexture(const JitLTexture &other) {
        if (this != &other) {
            if (other.mSurface) {
                mRenderer = other.mRenderer;
                mSurface = SDL_ConvertSurface(other.mSurface, other.mSurface->format, 0);
                LoadFromSurface("COPY:OTHER_TEXTURE");
            }
        }
    }

    void JitLTexture::LoadFromSurface(const std::string &path) {
        //Color key image
        SDL_SetColorKey(mSurface, SDL_TRUE, SDL_MapRGB(mSurface->format, 0, 0, 0));

        //Create texture from surface pixels
        mTexture = SDL_CreateTextureFromSurface(mRenderer, mSurface);
        if (mTexture == nullptr) {
            printf("Unable to create texture from %s! SDL Error: %s\n", path.c_str(), SDL_GetError());
        } else {
            //Get image dimensions
            mWidth = mSurface->w;
            mHeight = mSurface->h;
        }
    }

    JitLTexture &JitLTexture::operator=(const JitLTexture &other) {
        if (this == &other)
            return *this;

        //free();

        if (other.mSurface) {
            mSurface = SDL_ConvertSurface(other.mSurface, other.mSurface->format, 0);
            LoadFromSurface("COPY:OTHER_TEXTURE");
            mRenderer = other.mRenderer;
        } else {
            mTexture = nullptr;
            mRenderer = nullptr;
            mSurface = nullptr;
        }

        return *this;
    }
}