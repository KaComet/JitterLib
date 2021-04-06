#include "JitSpriteSet.h"

namespace Jit {
    JitSpriteSet::JitSpriteSet() {
        //Initialize variables
        mBitmap = nullptr;
        nTiles = 0;
        fontWidth = 0;
        fontHeight = 0;
    }

    JitSpriteSet::~JitSpriteSet() {
        // Free any loaded textures.
        this->free();
    }

    bool JitSpriteSet::loadFromFile(SDL_Renderer *renderer, const std::string &path, uint tileWidth, uint tileHeight) {
        // Check if all the provided pointers are valid. If not, return false.
        if (!renderer || path.empty())
            return false;

        printf("Loading tiles...\n");


        // Free any previously used resources.
        free();

        // Create the texture, set its renderer, and load the JitSpriteSet from the given path.
        mBitmap = new JitLTexture;
        mBitmap->setRenderer(renderer);
        if (!(mBitmap->loadFromFile(path))) {
            free();
            return false;
        }

        // If the JitSpriteSet is not formatted correctly, free and return zero;
        if (((mBitmap->getHeight() % tileHeight) != 0) || ((mBitmap->getWidth() % tileWidth) != 0)) {
            std::cout << std::endl << "Bad dem. Source height % tile Height = "
                      << mBitmap->getHeight() % tileHeight << " Source width % tile width = "
                      << mBitmap->getWidth() % tileWidth << std::endl;
            free();
            return false;
        }

        // Set the width, height, and number of tiles.
        fontWidth = tileWidth;
        fontHeight = tileHeight;
        nTiles = ((mBitmap->getWidth() / tileWidth) * (mBitmap->getHeight() / tileHeight));

        printf("   Loaded %u tiles\n", nTiles);

        // Return the number of tiles loaded.
        return true;
    }

// Frees any currently loaded resources.
    void JitSpriteSet::free() {
        // Delete any loaded JitSpriteSet and set the number of tiles to zero.
        delete mBitmap;
        mBitmap = nullptr;
        nTiles = 0;
    }

// Renders the specified sprite. If the specified sprite is not loaded, the function returns false.
    bool JitSpriteSet::render(unsigned int x, unsigned int y, Jit::FrameID valueToDisplay) const {
        // If the JitSpriteSet is not loaded, return false.
        if ((mBitmap == nullptr) || (valueToDisplay >= nTiles))
            return false;

        // Calculate the specified tiles position in the JitSpriteSet.
        unsigned int xPos, yPos, nTilesPerRow;
        nTilesPerRow = mBitmap->getWidth() / fontWidth;
        xPos = (valueToDisplay % nTilesPerRow) * fontWidth;
        yPos = (valueToDisplay / nTilesPerRow) * fontHeight;

        // The portion of the JitSpriteSet image to render.
        SDL_Rect mask;
        mask.x = (int) xPos;
        mask.y = (int) yPos;
        mask.h = fontHeight;
        mask.w = fontWidth;

        // The portion of the renderer to write to.
        SDL_Rect limit;
        limit.h = fontHeight;
        limit.w = fontWidth;
        limit.x = (int) x;
        limit.y = (int) y;

        // Render the sprite to the screen.
        mBitmap->renderPortion(mask, limit);

        return true;
    }

// Renders the specified frame in the given color. If the specified sprite is not loaded, the function returns false.
    bool JitSpriteSet::render(unsigned int x, unsigned int y, Jit::FrameID frameToDisplay, SDL_Color renderColor) {
        if (!mBitmap)
            return false;

        // Store the old color mod so it can be restored after rendering.
        const SDL_Color oldColorMod = mBitmap->getModColor();

        // Set the texture's color mod to the new color mod.
        setColor(renderColor);

        // Render the texture, storing whether or not rendering was successful.
        const bool wasRenderSuccessful = this->render(x, y, frameToDisplay);

        // Reset the texture's color mod to it's old value.
        mBitmap->setModColor(oldColorMod);

        // Return whether or not rendering was successful.
        return wasRenderSuccessful;
    }

    uint JitSpriteSet::getFontWidth() const {
        return fontWidth;
    }

    uint JitSpriteSet::getFontHeight() const {
        return fontHeight;
    }

    void JitSpriteSet::setColor(const SDL_Color &color) {
        if (!mBitmap)
            return;

        mBitmap->setModColor(color);
    }

    uint JitSpriteSet::getNTiles() const {
        return nTiles;
    }

    void JitSpriteSet::setRenderer(SDL_Renderer *renderer) {
        mBitmap->setRenderer(renderer);
    }
}
