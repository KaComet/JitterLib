#include <iostream>
#include <SDL.h>
#include <string>
#include "../src/JitSpriteSet.h"
#include "../src/JitLTimer.h"
#include <SDL_image.h>

// An example path would be "../resource/basic/tileset.png"

//TODO: Make texture files define their own parameters.
const int TILE_WIDTH = 15;
const int TILE_HEIGHT = 15;
const int ROW_WIDTH = 32;
const int SCREEN_TICKS_PER_FRAME = 5;

// The window we'll be rendering to
SDL_Window *gWindow = nullptr;

// The window renderer
SDL_Renderer *gRenderer = nullptr;

bool init(int windowWidth, int windowHeight);

bool loadSpriteSetFromFile(SDL_Renderer *renderer, Jit::JitSpriteSet &spriteSet, const std::string &fileName, bool usingSimpleResourcePath);

void printTileNumber(int x, int y, int tileWidth, int tileHeight, int tilesPerRow);

int main(int argc, char *argv[]) {
    std::string path;
    int windowWidth = 10, windowHeight = 10;
    Jit::JitSpriteSet sprites;

    /* Get the args. The last arg will be the
     *   resource path to the spriteset. */
    for (unsigned int i = 1; i < argc; i++) {
        path = argv[i];
    }

    // If the path is blank, exit.
    if (path.empty()) {
        std::cout << "Path is empty.\n";
        return 0;
    }

    // In order to load the spriteset, a window must be made first.
    init(windowWidth, windowHeight);

    // If the file could not be opened, exit.
    if (!loadSpriteSetFromFile(gRenderer, sprites, path, false)) {
        std::cout << "Could not open file.\n";
        return 0;
    }

    // Give the renderer to the spriteset
    sprites.setRenderer(gRenderer);

    // Calculate the required dimensions of the window.
    int nTiles = sprites.getNTiles();
    int heightInTiles = nTiles / ROW_WIDTH;
    if (nTiles % ROW_WIDTH)
        heightInTiles++;

    windowWidth = ROW_WIDTH * TILE_WIDTH;
    windowHeight = heightInTiles * TILE_HEIGHT;

    // Resize the window to fit all the tiles.
    SDL_SetWindowSize(gWindow, windowWidth, windowHeight);

    SDL_Event eventH;
    SDL_Rect screenRect;
    screenRect.x = 0;
    screenRect.y = 0;
    screenRect.h = windowHeight;
    screenRect.w = windowWidth;

    // Create a timer to regulate the FPS. Without this, the application
    //   will render as fast as possible. This would waste system resources.
    Jit::JitLTimer fpsReg;

    // Main loop flag
    bool quit = false;
    while (!quit) {
        // Reset the frame timer.
        fpsReg.start();

        // If there are events in the que, try to process them,
        while (SDL_PollEvent(&eventH) != 0) {
            switch (eventH.type) {
                case SDL_QUIT:
                    // If the user wants to quit the app, signal to exit.
                    quit = true;
                    break;
                case SDL_MOUSEBUTTONDOWN:
                    printTileNumber(eventH.motion.x, eventH.motion.y, TILE_WIDTH, TILE_HEIGHT, ROW_WIDTH);
                    break;
                default:;
            }
        }

        // Clear the renderer.
        SDL_RenderClear(gRenderer);

        // Draw the background as solid black.
        SDL_SetRenderDrawColor(gRenderer, 0, 0, 0, 0xFF);
        SDL_RenderFillRect(gRenderer, &screenRect);
        SDL_SetRenderDrawColor(gRenderer, 0xFF, 0xFF, 0xFF, 0xFF);

        // Draw all the tiles.
        for (unsigned int i = 0; i < nTiles; i++) {
            // Calculate the x and y value to draw the sprite to.
            unsigned int xPixel = (i % ROW_WIDTH) * TILE_WIDTH;
            unsigned int yPixel = (i / ROW_WIDTH) * TILE_HEIGHT;

            // Draw the sprite.
            sprites.render(xPixel, yPixel, i);
        }

        // Render the frame.
        SDL_RenderPresent(gRenderer);

        // Wait out the rest of the frame.
        int elapsedTicks = fpsReg.getTicks();
        if (elapsedTicks < SCREEN_TICKS_PER_FRAME)
            SDL_Delay((Uint32) (SCREEN_TICKS_PER_FRAME - elapsedTicks));
    }

    return 0;
}

// Initializes SDL2, creates a window, and creates a renderer.
// Call before using any SDL2 resources.
bool init(int windowWidth, int windowHeight) {
    //Initialization flag
    bool success = true;

    //Initialize SDL
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        printf("!SDL could not initialize! SDL Error: %s\n !", SDL_GetError());
        success = false;
    } else {
        //Try to set texture filtering to linear
        if (!SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "0")) {
            printf("!Linear texture filtering could not be enabled!");
        }

        //Create window
        gWindow = SDL_CreateWindow("WaLT Engine Demo", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
                                   windowWidth,
                                   windowHeight, SDL_WINDOW_SHOWN);
        if (gWindow == nullptr) {
            printf("!Window could not be created. SDL Error: %s\n !", SDL_GetError());
            success = false;
        } else {
            //Create renderer for window
            gRenderer = SDL_CreateRenderer(gWindow, -1, SDL_RENDERER_ACCELERATED);
            if (gRenderer == nullptr) {
                printf("!Renderer could not be created. SDL Error: %s\n !", SDL_GetError());
                success = false;
            } else {
                //Initialize renderer color
                SDL_SetRenderDrawColor(gRenderer, 0xFF, 0xFF, 0xFF, 0xFF);

                //Initialize PNG loading
                int imgFlags = IMG_INIT_PNG;
                if (!(IMG_Init(imgFlags) & imgFlags)) {
                    printf("!SDL_image could not initialize. SDL_image Error: %s\n !", IMG_GetError());
                    success = false;
                }
            }
        }
    }

    return success;
}

bool loadSpriteSetFromFile(SDL_Renderer *renderer, Jit::JitSpriteSet &spriteSet, const std::string &fileName, bool usingSimpleResourcePath) {
    // Check if all the provided pointers are valid. If not, return false.
    if (fileName.empty())
        return false;

    printf("Loading tiles...\n");

    // Get the path of the JitSpriteSet and load it.
    std::string path = "";
    if (usingSimpleResourcePath) {
        path = Jit::getResourcePath("") + fileName;
    } else {
        path = fileName;
    }
    const uint NTiles = spriteSet.loadFromFile(path, TILE_WIDTH, TILE_HEIGHT);

    // Print the number of elements loaded.
    printf("   Loaded %u tiles\n", NTiles);

    // If no elements could not be loaded, return false.
    return NTiles != 0;
}

void printTileNumber(int x, int y, int tileWidth, int tileHeight, int tilesPerRow) {
    int tileX = x / tileWidth;
    int tileY = y / tileHeight;

    int tileNumber = (tileY * tilesPerRow) + tileX;

    std::cout << "#" << tileNumber << std::endl;
}