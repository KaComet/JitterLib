#include "JitSpriteInteractionsList.h"

namespace Jit {
    bool JitSpriteInteractionsList::loadSpriteInteractions(const std::string &fileName, const std::string &tag) {
        const std::string LOADING_START_STRING = "$BEGIN_SPRITE_INTERACTIONS_DEF";
        const std::string LOADING_END_STRING = "$END_SPRITE_INTERACTIONS_DEF";
        const std::string SI_COMMAND_STR = "|--SpriteInteraction-->";
        unsigned int nLoaded = 0;

        SI_list.clear();

        // Determine the path of the resource.
        std::string path = getResourcePath("") + fileName;

        // If the path is empty, return false.
        if (path.empty())
            return false;

        printf("Loading %s sprite interactions...\n", tag.c_str());

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
                JitFrameDefMap newDef;

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
                    std::optional<Jit::SpriteInteraction> resultOpt = loadSI(currentLine);

                    // If the SI exists, add it to storage.
                    if (resultOpt.has_value()) {
                        Jit::SpriteInteraction &result = resultOpt.value();

                        /* Add the SI to storage. Check is a pair. The second
                         *   element is a bool that indicates if the insert
                         *   was successful. */
                        auto check = SI_list.emplace(result.name, result);

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
            printf("   * Could not open the file \"%s\"\n", path.c_str());
            return false;
        }

        // When done loading the file, print how many SIs were loaded and return true.
        printf("   Loaded %u sprite interactions.\n", nLoaded);

        return true;
    }

    std::optional<Jit::SpriteInteraction> JitSpriteInteractionsList::getSI(const JitDisplayMaterial &material) {
        std::optional<Jit::SpriteInteraction> result;

        try {
            result = SI_list.at(material.materialName);
        } catch (std::out_of_range &e) {
            std::cout << "Could not find SI <" << material.materialName << ">.";
            return std::nullopt;
        }

        return result;
    }

    /*

    std::optional<FrameDef>
    JitSpriteInteractionsList::get(const flat::IntegerCoordinate &cord, Jit::JitDisplayData &dis,
                                   JitFrameDefMap &frameDefs, bool getBackground) {
        if (!flat::isInBound(cord, dis.area()))
            return std::nullopt;

        JitDisplayMaterial thisMaterial;
        if (getBackground)
            thisMaterial = dis.at(cord).floorMaterial;
        else
            thisMaterial = dis.at(cord).wallMaterial;

        std::optional<Jit::SpriteInteraction> thisInteraction = getSI(thisMaterial);
        if (thisInteraction.has_value()) {
            FrameDef tmp = getInteraction(thisMaterial, thisInteraction.value(), dis, frameDefs, cord, getBackground);
            if (tmp.nFrames == 0) {
                std::cout << thisMaterial.materialName << " SI def is incomplete.\n";
                std::nullopt;
            } else
                return tmp;
        }

        return std::nullopt;
    }
     */

    FrameDef JitSpriteInteractionsList::getFrameDef(const std::string &name, JitFrameDefMap &frameDefs,
                                                    Jit::FrameDef &errorFrame) {
        Jit::SpriteInteraction resultSI;
        try {
            resultSI = SI_list.at(name);
            if (frameDefs.checkIfContains(resultSI.dDefault))
                return frameDefs.get(resultSI.dDefault);
        } catch (std::out_of_range &e) {}

        return errorFrame;
    }

/*FrameDef JitSpriteInteractionsList::getInteraction(const JitDisplayMaterial &material,
                                                   const Jit::SpriteInteraction &spriteInteraction,
                                                   Jit::JitDisplayData &dis,
                                                   JitFrameDefMap &frameDefs,
                                                   const flat::IntegerCoordinate &cord,
                                                   bool gettingBackground) {
    unsigned int interactions = 0;
    try {
        if (gettingBackground)
            interactions += (material == dis.at(flat::IntegerCoordinate{cord.x, cord.y + 1}).floorMaterial) << 3;
        else
            interactions += (material == dis.at(flat::IntegerCoordinate{cord.x, cord.y + 1}).wallMaterial) << 3;
    } catch (const std::out_of_range &e) {}

    try {
        if (gettingBackground)
            interactions += (material == dis.at(flat::IntegerCoordinate{cord.x, cord.y - 1}).floorMaterial) << 2;
        else
            interactions += (material == dis.at(flat::IntegerCoordinate{cord.x, cord.y - 1}).wallMaterial) << 2;
    } catch (const std::out_of_range &e) {
    }

    try {
        if (gettingBackground)
            interactions += (material == dis.at(flat::IntegerCoordinate{cord.x + 1, cord.y}).floorMaterial) << 1;
        else
            interactions += (material == dis.at(flat::IntegerCoordinate{cord.x + 1, cord.y}).wallMaterial) << 1;
    } catch (const std::out_of_range &e) {}

    try {
        if (gettingBackground)
            interactions += (material == dis.at(flat::IntegerCoordinate{cord.x - 1, cord.y}).floorMaterial) << 0;
        else
            interactions += (material == dis.at(flat::IntegerCoordinate{cord.x - 1, cord.y}).wallMaterial) << 0;
    } catch (const std::out_of_range &e) {}

    switch (interactions) {
        case 1:
            return frameDefs.get(spriteInteraction.W);
            break;
        case 2:
            return frameDefs.get(spriteInteraction.E);
            break;
        case 3:
            return frameDefs.get(spriteInteraction.EW);
            break;
        case 4:
            return frameDefs.get(spriteInteraction.S);
            break;
        case 5:
            return frameDefs.get(spriteInteraction.SW);
            break;
        case 6:
            return frameDefs.get(spriteInteraction.SE);
            break;
        case 7:
            return frameDefs.get(spriteInteraction.SEW);
            break;
        case 8:
            return frameDefs.get(spriteInteraction.N);
            break;
        case 9:
            return frameDefs.get(spriteInteraction.NW);
            break;
        case 10:
            return frameDefs.get(spriteInteraction.NE);
            break;
        case 11:
            return frameDefs.get(spriteInteraction.NEW);
            break;
        case 12:
            return frameDefs.get(spriteInteraction.NS);
            break;
        case 13:
            return frameDefs.get(spriteInteraction.NSW);
            break;
        case 14:
            return frameDefs.get(spriteInteraction.NSE);
            break;
        case 15:
            return frameDefs.get(spriteInteraction.NSEW);
            break;
        default:
            return frameDefs.get(spriteInteraction.dDefault);
    }
}
 */

    std::optional<Jit::SpriteInteraction>
    JitSpriteInteractionsList::getFullSpriteInteraction(const std::string &input) {
        Jit::SpriteInteraction result;

        std::string relevant = getEncapsulatedContents(input, '[', ']', 1);
        relevant = getEncapsulatedContents(relevant, '[', ']', 1);

        for (unsigned int i = 0; i < 16; i++) {
            std::string tmp = getEncapsulatedContents(relevant, '[', ']', i);
            tmp = getEncapsulatedContents(relevant, '<', '>', i);
            switch (i) {
                case 0:
                    result.dDefault = tmp;
                    break;
                case 1:
                    result.W = tmp;
                    break;
                case 2:
                    result.E = tmp;
                    break;
                case 3:
                    result.EW = tmp;
                    break;
                case 4:
                    result.S = tmp;
                    break;
                case 5:
                    result.SW = tmp;
                    break;
                case 6:
                    result.NW = tmp;
                    break;
                case 7:
                    result.NSW = tmp;
                    break;
                case 8:
                    result.N = tmp;
                    break;
                case 9:
                    result.SE = tmp;
                    break;
                case 10:
                    result.NE = tmp;
                    break;
                case 11:
                    result.NSE = tmp;
                    break;
                case 12:
                    result.NS = tmp;
                    break;
                case 13:
                    result.SEW = tmp;
                    break;
                case 14:
                    result.NEW = tmp;
                    break;
                case 15:
                    result.NSEW = tmp;
                    break;
                default:; // Cry
            }
        }

        if (isFullyDefined(result)) {
            return result;
        } else {
            return std::nullopt;
        }
    }

    Jit::SpriteInteraction JitSpriteInteractionsList::getSimpleSpriteInteraction(const std::string &input) {
        std::string relevant = getEncapsulatedContents(input, '[', ']', 1);
        relevant = getEncapsulatedContents(relevant, '[', ']', 1);
        relevant = getEncapsulatedContents(relevant, '[', ']', 0);
        relevant = getEncapsulatedContents(relevant, '<', '>', 0);

        Jit::SpriteInteraction result;
        result.dDefault = relevant;
        result.W = relevant;
        result.E = relevant;
        result.EW = relevant;
        result.S = relevant;
        result.SW = relevant;
        result.NW = relevant;
        result.NSW = relevant;
        result.N = relevant;
        result.SE = relevant;
        result.NE = relevant;
        result.NSE = relevant;
        result.NS = relevant;
        result.SEW = relevant;
        result.NEW = relevant;
        result.NSEW = relevant;

        return result;
    }

    std::string JitSpriteInteractionsList::getSI_Key(const std::string &input) {
        std::string tmp = getEncapsulatedContents(input, '[', ']', 0);
        return getEncapsulatedContents(tmp, '<', '>', 0);
    }

    bool JitSpriteInteractionsList::isSimpleSI(const std::string &input) {
        std::string tmp = getEncapsulatedContents(input, '[', ']', 1);
        tmp = getEncapsulatedContents(tmp, '<', '>', 0);

        return tmp == "simple";
    }

    bool JitSpriteInteractionsList::isFullyDefined(const Jit::SpriteInteraction &SI) {
        if (SI.dDefault.empty() || SI.W.empty() || SI.E.empty() || SI.EW.empty() || SI.S.empty() ||
            SI.SW.empty() || SI.SE.empty() || SI.SEW.empty() || SI.N.empty() || SI.NW.empty() || SI.NE.empty() ||
            SI.NEW.empty() || SI.NS.empty() || SI.NSW.empty() || SI.NSE.empty() || SI.NSEW.empty())
            return false;
        else
            return true;
    }

    bool JitSpriteInteractionsList::isMultiLine(const std::string &input) {
        if (input.size() < 3)
            return false;

        auto max = input.size();

        return (input[max - 1] == '.') && (input[max - 2] == '.') && (input[max - 3] == '.');
    }

    std::optional<Jit::SpriteInteraction> JitSpriteInteractionsList::loadSI(const std::string &input) {
        std::optional<Jit::SpriteInteraction> result;

        try {
            if (isSimpleSI(input)) {
                result = getSimpleSpriteInteraction(input);
            } else {
                result = getFullSpriteInteraction(input);
            }

            if (result.has_value())
                result.value().name = getSI_Key(input);
        } catch (...) {
            return std::nullopt;
        }

        return std::optional<Jit::SpriteInteraction>(result);
    }

}
