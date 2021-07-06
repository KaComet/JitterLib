#ifndef PROJECT_21_JITSOUNDTRACK_H
#define PROJECT_21_JITSOUNDTRACK_H

#include <string>
#include <optional>
#include <fstream>
#include <vector>
#include <stdexcept>
#include <memory>
#include <unordered_map>
#include <SDL.h>
#include <SDL_mixer.h>

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

        bool loadSounds(const std::string &fileName);

        bool loadMusic(const std::string &fileName);

    private:
        bool isMultiLine(const std::string &input);

        static std::pair<std::string, std::optional<Mix_Chunk *>> loadSoundFromPath(const std::string &input, const std::string &basePath);

        static std::pair<std::string, std::optional<Mix_Music *>> loadTrackFromPath(const std::string &input, const std::string &basePath);

        std::unordered_map<std::string, Mix_Chunk *> sounds;
        std::unordered_map<std::string, Mix_Music *> music;
        FadeOutIn current, next;
    };

}


#endif //PROJECT_21_JITSOUNDTRACK_H
