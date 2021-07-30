cmake_minimum_required(VERSION 3.17)
project(JitterLib_TilesetViewerMain)

set(CMAKE_CXX_STANDARD 20)

# Make sure the output binary will be placed in the bin directory.
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/bin)

# Find SDL2 and SDL2_image.
# sudo apt-get install libsdl2-2.0-0 libsdl2-dev libsdl2-image-dev
set(CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/modules")
find_package(SDL2 REQUIRED COMPONENTS main)
find_package(SDL2_image REQUIRED COMPONENTS main)
find_package(SDL2_mixer REQUIRED COMPONENTS main)
find_package(SDL2_ttf REQUIRED COMPONENTS main)

# Define executable target.
include_directories(${SDL2_IMAGE_INCLUDE_DIRS}
        ${SDL2_MIXER_INCLUDE_DIRS}
        ${SDL2_TTF_INCLUDE_DIRS}
        ${SDL2_INCLUDE_DIRS}
        ${SDL2main_INCLUDE_DIRS}
        ${CMAKE_BINARY_DIR}
        ${CMAKE_SOURCE_DIR}/include/FlatLib)

set(JitterLibFiles src/JitColorList.h
        src/JitColorList.cpp
        src/JitResource.h
        src/JitResource.cpp
        src/JitLTexture.h
        src/JitLTexture.cpp
        src/JitLTimer.h
        src/JitLTimer.cpp
        src/JitSpriteInteractionsList.h
        src/JitSpriteInteractionsList.cpp
        src/JitSpriteSet.h
        src/JitSpriteSet.cpp
        src/JitUIBasics.h
        src/JitFrameDefMap.cpp
        src/JitFrameDefMap.h
        src/JitDisplayTile.h
        src/JitDisplayMaterial.h
        src/JitBasics.h
        src/JitSoundTrack.cpp
        src/JitSoundTrack.h)

set(Flat include/FlatLib/FlatMath.h
        include/FlatLib/FlatTypes.h
        include/FlatLib/FlatTypes.cpp
        include/FlatLib/FlatMath.cpp
        include/FlatLib/FlatTypes.h
        include/FlatLib/FlatTypes.cpp
        include/FlatLib/FlatVector.h
        include/FlatLib/HappyArray.h)

add_executable(JitterLib_TilesetViewerMain Examples/TilesetViewerMain.cpp ${JitterLibFiles} ${Flat})
target_link_libraries(JitterLib_TilesetViewerMain ${SDL2_LIBS} ${SDL2_IMAGE_LIBRARIES} ${SDL2_MIXER_LIBRARIES} ${SDL2_TTF_LIBRARIES})

add_executable(JitterLib_SimpleLoad Examples/SimpleLoad.cpp ${JitterLibFiles} ${Flat} src/JitFontTextureFactory.cpp src/JitFontTextureFactory.h src/IJitFontRenderedBase.h src/renderedFonts/JitSimpleText.cpp src/renderedFonts/JitSimpleText.h)
target_link_libraries(JitterLib_SimpleLoad ${SDL2_LIBS} ${SDL2_IMAGE_LIBRARIES} ${SDL2_MIXER_LIBRARIES} ${SDL2_TTF_LIBRARIES})