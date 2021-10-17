#include <string>
#include <memory>
#include <SDL.h>
#include <SDL_ttf.h>
#include <FlatTypes.h>

#include "../src/JitColorList.h"
#include "../src/JitFrameDefMap.h"
#include "../src/JitSpriteInteractionsList.h"
#include "../src/JitFontTextureFactory.h"
#include "../src/JitIText.h"
#include "../src/renderedFonts/JitSimpleText.h"

// This simple programs demonstrates loading colors, frame definitions, and sprite interactions and rendering of text.

std::string path = "Examples/resource/basic/TexturePackData.WDF";
std::string fontPath = "Examples/resource/basic/luculent.ttf";
std::string tag = "basic";

const int SCREEN_WIDTH = 400;
const int SCREEN_HEIGHT = 200;
const int SCREEN_TICKS_PER_FRAME = 5;

// The window we'll be rendering to
SDL_Window *gWindow = nullptr;

// The window renderer
SDL_Renderer *gRenderer = nullptr;

bool init(int windowWidth, int windowHeight);

int main() {
    init(SCREEN_WIDTH, SCREEN_HEIGHT);

    Jit::JitColorList colors;
    Jit::JitFrameDefMap frameDefMap;
    Jit::JitSpriteInteractionsList spriteInteractionsList;
    Jit::JitFontTextureFactory fontTextureFactory;

    fontTextureFactory.loadRenderer(gRenderer);
    fontTextureFactory.loadFont(fontPath, 10);

    std::unique_ptr<JitIText> text = fontTextureFactory.makeSimpleText("Hello! gjkq...",
                                                                                   SDL_Color{
                                                                                           0xFF,
                                                                                           0xFF,
                                                                                           0xFF,
                                                                                           0xFF}).value();
    text->renderAt(flat::IntegerCoordinate(0, 0), 0.0);

    SDL_RenderPresent(gRenderer);

    SDL_Delay(5000);

    colors.loadColorsFromFile(path, tag);
    frameDefMap.loadFrameDefs(path);
    spriteInteractionsList.loadSpriteInteractions(path, "foreground");

    text->free();
    TTF_Quit();
    IMG_Quit();
    SDL_Quit();
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

                //Initialize SDL_ttf
                if (TTF_Init() == -1) {
                    printf("SDL_ttf could not initialize! SDL_ttf Error: %s\n", TTF_GetError());
                    success = false;
                }
            }
        }
    }

    return success;
}