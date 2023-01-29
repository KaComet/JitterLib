#include <string>
#include <memory>
#include <filesystem>
#include <SDL.h>
#include <SDL_ttf.h>
#include <FlatTypes.h>

#include "../src/JitColorList.h"
#include "../src/JitFrameDefMap.h"
#include "../src/JitSpriteInteractionsList.h"
#include "../src/JitFontTextureFactory.h"
#include "../src/UI/JitUIFrameFactory.h"
#include "../src/UI/JitUILabeledFrame.h"
#include "../src/JitIText.h"
#include "../src/renderedFonts/JitSimpleText.h"

std::string path = "Examples/resource/basic/TexturePackData.WDF";
std::string pathSprites = "Examples/resource/basic/tileset.png";
std::string fontPath = "Examples/resource/basic/luculent.ttf";
std::string uiTag = "ui";

const int SCREEN_WIDTH = 800;
const int SCREEN_HEIGHT = 400;
const int SCREEN_TICKS_PER_FRAME = 5;

// The window we'll be rendering to
SDL_Window *gWindow = nullptr;

// The window renderer
SDL_Renderer *gRenderer = nullptr;

bool init(int windowWidth, int windowHeight);

int main(int argc, char **argv) {
    // Set up the resource folder path.
    std::filesystem::path resourceFolderPath = std::filesystem::path{argv[0]};
    resourceFolderPath = resourceFolderPath.remove_filename().parent_path().parent_path();

    init(SCREEN_WIDTH, SCREEN_HEIGHT);

    Jit::JitColorList colors;
    Jit::JitFrameDefMap frameDefMap;
    Jit::JitSpriteInteractionsList spriteInteractionsList;
    std::shared_ptr<Jit::JitFontTextureFactory> fontTextureFactory = std::make_shared<Jit::JitFontTextureFactory>();
    Jit::JitSpriteSet spriteSet;

    spriteSet.setRenderer(gRenderer);
    spriteSet.loadFromFile(resourceFolderPath / pathSprites, 15, 15);
    colors.loadColorsFromFile(resourceFolderPath / path, uiTag);
    frameDefMap.loadFrameDefs(resourceFolderPath / path);
    spriteInteractionsList.loadSpriteInteractions(resourceFolderPath / path, uiTag);
    fontTextureFactory->loadFont(resourceFolderPath / fontPath, 11);
    fontTextureFactory->loadRenderer(gRenderer);

    Jit::FrameData frameData;
    frameData.area = flat::CoordinateAndRectangle(5, 10, 10, 5);
    frameData.colorName = "border";
    frameData.SIName = "border split";
    frameData.renderer = gRenderer;
    frameData.blankingColor = SDL_Color{0,0,0,255};
    Jit::JitUILabeledFrame frame = Jit::JitUILabeledFrame(frameData, fontTextureFactory, "# Hello! #");


    int i = 0;
    while (1) {
        frame.setLabelText("# Hello! Frame " + std::to_string(i++) + " #");
        frame.clearFrameArea(spriteSet, SDL_Color{0, 0, 0, 255});
        frame.render(spriteSet, spriteInteractionsList, frameDefMap, colors, 0.0);

        SDL_RenderPresent(gRenderer);
        SDL_Delay(50);
    }

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