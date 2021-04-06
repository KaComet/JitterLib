#ifndef TILESETVIEWER_FRAMEDEFMAP_H
#define TILESETVIEWER_FRAMEDEFMAP_H

#include <FlatVector.h>
#include <string>
#include "JitUIBasics.h"
#include <fstream>
#include "JitResource.h"
#include <vector>
#include <algorithm>
#include <optional>
#include <HappyArray.h>
#include <map>

namespace Jit {
    struct FrameDef {
        FrameDef() : frames(flat::HappyArray<Jit::FrameID>(0)) {
            nFrames = 0;
            FPS = 0;
        }

        Jit::FrameID &operator[](unsigned int i) const {
            if (i >= nFrames)
                throw std::out_of_range("");

            return frames[i];
        }

        [[nodiscard]] Jit::FrameID getFromTime(long double secondsCounter) const {
            if (nFrames == 0)
                throw std::runtime_error("");

            unsigned int i = (unsigned int) (secondsCounter * FPS) % nFrames;

            if (i >= nFrames)
                throw std::out_of_range("");

            return frames[i];
        }

        std::string frameKey;
        size_t nFrames;
        flat::HappyArray<Jit::FrameID> frames;
        long double FPS;
    };

    class JitFrameDefMap {
    public:
        bool loadFrameDefs(const std::string &fileName);

        FrameDef get(const std::string &name);

    private:
        // Gets the frame key from the given string. Returns nothing if it couldn't be found.
        static std::optional<std::string> getFrameKey(const std::string &input);

        // Get the frameIDs from a given string. Returns nothing if any couldn't be found.
        static std::optional<flat::HappyArray<Jit::FrameID>> getFrames(const std::string &input);

        // Get the FPS from a given string. Returns nothing if the FPS couldn't be found.
        static std::optional<double> getFPS(const std::string &input);

        static bool isMultiLine(const std::string &input);

        std::unordered_map<std::string, FrameDef> _spriteDefMap;

        //TODO: define on load.
        FrameDef _defaultFrameDef;
    };
}


#endif //TILESETVIEWER_FRAMEDEFMAP_H
