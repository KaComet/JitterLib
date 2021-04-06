#include "../src/JitColorList.h"
#include "../src/JitFrameDefMap.h"
#include "../src/JitSpriteInteractionsList.h"

std::string path = "Examples/resource/basic/TexturePackData.WDF";
std::string tag = "basic";

// This simple programs demonstrates loading colors, frame definitions, and sprite interactions.

int main() {
    Jit::JitColorList colors;
    Jit::JitFrameDefMap frameDefMap;
    Jit::JitSpriteInteractionsList spriteInteractionsList;

    colors.loadColorsFromFile(path, tag);
    frameDefMap.loadFrameDefs(path);
    spriteInteractionsList.loadSpriteInteractions(path, "foreground");
}