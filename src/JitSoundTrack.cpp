#include "JitSoundTrack.h"

bool Jit::JitSoundTrack::playSound(const std::string &soundName) {
    return false;
}

bool Jit::JitSoundTrack::playMusic(const std::string &musicName, std::optional<FadeOutIn> fadeInOut) {
    try {
        Mix_Music *track = music.at(musicName);
        return !Mix_PlayMusic(track, 0);
    } catch (std::out_of_range &e) {
        return false;
    }
}

bool Jit::JitSoundTrack::loadSounds(const std::string &fileName) {
    const std::string LOADING_START_STRING = "$BEGIN_SOUND_DEF";
    const std::string LOADING_END_STRING = "$END_SOUND_DEF";
    const std::string SI_COMMAND_STR = "|--SoundDef-->";
    const std::string tag = "sounds";
    unsigned int nLoaded = 0;

    sounds.clear();

    // Determine the fileName of the resource.
    std::string path = getResourcePath("") + fileName;

    // If the fileName is empty, return false.
    if (fileName.empty())
        return false;

    printf("Loading sounds...\n");

    // Open the input file using the previously determined fileName.
    std::ifstream inputFile;
    inputFile.open(path);

    const std::string pathBackOne = backOneFile(path);

    // If the file could be opened, start loading the file. If not, return false.
    if (inputFile.is_open()) {
        unsigned int lineNumber = 0;
        bool isLoadingEnabled = false;

        while (!inputFile.eof()) {
            bool lineLoaded = false;
            std::string currentLine;
            Mix_Chunk newDef;

            // NOTE: line numbers start at one.
            lineNumber++;

            // Load the current line.
            currentLine = "";
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

            /* If the current line contains a LOADING_START_STRING, enable
             *   loading. Unless, loading is already enabled. In that
             *   case, print an error message and skip this line. */
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

            if (isLoadingEnabled) {

                // Search for the SI directive.
                size_t cPos = currentLine.find(SI_COMMAND_STR);

                // If the line doesn't contain a SI directive, skip this line.
                if (cPos == std::string::npos)
                    continue;

                // Try to get the SI of the current line
                std::pair<std::string, std::optional<Mix_Chunk *>> resultOpt = loadSoundFromPath(currentLine, pathBackOne);

                // If the SI exists, add it to storage.
                if (resultOpt.second.has_value()) {
                    Mix_Chunk *&result = resultOpt.second.value();
                    const std::string resultName = resultOpt.first;

                    /* Add the SI to storage. Check is a pair. The second
                     *   element is a bool that indicates if the insert
                     *   was successful. */
                    auto check = sounds.emplace(resultName, result);

                    // If the SI could not be added, print an error message.
                    if (!check.second) {
                        printf("   * Line %u is attempting to redefine an already defined SI Skipping.\n",
                               lineNumber);
                    }

                    lineLoaded = true;
                    nLoaded++;
                } else {
                    printf("   * Line %u: Error loading sound\n", lineNumber);
                    continue;
                }
            }
        }
    } else {
        // If the file could not be loaded, notify the user and return false.
        printf("   * Could not open the file \"%s\"\n", fileName.c_str());
        return false;
    }

    // When done loading the file, print how many SIs were loaded and return true.
    printf("   Loaded %u sounds.\n", nLoaded);

    inputFile.close();

    return true;
}

bool Jit::JitSoundTrack::loadMusic(const std::string &fileName) {
    const std::string LOADING_START_STRING = "$BEGIN_SOUND_DEF";
    const std::string LOADING_END_STRING = "$END_SOUND_DEF";
    const std::string SI_COMMAND_STR = "|--MusicDef-->";
    const std::string tag = "music";
    unsigned int nLoaded = 0;

    sounds.clear();

    // Determine the fileName of the resource.
    std::string path = getResourcePath("") + fileName;

    // If the fileName is empty, return false.
    if (fileName.empty())
        return false;

    printf("Loading music tracks...\n");

    // Open the input file using the previously determined fileName.
    std::ifstream inputFile;
    inputFile.open(path);

    const std::string pathBackOne = backOneFile(path);

    // If the file could be opened, start loading the file. If not, return false.
    if (inputFile.is_open()) {
        unsigned int lineNumber = 0;
        bool isLoadingEnabled = false;

        while (!inputFile.eof()) {
            bool lineLoaded = false;
            std::string currentLine;
            Mix_Chunk newDef;

            // NOTE: line numbers start at one.
            lineNumber++;

            // Load the current line.
            currentLine = "";
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

            /* If the current line contains a LOADING_START_STRING, enable
             *   loading. Unless, loading is already enabled. In that
             *   case, print an error message and skip this line. */
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

            if (isLoadingEnabled) {

                // Search for the SI directive.
                size_t cPos = currentLine.find(SI_COMMAND_STR);

                // If the line doesn't contain a SI directive, skip this line.
                if (cPos == std::string::npos)
                    continue;

                // Try to get the SI of the current line
                std::pair<std::string, std::optional<Mix_Music *>> resultOpt = loadTrackFromPath(currentLine, pathBackOne);

                // If the SI exists, add it to storage.
                if (resultOpt.second.has_value()) {
                    Mix_Music *&result = resultOpt.second.value();
                    const std::string resultName = resultOpt.first;

                    /* Add the SI to storage. Check is a pair. The second
                     *   element is a bool that indicates if the insert
                     *   was successful. */
                    auto check = music.emplace(resultName, result);

                    // If the SI could not be added, print an error message.
                    if (!check.second) {
                        printf("   * Line %u is attempting to redefine an already defined SI Skipping.\n",
                               lineNumber);
                    }

                    lineLoaded = true;
                    nLoaded++;
                } else {
                    printf("   * Line %u is not formatted correctly. Skipping\n", lineNumber);
                    continue;
                }
            }
        }
    } else {
        // If the file could not be loaded, notify the user and return false.
        printf("   * Could not open the file \"%s\"\n", fileName.c_str());
        return false;
    }

    // When done loading the file, print how many SIs were loaded and return true.
    printf("   Loaded %u tracks.\n", nLoaded);

    inputFile.close();

    return true;
}

bool Jit::JitSoundTrack::isMultiLine(const std::string &input) {
    if (input.size() < 3)
        return false;

    auto max = input.size();

    return (input[max - 1] == '.') && (input[max - 2] == '.') && (input[max - 3] == '.');
}

std::pair<std::string, std::optional<Mix_Chunk *>> Jit::JitSoundTrack::loadSoundFromPath(const std::string &input, const std::string &basePath) {
    std::pair<std::string, std::optional<Mix_Chunk *>> result;
    bool loadGood = true;

    try {
        std::string name = getEncapsulatedContents(input, '[', ']', 0);
        name = getEncapsulatedContents(name, '<', '>', 0);

        std::string ppath = getEncapsulatedContents(input, '[', ']', 1);
        ppath = getEncapsulatedContents(ppath, '<', '>', 0);

        if (!name.empty())
            result.first = name;
        else
            loadGood = false;

        if (!ppath.empty()) {
            ppath = basePath + ppath;
            const char *path = ppath.c_str();
            Mix_Chunk *ptr = Mix_LoadWAV(path);
            result.second = std::optional<Mix_Chunk *>(ptr);
            if (result.second.value() == nullptr)
                result.second = std::nullopt;
        }
    } catch (...) {
        loadGood = false;
    }

    if (loadGood)
        return result;
    else
        return std::pair<std::string, std::optional<Mix_Chunk *>>("", std::nullopt);
    return result;
}

std::pair<std::string, std::optional<Mix_Music *>> Jit::JitSoundTrack::loadTrackFromPath(const std::string &input, const std::string &basePath) {
    std::pair<std::string, std::optional<Mix_Music *>> result;
    bool loadGood = true;

    try {
        std::string name = getEncapsulatedContents(input, '[', ']', 0);
        name = getEncapsulatedContents(name, '<', '>', 0);

        std::string ppath = getEncapsulatedContents(input, '[', ']', 1);
        ppath = getEncapsulatedContents(ppath, '<', '>', 0);

        if (!name.empty())
            result.first = name;
        else
            loadGood = false;

        if (!ppath.empty()) {
            ppath = basePath + ppath;
            const char *path = ppath.c_str();
            result.second = std::optional<Mix_Music *>(Mix_LoadMUS(path));
        }
    } catch (...) {
        loadGood = false;
    }

    if (loadGood)
        return result;
    else
        return std::pair<std::string, std::optional<Mix_Music *>>("", std::nullopt);
    return result;
}
