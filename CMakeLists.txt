cmake_minimum_required(VERSION 3.17)
project(JitterLib_TilesetViewerMain)

set(CMAKE_CXX_STANDARD 17)

# Make sure the output binary will be placed in the bin directory.
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/bin)

# Find SDL2 and SDL2_image.
# sudo apt-get install libsdl2-2.0-0 libsdl2-dev libsdl2-image-dev
set(CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/Modules")
find_package(SDL2 REQUIRED COMPONENTS main)
find_package(SDL2_image REQUIRED COMPONENTS main)

# Define executable target.
include_directories(${SDL2_IMAGE_INCLUDE_DIRS}
        ${SDL2_INCLUDE_DIRS}
        ${SDL2main_INCLUDE_DIRS}
        ${CMAKE_BINARY_DIR}
        ${CMAKE_SOURCE_DIR}/include)

add_subdirectory(src)

set(JitterLibFiles src/ColorList.h
        src/ColorList.cpp
        src/resource.h
        src/resource.cpp
        src/LTexture.h
        src/LTexture.cpp
        src/ltimer.h
        src/ltimer.cpp
        src/SpriteInteractionsList.h
        src/SpriteInteractionsList.cpp
        src/SpriteSet.h
        src/SpriteSet.cpp
        src/UIBasics.h
        src/FrameDefMap.cpp
        src/FrameDefMap.h
        src/Tile.h
        src/Material.h
        src/DisplayData.h
        src/DisplayData.cpp
        src/Basics.h)

set(Flat include/FlatMath.h
        include/FlatTypes.h
        include/FlatTypes.cpp
        include/FlatMath.cpp
        include/FlatTypes.h
        include/FlatTypes.cpp
        include/FlatVector.h
        include/HappyArray.h)

add_executable(JitterLib_TilesetViewerMain Examples/TilesetViewerMain.cpp ${JitterLibFiles} ${Flat})
target_link_libraries(JitterLib_TilesetViewerMain ${SDL2_LIBS} ${SDL2_IMAGE_LIBRARIES})