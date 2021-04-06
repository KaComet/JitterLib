#include "JitResource.h"

namespace Jit {
// Get the path to a file in the resource directory.
// NOTE: all paths assume that the executable is contained in a directory named "bin".
    std::string getResourcePath(const std::string &subDir) {
#ifdef _WIN32
        const char PATH_SEP = '\\';
#else
        const char PATH_SEP = '/';
#endif
        //This will hold the base resource path: Lessons/res/
        //We give it static lifetime so that we'll only need to call
        //SDL_GetBasePath once to get the executable path
        static std::string baseRes;
        if (baseRes.empty()) {
            //SDL_GetBasePath will return nullptr if something went wrong in retrieving the path
            char *basePath = SDL_GetBasePath();
            if (basePath != nullptr) {
                baseRes = basePath;
                SDL_free(basePath);
            } else {
                std::cerr << "Error getting resource path: " << SDL_GetError() << std::endl;
                return "";
            }
            //We replace the last bin/ with res/ to get the the resource path
            size_t pos = baseRes.rfind("bin");
            baseRes = baseRes.substr(0, pos) + "resource" + PATH_SEP;
        }
        //If we want a specific subdirectory path in the resource directory
        //append it to the base path. This would be something like Lessons/res/Lesson0
        return subDir.empty() ? baseRes : baseRes + subDir + PATH_SEP;
    }

    std::string getDelimitedContents(const std::string &input, const char DelimitingChar, const uint start) {
        uint delmitCount = 0;
        size_t beginPos = 0, endPos = 0;
        for (size_t i = 0; i < input.size(); i++) {
            if (DelimitingChar == input.at(i)) {
                delmitCount++;

                if (delmitCount == start) {
                    beginPos = i + 1;
                }
            }

            if (delmitCount == (start + 1)) {
                endPos = i;
                break;
            }
        }

        if (delmitCount < start)
            return " ";

        if (endPos < beginPos)
            endPos = input.size();

        return input.substr(beginPos, endPos - beginPos);
    }

    std::string
    getEncapsulatedContents(const std::string &input, const char capsuleStartChar, const char capsuleEndChar,
                            const uint capsuleNumber) {
        std::string result;
        uint capsuleStartCount = 0;
        uint capsuleEndCount = 0;
        int currentCapsuleNumber = -1;
        size_t beginPos = 0, endPos = 0;
        bool isInCapsule = false, enteringCapsule = false, exitingCapsule = false;

        // Scan through every character in the input.
        for (char c : input) {
            // If we find a capsule opener, count it.
            if (c == capsuleStartChar)
                capsuleStartCount++;

            // If we find a capsule closer, count it.
            if (c == capsuleEndChar)
                capsuleEndCount++;

            /* If we have the same number of starters then closers, and we
             *   were just in a capsule, we are exiting a capsule. */
            if ((capsuleStartCount == capsuleEndCount) && isInCapsule)
                exitingCapsule = true;
            else
                exitingCapsule = false;

            /* If we have more starters then closers, and we're not in a
             *   capsule, we are entering a capsule. */
            if ((capsuleStartCount > capsuleEndCount) && !isInCapsule)
                enteringCapsule = true;
            else
                enteringCapsule = false;

            /* If we determined we're entering a top-level
             *   capsule, set the current capsule
             *   number, and set the necessary flags. */
            if (enteringCapsule) {
                currentCapsuleNumber++;
                isInCapsule = true;
                enteringCapsule = false;
                continue;
            }

            /* If we determined we're entering a top-level
             *   capsule, set the necessary flags. */
            if (exitingCapsule) {
                isInCapsule = false;
                exitingCapsule = false;
                continue;
            }

            // If we're in the correct capsule, add the c to the result.
            if (isInCapsule && (currentCapsuleNumber == capsuleNumber))
                result.push_back(c);

            // Leave the loop if we have found what we needed.
            if ((currentCapsuleNumber == (int) capsuleNumber) && !isInCapsule)
                break;
        }

        // If the desired capsule does not close, throw.
        if (isInCapsule && (currentCapsuleNumber == capsuleNumber)) {
            throw std::runtime_error("Capsule " + std::to_string(capsuleNumber) + " does not close.");
        }

        // If the desired capsule could not be found, throw.
        if ((currentCapsuleNumber < capsuleNumber) || (currentCapsuleNumber == -1)) {
            throw std::out_of_range("Capsule " + std::to_string(capsuleNumber) + " does not exist.");
        }

        return result;
    }

    std::string clearWhiteSpace(const std::string &input) {
        std::string result;

        for (char c:input) {
            if ((c > ' ') && (c < '~'))
                result.push_back(c);
        }

        return result;
    }
}