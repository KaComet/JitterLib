#ifndef PROJECT_21_JITSOUNDTRACK_H
#define PROJECT_21_JITSOUNDTRACK_H

#include <string>
#include <optional>
#include <fstream>
#include <vector>
#include <filesystem>
#include <stdexcept>
#include <memory>
#include <unordered_map>
#include <SDL.h>
#include <SDL_mixer.h>
#include <FlatMath.h>

#include "JitResource.h"

namespace Jit {
    struct FadeOutIn {
        FadeOutIn() : FadeOutIn(0, 0, 0, 0) {};

        FadeOutIn(double fadeOutDuration, double fadeOutEndVolume, double fadeInDuration, double fadeInEndVolume) :
                fadeOutDuration(fadeOutDuration),
                fadeOutEndVolume(fadeOutEndVolume),
                fadeInDuration(fadeInDuration),
                fadeInEndVolume(fadeInEndVolume) {
            isFadingIn = (fadeInDuration > 0);
            isFadingOut = (fadeOutDuration > 0);
        };

        bool isFadingIn, isFadingOut;
        double fadeOutDuration;
        double fadeOutEndVolume;
        double fadeInDuration;
        double fadeInEndVolume;
    };

    class JitSoundTrack {
    public:
        bool playSound(const std::string &soundName);

        bool playMusic(const std::string &musicName, std::optional<FadeOutIn> fadeInOut = std::nullopt);

        bool loadSounds(const std::filesystem::path &path);

        bool loadMusic(const std::filesystem::path &path);

        void unloadAll();

        void unloadMusic();

        void unloadSounds();

    private:
        const std::string LOADING_START_STRING = "$BEGIN_SOUND_DEF"; // Sound definition start directive
        const std::string LOADING_END_STRING = "$END_SOUND_DEF"; // Sound definition end directive
        const std::string SD_COMMAND_STR = "|--SoundDef-->"; // Sound definition command
        const std::string MD_COMMAND_STR = "|--MusicDef-->"; // Music definition command

        static bool isMultiLine(const std::string &input);

        static std::pair<std::string, std::optional<Mix_Chunk *>> loadSoundFromPath(const std::string &input, const std::string &basePath);

        static std::pair<std::string, std::optional<Mix_Music *>> loadTrackFromPath(const std::string &input, const std::string &basePath);

        std::unordered_map<std::string, Mix_Chunk *> sounds;
        std::unordered_map<std::string, Mix_Music *> music;
        FadeOutIn current, next;
    };

}


#endif //PROJECT_21_JITSOUNDTRACK_H
