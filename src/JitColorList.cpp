#include "JitColorList.h"

namespace Jit {
// Loads the colors defined from the given file name into the given JitColorList.
// If the file could not be opened, the function returns false.
    bool JitColorList::loadColorsFromFile(const std::string &fileName, const std::string &tag) {
        const std::string LOADING_START_STRING = "$BEGIN_COLORS";
        const std::string LOADING_END_STRING = "$END_COLORS";
        const std::string COLOR_COMMAND_STR = "|--ColorDef-->";
        unsigned int nLoaded = 0;

        colorDefinitions.clear();

        // Determine the path of the resource.
        std::string path = getResourcePath("") + fileName;

        // If the path is empty, return false.
        if (path.empty())
            return false;

        printf("Loading %s color definitions..\n", tag.c_str());

        // Open the input file using the previously determined path.
        std::ifstream inputFile;
        inputFile.open(path);

        // If the file could be opened, start loading the file. If not, return false.
        if (inputFile.is_open()) {
            unsigned int lineNumber = 0;
            bool isLoadingEnabled = false;

            while (!inputFile.eof()) {
                bool lineLoaded = false;
                std::string currentLine;
                ColorDef newDef;

                // NOTE: line numbers start at one.
                // Load the current line.
                currentLine = "";
                do {
                    // NOTE: line numbers start at one.
                    lineNumber++;
                    std::string tmp;
                    std::getline(inputFile, tmp);
                    //lineNumber++;
                    //tmp = clearWhiteSpace(tmp);
                    currentLine.append(tmp);
                } while (isMultiLine(currentLine));

                if (currentLine.empty() || (flat::getFirstNonWhitespace(currentLine) == '@'))
                    continue;

                /* If the current line contains a LOADING_START_STRING, enable
                 *   loading. Unless, loading is already enabled. In that
                 *   case, print an error message. */
                const size_t startString = currentLine.find(LOADING_START_STRING);
                if (startString != std::string::npos) {
                    std::string thisTag;
                    try {
                        thisTag = getEncapsulatedContents(currentLine, '<', '>', 0);
                    } catch (...) {
                        thisTag = "";
                    }

                    if (thisTag == tag) {
                        if (isLoadingEnabled) {
                            printf("   * Line %u is attempting to load to %s, while %s is being loaded.\n", lineNumber,
                                   thisTag.c_str(), tag.c_str());
                        } else {
                            isLoadingEnabled = true;
                            continue;
                        }
                    }
                }

                /* If the current line contains a LOADING_END_STRING, disable
                 *   loading. Unless, loading is already disabled. In that
                 *   case, print an error message and skip this line. */
                if (currentLine == LOADING_END_STRING) {
                    if (isLoadingEnabled) {
                        isLoadingEnabled = false;
                        continue;
                    } else {
                        //printf("   * Line %u is not formatted correctly. Contains %s even though a %s hasn't been declared earlier in the file. Skipping.\n",
                        //       lineNumber, LOADING_END_STRING.c_str(), LOADING_START_STRING.c_str());
                        continue;
                    }
                }

                if (isLoadingEnabled) {
                    /* If the current line contains a LOADING_START_STRING, enable
                     *   loading. Unless, loading is already enabled. In that
                     *   case, print an error message. */
                    const size_t startString = currentLine.find(LOADING_START_STRING);
                    if (startString != std::string::npos) {
                        std::string thisTag;
                        try {
                            thisTag = getEncapsulatedContents(currentLine, '<', '>', 0);
                        } catch (...) {
                            thisTag = "";
                        }

                        if (thisTag == tag) {
                            if (isLoadingEnabled) {
                                printf("   * Line %u is attempting to load to %s, while %s is being loaded.\n",
                                       lineNumber,
                                       thisTag.c_str(), tag.c_str());
                            } else {
                                isLoadingEnabled = true;
                                continue;
                            }
                        }
                    }

                    if (isLoadingEnabled) {
                        ColorDef colorDef;

                        // Try to parse the contents of the line.
                        try {
                            std::string key = getColor_Key(currentLine);
                            auto colors = getColors(currentLine);
                            auto CPS = getCPS(currentLine);

                            if (key.empty())
                                throw std::runtime_error("");

                            colorDef.colors = colors.value().colors;
                            colorDef.nColors = colors.value().nColors;
                            colorDef.colorsPerSecond = CPS.value();
                            colorDef.name = key;
                        } catch (...) {
                            // If the line was incorrectly formatted, print an error message and skip it.
                            printf("   * Line %u is not formatted correctly. Skipping.\n", lineNumber);
                            continue;
                        }


                        /* Add the SI to storage. Check is a pair. The second
                             *   element is a bool that indicates if the insert
                             *   was successful. */
                        auto check = colorDefinitions.emplace(colorDef.name, colorDef);

                        // If the SI could not be added, print an error message.
                        if (!check.second) {
                            printf("   * Line %u is attempting to redefine an already defined SI Skipping.\n",
                                   lineNumber);
                            continue;
                        }
                        nLoaded++;
                        lineLoaded = true;
                    }
                }
            }
        } else {
            // If the file could not be loaded, notify the user and return false.
            printf("   * Could not open the file \"%s\"\n", path.

                    c_str()

            );
            return false;
        }

        // When done loading the file, print how many SIs were loaded and return true.
        printf("   Loaded %u color definitions.\n", nLoaded);

        return true;
    }

    ColorDef JitColorList::get(const std::string &name) {
        try {
            return colorDefinitions.at(name);
        } catch (std::out_of_range &e) {
            throw std::runtime_error("Couldn't find color <" + name + ">.");
        }
    }

    bool JitColorList::checkIfContains(const std::string &name) {
        try {
            colorDefinitions.at(name);
            return true;
        } catch (std::out_of_range &e) {
            return false;
        }
    }

// Converts a CSS color formatted string to SDL_Color.
// Returns the value provided for ErrorColor if the given string is not a valid CSS color.
    SDL_Color JitColorList::getSDL_ColorFromCSS_Color(const std::string &input) {
        // If the string is not the right size or does not begin with '#', return errorColor.
        if ((input.size() != 7) || (input.at(0) != '#'))
            throw std::runtime_error("Bad color");

        // Check the string to make sure that it is formatted correctly (contains characters only in the range of 0-F.)
        if (!stringIsHex(input.substr(1, 6)))
            throw std::runtime_error("Bad color");

        Uint8 red, blue, green;
        std::string redS, blueS, greenS;
        red = static_cast<Uint8>(std::stoi(input.substr(1, 2), nullptr, 16));
        blue = static_cast<Uint8>(std::stoi(input.substr(3, 2), nullptr, 16));
        green = static_cast<Uint8>(std::stoi(input.substr(5, 2), nullptr, 16));

        return SDL_Color{red, blue, green};
    }

// Returns true if the provided string is a valid hexadecimal number.
    bool JitColorList::stringIsHex(const std::string &input) {
        // If the input is empty, return false.
        if (input.empty())
            return false;

        // Scan through the string, returning false if any of the
        //   characters are not in the range of 0-F.
        if (std::all_of(input.begin(), input.end(), [](char c) { return std::isxdigit(c); }))
            return true;

        return false;
    }

    bool JitColorList::isMultiLine(const std::string &input) {
        if (input.size() < 3)
            return false;

        auto max = input.size();

        return (input[max - 1] == '.') && (input[max - 2] == '.') && (input[max - 3] == '.');
    }

    std::string JitColorList::getColor_Key(const std::string &input) {
        std::string tmp = getEncapsulatedContents(input, '[', ']', 0);
        return getEncapsulatedContents(tmp, '<', '>', 0);
    }

    std::optional<double> JitColorList::getCPS(const std::string &input) {
        double result;
        std::string tmp;
        try {
            tmp = getEncapsulatedContents(input, '[', ']', 1);
        } catch (...) {
            return std::nullopt;
        }

        try {
            tmp = getEncapsulatedContents(tmp, '[', ']', 1);
        } catch (...) {
            return std::nullopt;
        }

        try {
            result = std::stod(tmp, nullptr);
        } catch (...) {
            return std::nullopt;
        }

        return std::optional<double>(result);
    }

    std::optional<ColorDef> JitColorList::getColors(const std::string &input) {
        unsigned int nColors = 0;
        std::string tmp;

        try {
            tmp = getEncapsulatedContents(input, '[', ']', 1);
        } catch (...) {
            return std::nullopt;
        }

        try {
            tmp = getEncapsulatedContents(tmp, '[', ']', 0);
        } catch (...) {
            return std::nullopt;
        }

        std::vector<SDL_Color> tmpResult;
        unsigned int currentColor = 0;
        bool colorsLeft = true;
        while (colorsLeft) {
            std::string tmp2;
            SDL_Color tmpColor;
            try {
                tmp2 = getEncapsulatedContents(tmp, '[', ']', currentColor);
            } catch (std::out_of_range &e) {
                colorsLeft = false;
                continue;
            } catch (...) {
                return std::nullopt;
            }

            try {
                tmpColor = getSDL_ColorFromCSS_Color(tmp2);
            } catch (...) {
                return std::nullopt;
            }

            tmpResult.push_back(tmpColor);
            currentColor++;
            nColors++;
        }

        auto result = ColorDef(nColors);

        for (unsigned int i = 0; i < nColors; i++)
            result[i] = tmpResult.at(i);

        return result;
    }
}