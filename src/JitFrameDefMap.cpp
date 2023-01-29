#include "JitFrameDefMap.h"

namespace Jit {
    bool JitFrameDefMap::loadFrameDefs(const std::filesystem::path &path) {
        unsigned int nLoaded = 0;

        // Clear the old definitions.
        _spriteDefMap.clear();

        // If the path is empty, return false.
        if (path.empty())
            return false;

        printf("Loading frame definitions...\n");

        // Open the input file using the previously determined path.
        std::ifstream inputFile;
        inputFile.open(path);

        // If the file could be opened, start loading the file. If not, return false.
        if (inputFile.is_open()) {
            unsigned int lineNumber = 0;

            // This will indicate if the line being loaded is within a frame definition directive
            bool isLoadingEnabled = false;

            while (!inputFile.eof()) {
                std::string currentLine;

                /* Load the current line. If the current line ends in ellipses, indicating a
                 *   multi-line, continue loading until the line is complete (no more ellipses.) */
                do {
                    // NOTE: line numbers start at one.
                    lineNumber++;
                    std::string tmp;
                    std::getline(inputFile, tmp);
                    currentLine.append(tmp);
                } while (isMultiLine(currentLine));

                // Check if the current line is a comment. if so, skip.
                if (currentLine.empty() || (flat::getFirstNonWhitespace(currentLine) == '@'))
                    continue;

                /* If the current line contains the start directive, enable
                 *   loading. Unless, loading is already enabled. In that
                 *   case, print an error message. */
                const size_t startString = currentLine.find(LOADING_START_STRING);
                if (startString != std::string::npos) {
                    if (isLoadingEnabled) {
                        printf("   * Line %u is not formatted correctly. Contains %s even though one has been declared earlier in the file. Skipping.\n",
                               lineNumber, LOADING_START_STRING.c_str());
                        continue;
                    } else {
                        isLoadingEnabled = true;
                        continue;
                    }
                }

                /* If the current line contains the end directive, disable
                 *   loading. Unless, loading is already disabled. In that
                 *   case, skip this line. */
                if (currentLine == LOADING_END_STRING) {
                    if (isLoadingEnabled) {
                        isLoadingEnabled = false;
                        continue;
                    } else
                        continue;
                }

                // If we are within a frame definition directive, start trying to load data.
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

                    /* Add the SpriteDef to storage. Check is a pair. The second
                     *   element is a bool that indicates if the insert
                     *   was successful. */
                    auto check = _spriteDefMap.emplace(spriteDefMapEntry.frameKey, spriteDefMapEntry);

                    // If the SpriteDef could not be added, print an error message.
                    if (!check.second) {
                        printf("   * Line %u is attempting to redefine an already defined frame definition Skipping.\n",
                               lineNumber);
                        continue;
                    }
                    nLoaded++;
                }
            }
            inputFile.close();
        } else {

            // If the file could not be loaded, notify the user and return false.
            printf("   * Could not open the file \"%s\"\n", path.c_str());
            return false;
        }

        // Print how many frames were loaded. If none were loaded, indicate so.
        if (nLoaded) {
            // When done loading the file, print how many frame definitions were loaded and return true.
            printf("   Loaded %u frame definitions.\n", nLoaded);
            return true;
        } else {
            printf("   Was not able to load any frame definitions from the file.\n");
            return false;
        }
    }

// [<!SD!-grass>] |--SpriteDef--> [ RawSprite [[43][43][43][45][253][96][45]] at [0.0] FPS]
//   ^________^ <- Returns this part of the def.
    std::optional<std::string> JitFrameDefMap::getFrameKey(const std::string &input) {
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

// [<!SD!-grass>] |--SpriteDef--> [ RawSprite [[43][43][43][45][253][96][45]] at [0.0] FPS]
//                    Returns these frames --> ^___________________________^
    std::optional<flat::HappyArray<Jit::FrameID>> JitFrameDefMap::getFrames(const std::string &input) {
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

        std::vector<Jit::FrameID> tmpResult;
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

        auto result = flat::HappyArray<Jit::FrameID>(nFrames);

        for (unsigned int i = 0; i < nFrames; i++)
            result[i] = tmpResult.at(i);


        return result;
    }

// [<!SD!-grass>] |--SpriteDef--> [ RawSprite [[43][43][43][45][253][96][45]] at [0.0] FPS]
//                                                            Returns the FPS --> ^_^
    std::optional<double> JitFrameDefMap::getFPS(const std::string &input) {
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
    bool JitFrameDefMap::isMultiLine(const std::string &input) {
        if (input.size() < 3)
            return false;

        auto max = input.size();

        return (input[max - 1] == '.') && (input[max - 2] == '.') && (input[max - 3] == '.');
    }

    FrameDef &JitFrameDefMap::get(const std::string &name) {
        return _spriteDefMap.at(name);
    }

    bool JitFrameDefMap::checkIfContains(const std::string &name) {
        try {
            _spriteDefMap.at(name);
            return true;
        } catch (std::out_of_range &e) {
            return false;
        }
    }
}
