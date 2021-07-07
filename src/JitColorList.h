#ifndef WELT_COLORLIST_H
#define WELT_COLORLIST_H

#include <SDL.h>
#include <string>
#include <cctype>
#include <iostream>
#include <fstream>
#include <algorithm>
#include <optional>
#include "JitResource.h"
#include <map>
#include <HappyArray.h>

namespace Jit {
    struct ColorDef {
        ColorDef() : colors(flat::HappyArray<SDL_Color>(0)) {
            colorsPerSecond = 0;
            nColors = 0;
            doubleColorsPerSecondCached = 0;
        }

        explicit ColorDef(unsigned int i) : colors(flat::HappyArray<SDL_Color>(i)) {
            colorsPerSecond = 0;
            nColors = i;
            doubleColorsPerSecondCached = (long double) i;
        }

        SDL_Color &operator[](unsigned int i) const {
            if (i >= nColors)
                throw std::out_of_range("");

            return colors[i];
        }

        [[nodiscard]] SDL_Color getFromTime(long double secondsCounter, long double offset) const {
            unsigned int i;
            if (colorsPerSecond > 0)
                i = (unsigned int) ((secondsCounter + offset) * colorsPerSecond) % nColors;
            else
                i = (unsigned int) offset % nColors;
            if (i >= nColors)
                throw std::out_of_range("");

            return colors[i];
        }

        flat::HappyArray<SDL_Color> colors;
        std::string name;
        unsigned int nColors;
        double colorsPerSecond;

    private:
        long double doubleColorsPerSecondCached;
    };

    class JitColorList {
    public:
        bool loadColorsFromFile(const std::string &fileName, const std::string &tag);

        std::optional<ColorDef> get(const std::string &string);

    private:
        static SDL_Color getSDL_ColorFromCSS_Color(const std::string &input);

        static bool stringIsHex(const std::string &input);

        static bool isMultiLine(const std::string &input);

        [[nodiscard]] static std::string getColor_Key(const std::string &input);

        // Get the FPS from a given string. Returns nothing if the FPS couldn't be found.
        static std::optional<double> getCPS(const std::string &input);

        static std::optional<ColorDef> getColors(const std::string &input);

        std::unordered_map<std::string, ColorDef> colorDefinitions;
    };
}


#endif //WELT_COLORLIST_H
