#include "JitUILabeledFrame.h"

void Jit::JitUILabeledFrame::render(Jit::JitSpriteSet &spriteSet, JitSpriteInteractionsList &SIs,
                                    JitFrameDefMap &frameDefMap,
                                    JitColorList &colors, double time) {
    unsigned int height = spriteSet.getFontHeight();
    unsigned int width = spriteSet.getFontWidth();
    bool drawVert = data.area.height > 2;
    bool drawHorz = data.area.width > 2;
    FrameDef &error = frameDefMap.get("undefined");
    SDL_Color color = colors.get(data.colorName).getFromTime(time, 0);
    FrameID EW_Edge = SIs.getFrameDef(data.SIName, frameDefMap, error, InteractionEnum::ccEW).getFromTime(time, 0);
    FrameID NS_Edge = SIs.getFrameDef(data.SIName, frameDefMap, error, InteractionEnum::NScc).getFromTime(time, 0);
    FrameID SE_Corner = SIs.getFrameDef(data.SIName, frameDefMap, error, InteractionEnum::cSEc).getFromTime(time, 0);
    FrameID SW_Corner = SIs.getFrameDef(data.SIName, frameDefMap, error, InteractionEnum::cScW).getFromTime(time, 0);
    FrameID NE_Corner = SIs.getFrameDef(data.SIName, frameDefMap, error, InteractionEnum::NcEc).getFromTime(time, 0);
    FrameID NW_Corner = SIs.getFrameDef(data.SIName, frameDefMap, error, InteractionEnum::NccW).getFromTime(time, 0);
    for (unsigned int y = 0; y < data.area.height; y++) {
        if (!y) {
            // Top left corner.
            spriteSet.render(data.area.x, y * height + data.area.y, SE_Corner, color);

            // Top right corner.
            spriteSet.render((data.area.width - 1) * width + data.area.x, y * height + data.area.y, SW_Corner, color);

            // Draw top edge.
            if (drawHorz) {
                for (unsigned int x = 1; x < (data.area.width - 1); x++) {
                    spriteSet.render(x * width + data.area.x, y * height + data.area.y, EW_Edge, color);
                }
            }
        } else if (y == (data.area.height - 1)) {
            // Draw bottom left corner.
            spriteSet.render(data.area.x, y * height + data.area.y, NE_Corner, color);

            // Draw bottom right corner.
            spriteSet.render((data.area.width - 1) * width + data.area.x, y * height + data.area.y, NW_Corner, color);

            // Draw bottom edge.
            if (drawHorz) {
                for (unsigned int x = 1; x < (data.area.width - 1); x++) {
                    spriteSet.render(x * width + data.area.x, y * height + data.area.y, EW_Edge, color);
                }
            }
        } else {
            // Draw section of the left and right edges.
            if (drawVert) {
                spriteSet.render(data.area.x, y * height + data.area.y, NS_Edge, color);
                spriteSet.render((data.area.width - 1) * width + data.area.x, y * height + data.area.y, NS_Edge, color);
            }
        }
    }

    int diff = ((int)text->getSize().height - (int)height) >> 1;
    unsigned int textYOffset = std::abs(diff);

    text->fillSpace(flat::IntegerCoordinate{data.area.x + (width), data.area.y + textYOffset}, data.blankingColor);
    text->renderAt(flat::IntegerCoordinate{data.area.x + (width), data.area.y + textYOffset}, time, color);

    if (content)
        content->render(spriteSet, SIs, frameDefMap, colors, time, data.area);
}

void Jit::JitUILabeledFrame::setLabelText(const std::string &label) {
    std::optional<std::unique_ptr<JitIText>> textOpt = fontTextureFactory->makeSimpleText(label);
    if (textOpt.has_value())
        text = std::move(textOpt.value());

}

void Jit::JitUILabeledFrame::setContent(std::unique_ptr<JitUIIFrameContent> frameContent) {
    content = std::move(frameContent);
}
