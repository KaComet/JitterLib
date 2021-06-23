#include "JitBasics.h"

namespace Jit {
    void clearRenderer(SDL_Renderer *renderer, SDL_Color clearColor, std::optional<SDL_Rect *> rect) {
        // Ensure that the renderer pointer is valid.
        if (renderer) {
            SDL_RenderClear(renderer);

            // Save the old renderer draw color.
            SDL_Color tmp;
            SDL_GetRenderDrawColor(renderer, &tmp.r, &tmp.g, &tmp.b, &tmp.a);

            // Set the clear color
            SDL_SetRenderDrawColor(renderer, clearColor.r, clearColor.g, clearColor.b, clearColor.a);

            // If a specific area has been given, clear that area. If not, clear the entire renderer.
            if (rect.has_value())
                SDL_RenderFillRect(renderer, rect.value());
            else
                SDL_RenderFillRect(renderer, nullptr);

            // Set the renderer draw color to the original color.
            SDL_SetRenderDrawColor(renderer, tmp.r, tmp.g, tmp.b, tmp.a);

        }
    }
}