#ifndef JITTERLIB_TILESETVIEWERMAIN_JITUIIFRAME_H
#define JITTERLIB_TILESETVIEWERMAIN_JITUIIFRAME_H

#include <string>
#include <FlatTypes.h>
#include "../JitSpriteSet.h"
#include "../JitSpriteInteractionsList.h"
#include "../JitColorList.h"

namespace Jit {
    struct FrameData {
        flat::CoordinateAndRectangle area;
        std::string SIName, colorName;
        SDL_Renderer *renderer;
        SDL_Color blankingColor;
    };

    class JitUIIFrame {
    public:
        explicit JitUIIFrame(FrameData data) : data(std::move(data)) {};

        virtual ~JitUIIFrame() = default;

        virtual void render(Jit::JitSpriteSet &spriteSet, JitSpriteInteractionsList &SIs, JitFrameDefMap &frameDefMap,
                            JitColorList &colors, double time) = 0;

        virtual void setFrameSI(const std::string &SIName) noexcept {
            data.SIName = SIName;
        };

        virtual void setFrameColor(const std::string &colorName) noexcept {
            data.colorName = colorName;
        };

        virtual void setFrameArea(flat::CoordinateAndRectangle area) {
            if ((area.width < 2) || (area.height < 2))
                throw std::runtime_error("Frame width & height must be >2");

            data.area = area;
        }

        virtual void clearFrameArea(const JitSpriteSet &sprites, SDL_Color clearColor) {
            if (data.renderer) {
                SDL_Rect clearArea;
                clearArea.x = data.area.x;
                clearArea.y = data.area.y;
                clearArea.w = data.area.width * sprites.getFontWidth();
                clearArea.h = data.area.height * sprites.getFontHeight();

                SDL_SetRenderDrawColor(data.renderer, clearColor.r, clearColor.g, clearColor.b, clearColor.a);
                SDL_RenderFillRect(data.renderer, &clearArea);
            }
        };

        virtual void clearFrameArea(const JitSpriteSet &sprites) {
            if (data.renderer) {
                SDL_Rect clearArea;
                clearArea.x = data.area.x;
                clearArea.y = data.area.y;
                clearArea.w = data.area.width * sprites.getFontWidth();
                clearArea.h = data.area.height * sprites.getFontHeight();

                SDL_SetRenderDrawColor(data.renderer, data.blankingColor.r, data.blankingColor.g,
                                       data.blankingColor.b, data.blankingColor.a);
                SDL_RenderFillRect(data.renderer, &clearArea);
            }
        };

        virtual void setBlankingColor(SDL_Color color) noexcept {
            data.blankingColor = color;
        };

    protected:
        FrameData data;
    };
}


#endif //JITTERLIB_TILESETVIEWERMAIN_JITUIIFRAME_H
