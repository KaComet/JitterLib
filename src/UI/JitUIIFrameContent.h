#ifndef JITTERLIB_TILESETVIEWERMAIN_JITUIIFRAMECONTENT_H
#define JITTERLIB_TILESETVIEWERMAIN_JITUIIFRAMECONTENT_H

#include "../JitSpriteSet.h"
#include "../JitSpriteInteractionsList.h"
#include "../JitFrameDefMap.h"
#include "../JitColorList.h"
#include <FlatTypes.h>

namespace Jit {
    class JitUIIFrameContent {
    public:
        virtual ~JitUIIFrameContent() = default;

        virtual void render(Jit::JitSpriteSet &spriteSet, JitSpriteInteractionsList &SIs, JitFrameDefMap &frameDefMap,
                            JitColorList &colors, double time, flat::CoordinateAndRectangle area) = 0;
    };
}


#endif //JITTERLIB_TILESETVIEWERMAIN_JITUIIFRAMECONTENT_H
