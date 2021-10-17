#ifndef JITTERLIB_TILESETVIEWERMAIN_JITUILABELEDFRAME_H
#define JITTERLIB_TILESETVIEWERMAIN_JITUILABELEDFRAME_H

#include <FlatTypes.h>

#include <utility>
#include <SDL.h>
#include <string>
#include <stdexcept>
#include <optional>
#include <memory>
#include <cstdlib>
#include "../JitColorList.h"
#include "../JitFrameDefMap.h"
#include "../JitSpriteSet.h"
#include "../JitSpriteInteractionsList.h"
#include "../JitFontTextureFactory.h"
#include "JitUIIFrame.h"
#include "JitUIIFrameContent.h"

namespace Jit {
    class JitUILabeledFrame : public JitUIIFrame {
    public:
        explicit JitUILabeledFrame(FrameData data, std::shared_ptr<JitFontTextureFactory> fontTextureFactory,
                                   const std::string &label = "Label") :
                JitUIIFrame(std::move(data)),
                content(nullptr),
                fontTextureFactory(std::move(fontTextureFactory)) {
            setLabelText(label);
        };

        ~JitUILabeledFrame() override = default;

        void render(Jit::JitSpriteSet &spriteSet, JitSpriteInteractionsList &SIs, JitFrameDefMap &frameDefMap,
                    JitColorList &colors, double time) override;

        void setLabelText(const std::string &label);

        void setContent(std::unique_ptr<JitUIIFrameContent> content);

    private:
        std::shared_ptr<JitFontTextureFactory> fontTextureFactory;
        std::unique_ptr<JitIText> text;
        std::unique_ptr<JitUIIFrameContent> content;
    };
}


#endif //JITTERLIB_TILESETVIEWERMAIN_JITUILABELEDFRAME_H
