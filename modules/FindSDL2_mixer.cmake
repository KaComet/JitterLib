#------------------------------------------------------------------------------
# Usage: find_package(SDL2_MIXER [REQUIRED])
#
# Sets variables:
#     SDL2_MIXER_INCLUDE_DIRS
#     SDL2_MIXER_LIBRARIES
#     SDL2_MIXER_DLLS
#------------------------------------------------------------------------------

include(FindPackageHandleStandardArgs)

if (WIN32)
    # Search for SDL2_MIXER in extern/SDL2_mixer-2.0.0
    find_path(SDL2_MIXER_ROOT "include/SDL_mixer.h" PATHS "${CMAKE_CURRENT_LIST_DIR}/../extern/SDL2_mixer-2.0.0" NO_DEFAULT_PATH)
    if (SDL2_MIXER_ROOT)
        set(SDL2_MIXER_INCLUDE_DIRS "${SDL2_MIXER_ROOT}/include")
        if ("${CMAKE_GENERATOR}" MATCHES "Win64")
            set(SDL2_MIXER_LIBRARIES "${SDL2_MIXER_ROOT}/lib/x64/SDL2_mixer.lib")
            set(SDL2_MIXER_DLLS "${SDL2_MIXER_ROOT}/lib/x64/SDL2_MIXER.dll")
        else ()
            set(SDL2_MIXER_LIBRARIES "${SDL2_MIXER_ROOT}/lib/x86/SDL2_mixer.lib")
            set(SDL2_MIXER_DLLS "${SDL2_MIXER_ROOT}/lib/x86/SDL2_mixer.dll")
        endif ()
    endif ()

    mark_as_advanced(SDL2_MIXER_ROOT)
    find_package_handle_standard_args(SDL2_mixer DEFAULT_MSG SDL2_MIXER_INCLUDE_DIRS SDL2_MIXER_LIBRARIES SDL2_MIXER_DLLS)
else ()
    # On MacOS, should be installed via Macports
    # On Ubuntu, install with: apt-get install libsdl2-MIXER-dev
    if (NOT SDL2_MIXER_INCLUDE_DIR AND SDL2MIXER_INCLUDE_DIR)
        set(SDL2_MIXER_INCLUDE_DIR ${SDL2MIXER_INCLUDE_DIR} CACHE PATH "directory cache entry initialized from old variable name")
    endif ()
    find_path(SDL2_MIXER_INCLUDE_DIR SDL_mixer.h
            HINTS
            ENV SDL2MIXERDIR
            ENV SDL2DIR
            ${SDL2_DIR}
            PATH_SUFFIXES SDL2
            # path suffixes to search inside ENV{SDL2DIR}
            include/SDL2 include
            )

    if (CMAKE_SIZEOF_VOID_P EQUAL 8)
        set(VC_LIB_PATH_SUFFIX lib/x64)
    else ()
        set(VC_LIB_PATH_SUFFIX lib/x86)
    endif ()

    if (NOT SDL2_MIXER_DLLS AND SDL2MIXER_LIBRARY)
        set(SDL2_MIXER_DLLS ${SDL2MIXER_LIBRARY} CACHE FILEPATH "file cache entry initialized from old variable name")
    endif ()
    find_library(SDL2_MIXER_DLLS
            NAMES SDL2_mixer
            HINTS
            ENV SDL2MIXERDIR
            ENV SDL2DIR
            ${SDL2_DIR}
            PATH_SUFFIXES lib ${VC_LIB_PATH_SUFFIX}
            )

    if (SDL2_MIXER_INCLUDE_DIR AND EXISTS "${SDL2_MIXER_INCLUDE_DIR}/SDL2_mixer.h")
        file(STRINGS "${SDL2_MIXER_INCLUDE_DIR}/SDL2_mixer.h" SDL2_MIXER_VERSION_MAJOR_LINE REGEX "^#define[ \t]+SDL2_MIXER_MAJOR_VERSION[ \t]+[0-9]+$")
        file(STRINGS "${SDL2_MIXER_INCLUDE_DIR}/SDL2_mixer.h" SDL2_MIXER_VERSION_MINOR_LINE REGEX "^#define[ \t]+SDL2_MIXER_MINOR_VERSION[ \t]+[0-9]+$")
        file(STRINGS "${SDL2_MIXER_INCLUDE_DIR}/SDL2_mixer.h" SDL2_MIXER_VERSION_PATCH_LINE REGEX "^#define[ \t]+SDL2_MIXER_PATCHLEVEL[ \t]+[0-9]+$")
        string(REGEX REPLACE "^#define[ \t]+SDL2_MIXER_MAJOR_VERSION[ \t]+([0-9]+)$" "\\1" SDL2_MIXER_VERSION_MAJOR "${SDL2_MIXER_VERSION_MAJOR_LINE}")
        string(REGEX REPLACE "^#define[ \t]+SDL2_MIXER_MINOR_VERSION[ \t]+([0-9]+)$" "\\1" SDL2_MIXER_VERSION_MINOR "${SDL2_MIXER_VERSION_MINOR_LINE}")
        string(REGEX REPLACE "^#define[ \t]+SDL2_MIXER_PATCHLEVEL[ \t]+([0-9]+)$" "\\1" SDL2_MIXER_VERSION_PATCH "${SDL2_MIXER_VERSION_PATCH_LINE}")
        set(SDL2_MIXER_VERSION_STRING ${SDL2_MIXER_VERSION_MAJOR}.${SDL2_MIXER_VERSION_MINOR}.${SDL2_MIXER_VERSION_PATCH})
        unset(SDL2_MIXER_VERSION_MAJOR_LINE)
        unset(SDL2_MIXER_VERSION_MINOR_LINE)
        unset(SDL2_MIXER_VERSION_PATCH_LINE)
        unset(SDL2_MIXER_VERSION_MAJOR)
        unset(SDL2_MIXER_VERSION_MINOR)
        unset(SDL2_MIXER_VERSION_PATCH)
    endif ()

    set(SDL2_MIXER_LIBRARIES ${SDL2_MIXER_DLLS})
    set(SDL2_MIXER_INCLUDE_DIRS ${SDL2_MIXER_INCLUDE_DIR})

    include(FindPackageHandleStandardArgs)

    FIND_PACKAGE_HANDLE_STANDARD_ARGS(SDL2_mixer
            REQUIRED_VARS SDL2_MIXER_LIBRARIES SDL2_MIXER_INCLUDE_DIRS
            VERSION_VAR SDL2_MIXER_VERSION_STRING)

    # for backward compatibility
    set(SDL2MIXER_LIBRARY ${SDL2_MIXER_LIBRARIES})
    set(SDL2MIXER_INCLUDE_DIR ${SDL2_MIXER_INCLUDE_DIRS})
    set(SDL2MIXER_FOUND ${SDL2_MIXER_FOUND})

    mark_as_advanced(SDL2_MIXER_DLLS SDL2_MIXER_INCLUDE_DIR)
endif ()