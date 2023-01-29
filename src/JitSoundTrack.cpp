#include "JitSoundTrack.h"

bool Jit::JitSoundTrack::playSound(const std::string &soundName) {
    try {
        Mix_Chunk *sound = sounds.at(soundName);
        return !Mix_PlayChannel(-1, sound, 0);
    } catch (std::out_of_range &e) {
        return false;
    }
}

bool Jit::JitSoundTrack::playMusic(const std::string &musicName, std::optional<FadeOutIn> fadeInOut) {
    //TODO: Add fading.
    try {
        Mix_Music *track = music.at(musicName);
        return !Mix_PlayMusic(track, 0);
    } catch (std::out_of_range &e) {
        return false;
    }
}

bool Jit::JitSoundTrack::loadSounds(const std::filesystem::path &path) {
    const std::string tag = "sounds";
    unsigned int nLoaded = 0;

    unloadSounds();

    // If the fileName is empty, return false.
    if (path.empty())
        return false;

    printf("Loading sounds...\n");

    // Open the input file using the previously determined fileName.
    std::ifstream inputFile;
    inputFile.open(path);

    const std::string pathBackOne = backOneFile(path);

#pragma clang diagnostic push // Clang is being temperamental
#pragma ide diagnostic ignored "UnreachableCode"
    // If the file could be opened, start loading the file. If not, return false.
    if (inputFile.is_open()) {
        unsigned int lineNumber = 0;
        bool isLoadingEnabled = false;

#pragma clang diagnostic push // Clang is being temperamental
#pragma ide diagnostic ignored "EndlessLoop"
        while (!inputFile.eof()) {
            std::string currentLine;

            // Load the current line.
            do {
                // NOTE: line numbers start at one.
                lineNumber++;
                std::string tmp;
                std::getline(inputFile, tmp);
                tmp = clearWhiteSpace(tmp);
                currentLine.append(tmp);
            } while (isMultiLine(currentLine));

            // Check if the current line is a comment. if so, skip.
            if (currentLine.empty() || (flat::getFirstNonWhitespace(currentLine) == '@'))
                continue;

            /* If the current line contains the start directive, enable
             *   loading. Unless, loading is already enabled. In that
             *   case, print an error message. */
            const size_t startStringPos = currentLine.find(LOADING_START_STRING);
            if (startStringPos != std::string::npos) {
                std::string thisTag;
                try {
                    thisTag = getEncapsulatedContents(currentLine, '<', '>', 0);
                } catch (...) {
                    thisTag = "";
                }

                /* Try and get the tag. if one is present, enable loading. if not,
                 *   keep loading disabled and print an error message. */
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

            // If we are within a sound definition directive, start trying to load data.
            if (isLoadingEnabled) {
                // Search for the sound definition command
                size_t cPos = currentLine.find(SD_COMMAND_STR);

                // If the line doesn't contain a sound definition command, skip this line.
                if (cPos == std::string::npos)
                    continue;

                // Try to get the sound of the current line
                std::pair<std::string, std::optional<Mix_Chunk *>> resultOpt = loadSoundFromPath(currentLine,
                                                                                                 pathBackOne);

                // If the sound exists, add it to storage.
                if (resultOpt.second.has_value()) {
                    Mix_Chunk *&result = resultOpt.second.value();
                    const std::string resultName = resultOpt.first;

                    /* Add the sound to storage. Check is a pair. The second
                     *   element is a bool that indicates if the insert
                     *   was successful. */
                    auto check = sounds.emplace(resultName, result);

                    // If the sound could not be added, print an error message.
                    if (!check.second) {
                        printf("   * Line %u is attempting to redefine an already defined sound Skipping.\n",
                               lineNumber);
                    }

                    nLoaded++;
                } else {
                    printf("   * Line %u: Error loading sound\n", lineNumber);
                    continue;
                }
            }
        }

        inputFile.close();
    } else {
        // If the file could not be loaded, notify the user and return false.
        printf("   * Could not open the file \"%s\"\n", path.c_str());
        return false;
    }
#pragma clang diagnostic pop
#pragma clang diagnostic pop

    // Print how many sounds were loaded. If none were loaded, indicate so.
    if (nLoaded) {
        // When done loading the file, print how many sound definitions were loaded and return true.
        printf("   Loaded %u sounds.\n", nLoaded);
        return true;
    } else {
        printf("   Was not able to load any sounds from the file.\n");
        return false;
    }
}

bool Jit::JitSoundTrack::loadMusic(const std::filesystem::path &path) {
    const std::string tag = "music";
    unsigned int nLoaded = 0;

    unloadMusic();

    // If the fileName is empty, return false.
    if (path.empty())
        return false;

    printf("Loading music tracks...\n");

    // Open the input file using the previously determined fileName.
    std::ifstream inputFile;
    inputFile.open(path);

    const std::string pathBackOne = backOneFile(path);

#pragma clang diagnostic push // Clang is being temperamental
#pragma ide diagnostic ignored "UnreachableCode"
    // If the file could be opened, start loading the file. If not, return false.
    if (inputFile.is_open()) {
        unsigned int lineNumber = 0;
        bool isLoadingEnabled = false;

#pragma clang diagnostic push // Clang is being temperamental
#pragma ide diagnostic ignored "EndlessLoop"
        while (!inputFile.eof()) {
            std::string currentLine;

            /* Load the current line. If the current line ends in ellipses, indicating a
             *   multi-line, continue loading until the line is complete (no more ellipses.) */
            do {
                // NOTE: line numbers start at one.
                lineNumber++;
                std::string tmp;
                std::getline(inputFile, tmp);
                tmp = clearWhiteSpace(tmp);
                currentLine.append(tmp);
            } while (isMultiLine(currentLine) || (flat::getFirstNonWhitespace(currentLine) == '@'));

            // Check if the current line is a comment. if so, skip.
            if (currentLine.empty() || (flat::getFirstNonWhitespace(currentLine) == '@'))
                continue;

            /* If the current line contains the start directive, enable
                 *   loading. Unless, loading is already enabled. In that
                 *   case, print an error message. */
            const size_t startStringPos = currentLine.find(LOADING_START_STRING);
            if (startStringPos != std::string::npos) {
                std::string thisTag;
                try {
                    thisTag = getEncapsulatedContents(currentLine, '<', '>', 0);
                } catch (...) {
                    thisTag = "";
                }

                /* Try and get the tag. if one is present, enable loading. if not,
                 *   keep loading disabled and print an error message. */
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

            // If we are within a sound definition directive, start trying to load data.
            if (isLoadingEnabled) {
                // Search for the music definition command.
                size_t cPos = currentLine.find(MD_COMMAND_STR);

                // If the line doesn't contain a music definition command, skip this line.
                if (cPos == std::string::npos)
                    continue;

                // Try to get the track of the current line
                std::pair<std::string, std::optional<Mix_Music *>> resultOpt = loadTrackFromPath(currentLine,
                                                                                                 pathBackOne);

                // If the track exists, add it to storage.
                if (resultOpt.second.has_value()) {
                    Mix_Music *&result = resultOpt.second.value();
                    const std::string resultName = resultOpt.first;

                    /* Add the track to storage. Check is a pair. The second
                     *   element is a bool that indicates if the insert
                     *   was successful. */
                    auto check = music.emplace(resultName, result);

                    // If the track could not be added, print an error message.
                    if (!check.second) {
                        printf("   * Line %u is attempting to redefine an already defined track Skipping.\n",
                               lineNumber);
                    }

                    nLoaded++;
                } else {
                    printf("   * Line %u is not formatted correctly. Skipping\n", lineNumber);
                    continue;
                }
            }
        }

        inputFile.close();
    } else {
        // If the file could not be loaded, notify the user and return false.
        printf("   * Could not open the file \"%s\"\n", path.c_str());
        return false;
    }
#pragma clang diagnostic pop
#pragma clang diagnostic pop

    // Print how many tracks were loaded. If none were loaded, indicate so.
    if (nLoaded) {
        // When done loading the file, print how many music definitions were loaded and return true.
        printf("   Loaded %u tracks.\n", nLoaded);
        return true;
    } else {
        printf("   Was not able to load any tracks from the file.\n");
        return false;
    }
}

void Jit::JitSoundTrack::unloadAll() {
    unloadMusic();
    unloadSounds();
}

void Jit::JitSoundTrack::unloadMusic() {
    for (const auto &t : music)
        Mix_FreeMusic(t.second);

    music.clear();
}

void Jit::JitSoundTrack::unloadSounds() {
    for (const auto &s : sounds)
        Mix_FreeChunk(s.second);

    sounds.clear();
}

bool Jit::JitSoundTrack::isMultiLine(const std::string &input) {
    if (input.size() < 3)
        return false;

    auto max = input.size();

    return (input[max - 1] == '.') && (input[max - 2] == '.') && (input[max - 3] == '.');
}

std::pair<std::string, std::optional<Mix_Chunk *>>
Jit::JitSoundTrack::loadSoundFromPath(const std::string &input, const std::string &basePath) {
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

std::pair<std::string, std::optional<Mix_Music *>>
Jit::JitSoundTrack::loadTrackFromPath(const std::string &input, const std::string &basePath) {
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
