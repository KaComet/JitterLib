#ifndef SHEER_UIBASICS_H
#define SHEER_UIBASICS_H

#include <string>
#include <SDL.h>
#include <SDL_image.h>

namespace Jit {
    // For specifying a char/pic that is used to display an object.
    typedef unsigned int FrameID;

/* Contains all the displayIDs for every possible
 *   state of a tile. Used to allow for tiles to
 *   connect (like wall or fences) or other
 *   "fancy" effects. */
    struct SpriteInteraction {
        std::string name,
                dDefault,
                W,
                E,
                EW,
                S,
                SW,
                SE,
                SEW,
                N,
                NW,
                NE,
                NEW,
                NS,
                NSW,
                NSE,
                NSEW;
    };

    struct WindowInstance {
        WindowInstance() : window(nullptr), renderer(nullptr) {}

        SDL_Window *window;
        SDL_Renderer *renderer;
    };

    // Closes any SDL2/SDL2_Image resources. Call before exiting the application.
    void SDL_Close(WindowInstance &windowInstance);

    // Initializes SDL2, creates a window, and creates a renderer.
    // Call before using any SDL2 resources.
    bool SLD_Init(WindowInstance &windowInstance, int width, int height);
}


#endif //SHEER_UIBASICS_H
