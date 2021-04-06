#include "FrameDefMap.h"

bool FrameDefMap::loadFrameDefs(const std::string &fileName) {
    const std::string LOADING_START_STRING = "$BEGIN_SPRITE_DEF";
    const std::string LOADING_END_STRING = "$END_SPRITE_DEF";
    // Determine the path of the resource.
    std::string path = getResourcePath("") + fileName;

    // If the path is empty, return false.
    if (path.empty())
        return false;

    printf("Loading frame definitions...\n");

    // Open the input file using the previously determined path.
    std::ifstream inputFile;
    inputFile.open(path);

    // If the file could be opened, start loading the file. If not, return false.
    if (inputFile.is_open()) {
        unsigned int lineNumber = 0, nLoaded = 0;
        bool isLoadingEnabled = false;

        while (!inputFile.eof()) {
            bool lineLoaded = false;
            std::string currentLine;

            // Load the current line.
            do {
                // NOTE: line numbers start at one.
                lineNumber++;
                std::string tmp;
                std::getline(inputFile, tmp);
                //lineNumber++;
                tmp = clearWhiteSpace(tmp);
                currentLine.append(tmp);
            } while (isMultiLine(currentLine));

            if (currentLine.empty())
                continue;

            if (currentLine == LOADING_START_STRING) {
                if (isLoadingEnabled) {
                    printf("   * Line %u is not formatted correctly. Contains %s even though one has been declared earlier in the file. Skipping.\n",
                           lineNumber, LOADING_START_STRING.c_str());
                    continue;
                } else {
                    isLoadingEnabled = true;
                    continue;
                }
            }

            if (currentLine == LOADING_END_STRING) {
                if (isLoadingEnabled) {
                    isLoadingEnabled = false;
                    continue;
                } else {
                    printf("   * Line %u is not formatted correctly. Contains %s even though a %s hasn't been declared earlier in the file. Skipping.\n",
                           lineNumber, LOADING_END_STRING.c_str(), LOADING_START_STRING.c_str());
                    continue;
                }
            }

            if (isLoadingEnabled) {
                FrameDef spriteDefMapEntry;

                // Try to parse the contents of the line.
                try {

                    auto key = getFrameKey(currentLine);
                    auto frames = getFrames(currentLine);
                    auto FPS = getFPS(currentLine);

                    spriteDefMapEntry.frameKey = key.value();
                    spriteDefMapEntry.frames = frames.value();
                    spriteDefMapEntry.nFrames = frames.value().size();
                    spriteDefMapEntry.FPS = FPS.value();
                } catch (...) {
                    // If the line was incorrectly formatted, print an error message and skip it.
                    printf("   * Line %u is not formatted correctly. Skipping.\n", lineNumber);
                    continue;
                }


                if (spriteDefMapEntry.frameKey == "default") {
                    _defaultFrameDef = spriteDefMapEntry;
                    nLoaded++;
                } else {
                    _spriteDefMap.emplace(spriteDefMapEntry.frameKey, spriteDefMapEntry);
                    nLoaded++;
                }
            }
        }

        if (nLoaded) {
            // When done loading the file, print how many SIs were loaded and return true.
            printf("   Loaded %u frame definitions.\n", nLoaded);
            return true;
        } else {
            printf("   Was not able to load any frame definitions from the file.\n");
            return false;
        }
    } else {

        // If the file could not be loaded, notify the user and return false.
        printf("   * Could not open the file \"%s\"\n", path.c_str());
        return false;
    }
}

// [<!SD!-simpleEnt>]       |--SpriteDef--> [ RawSprite [[1][1][1][1][2][2][2][2]]  at [5.0] FPS]
//   ^____________^ <- Returns this part of the def.
std::optional<std::string> FrameDefMap::getFrameKey(const std::string& input) {
    std::string output;
    try {
        output = getEncapsulatedContents(input, '[', ']', 0);
    } catch (...) {
        return std::nullopt;
    }

    try {
        output = getEncapsulatedContents(input, '<', '>', 0);
    } catch (...) {
        return std::nullopt;
    }

    return std::optional<std::string>(output);
}

// [<!SD!-simpleEnt>]       |--SpriteDef--> [ RawSprite [[1][1][1][1][2][2][2][2]]  at [5.0] FPS]
//                              Returns these frames --> ^______________________^
std::optional<flat::HappyArray<walt::UI::FrameID>> FrameDefMap::getFrames(const std::string &input) {
    unsigned int nFrames = 0;
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

    std::vector<walt::UI::FrameID> tmpResult;
    unsigned int currentFrame = 0;
    bool framesLeft = true;
    while (framesLeft) {
        std::string tmp2;
        unsigned int thing;
        try {
            tmp2 = getEncapsulatedContents(tmp, '[', ']', currentFrame);
        } catch (std::out_of_range &e) {
            framesLeft = false;
            continue;
        } catch (...) {
            return std::nullopt;
        }

        try {
            thing = std::stoi(tmp2, nullptr, 10);
        } catch (...) {
            return std::nullopt;
        }

        tmpResult.push_back(thing);
        currentFrame++;
        nFrames++;
    }

    auto result = flat::HappyArray<walt::UI::FrameID>(nFrames);

    for (unsigned int i = 0; i < nFrames; i++)
        result[i] = tmpResult.at(i);


    return result;
}

// [<!SD!-simpleEnt>]       |--SpriteDef--> [ RawSprite [[1][1][1][1][2][2][2][2]]  at [5.0] FPS]
//                                                            Returns the FPS frames --> ^
std::optional<double> FrameDefMap::getFPS(const std::string& input) {
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

// If a line is ended with ellipses "...", it's a multiline.
bool FrameDefMap::isMultiLine(const std::string &input) {
    if (input.size() < 3)
        return false;

    auto max = input.size();

    return (input[max - 1] == '.') && (input[max - 2] == '.') && (input[max - 3] == '.');
}

FrameDef FrameDefMap::get(const std::string &name) {
    FrameDef result;
    try {
        result = _spriteDefMap.at(name);
    } catch (std::out_of_range &e) {
        result = _spriteDefMap.at("undefined");
    }

    return result;
}
