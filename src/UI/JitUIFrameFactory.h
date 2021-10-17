#ifndef JITTERLIB_TILESETVIEWERMAIN_JITUIFRAMEFACTORY_H
#define JITTERLIB_TILESETVIEWERMAIN_JITUIFRAMEFACTORY_H

#include <SDL.h>
#include <optional>
#include <memory>

#include "JitUILabeledFrame.h"

class JitUIFrameFactory {
public:
    void LoadRenderer(SDL_Renderer *render);

    //std::optional<std::unique_ptr<JitUILabeledFrame>> makeFrame();
};


#endif //JITTERLIB_TILESETVIEWERMAIN_JITUIFRAMEFACTORY_H
