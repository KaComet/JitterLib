#include "JitUIBasics.h"

namespace walt::UI {
// Closes any SDL2/SDL2_Image resources. Call before exiting the application.
    void SDL_Close(WindowInstance &windowInstance) {
        //Destroy window
        SDL_DestroyRenderer(windowInstance.renderer);
        SDL_DestroyWindow(windowInstance.window);
        windowInstance.renderer = nullptr;
        windowInstance.window = nullptr;

        //Quit SDL subsystems
        IMG_Quit();
        SDL_Quit();
    }

// Initializes SDL2, creates a window, and creates a renderer.
// Call before using any SDL2 resources.
    bool SLD_Init(WindowInstance &windowInstance, int width, int height) {
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
            windowInstance.window = SDL_CreateWindow("WaLT Engine Demo", SDL_WINDOWPOS_UNDEFINED,
                                                     SDL_WINDOWPOS_UNDEFINED,
                                                     width,
                                                     height, SDL_WINDOW_SHOWN);
            if (windowInstance.window == nullptr) {
                printf("!Window could not be created. SDL Error: %s\n !", SDL_GetError());
                success = false;
            } else {
                //Create renderer for window
                windowInstance.renderer = SDL_CreateRenderer(windowInstance.window, -1, SDL_RENDERER_ACCELERATED);
                if (windowInstance.renderer == nullptr) {
                    printf("!Renderer could not be created. SDL Error: %s\n !", SDL_GetError());
                    success = false;
                } else {
                    //Initialize renderer color
                    SDL_SetRenderDrawColor(windowInstance.renderer, 0xFF, 0xFF, 0xFF, 0xFF);

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
}