#ifndef WELT_RESOURCE_H
#define WELT_RESOURCE_H

#include <stdexcept>
#include <iostream>
#include <string>
#include <SDL.h>

namespace Jit {
#ifdef _WIN32
    const char PATH_SEP = '\\';
#else
    const char PATH_SEP = '/';
#endif

    std::string getResourcePath(const std::string &subDir);

    std::string getDelimitedContents(const std::string &input, char DelimitingChar, unsigned int start);

/* Extracts the contents from a capsule. Will only extract the contents of top-level capsules (Will not
 *   parse any encapsulated capsules.) For example:
 *   getEncapsulatedContents("...[][ABC][[1]],[[!@#]2]", '[', ']', 3)
 *   returns the string "[[!@#]2]" */
    std::string getEncapsulatedContents(const std::string &input, char capsuleStartChar, char capsuleEndChar,
                                        uint capsuleNumber);

    std::string clearWhiteSpace(const std::string &input);

    std::string backOneFile(const std::string &path);
}

#endif //WELT_RESOURCE_H
