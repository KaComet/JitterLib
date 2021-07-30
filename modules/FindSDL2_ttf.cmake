#------------------------------------------------------------------------------
# Usage: find_package(SDL2_TTF [REQUIRED])
#
# Sets variables:
#     SDL2_TTF_INCLUDE_DIRS
#     SDL2_TTF_LIBRARIES
#     SDL2_TTF_DLLS
#------------------------------------------------------------------------------

include(FindPackageHandleStandardArgs)

if (WIN32)
    # Search for SDL2_TTF in extern/SDL2_ttf-2.0.0
    find_path(SDL2_TTF_ROOT "include/SDL_ttf.h" PATHS "${CMAKE_CURRENT_LIST_DIR}/../extern/SDL2_ttf-2.0.0" NO_DEFAULT_PATH)
    if (SDL2_TTF_ROOT)
        set(SDL2_TTF_INCLUDE_DIRS "${SDL2_TTF_ROOT}/include")
        if ("${CMAKE_GENERATOR}" MATCHES "Win64")
            set(SDL2_TTF_LIBRARIES "${SDL2_TTF_ROOT}/lib/x64/SDL2_ttf.lib")
            set(SDL2_TTF_DLLS "${SDL2_TTF_ROOT}/lib/x64/SDL2_TTF.dll")
        else ()
            set(SDL2_TTF_LIBRARIES "${SDL2_TTF_ROOT}/lib/x86/SDL2_ttf.lib")
            set(SDL2_TTF_DLLS "${SDL2_TTF_ROOT}/lib/x86/SDL2_ttf.dll")
        endif ()
    endif ()

    mark_as_advanced(SDL2_TTF_ROOT)
    find_package_handle_standard_args(SDL2_ttf DEFAULT_MSG SDL2_TTF_INCLUDE_DIRS SDL2_TTF_LIBRARIES SDL2_TTF_DLLS)
else ()
    # On MacOS, should be installed via Macports
    # On Ubuntu, install with: apt-get install libsdl2-TTF-dev
    if (NOT SDL2_TTF_INCLUDE_DIR AND SDL2TTF_INCLUDE_DIR)
        set(SDL2_TTF_INCLUDE_DIR ${SDL2TTF_INCLUDE_DIR} CACHE PATH "directory cache entry initialized from old variable name")
    endif ()
    find_path(SDL2_TTF_INCLUDE_DIR SDL_ttf.h
            HINTS
            ENV SDL2TTFDIR
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

    if (NOT SDL2_TTF_DLLS AND SDL2TTF_LIBRARY)
        set(SDL2_TTF_DLLS ${SDL2TTF_LIBRARY} CACHE FILEPATH "file cache entry initialized from old variable name")
    endif ()
    find_library(SDL2_TTF_DLLS
            NAMES SDL2_ttf
            HINTS
            ENV SDL2TTFDIR
            ENV SDL2DIR
            ${SDL2_DIR}
            PATH_SUFFIXES lib ${VC_LIB_PATH_SUFFIX}
            )

    if (SDL2_TTF_INCLUDE_DIR AND EXISTS "${SDL2_TTF_INCLUDE_DIR}/SDL2_ttf.h")
        file(STRINGS "${SDL2_TTF_INCLUDE_DIR}/SDL2_ttf.h" SDL2_TTF_VERSION_MAJOR_LINE REGEX "^#define[ \t]+SDL2_TTF_MAJOR_VERSION[ \t]+[0-9]+$")
        file(STRINGS "${SDL2_TTF_INCLUDE_DIR}/SDL2_ttf.h" SDL2_TTF_VERSION_MINOR_LINE REGEX "^#define[ \t]+SDL2_TTF_MINOR_VERSION[ \t]+[0-9]+$")
        file(STRINGS "${SDL2_TTF_INCLUDE_DIR}/SDL2_ttf.h" SDL2_TTF_VERSION_PATCH_LINE REGEX "^#define[ \t]+SDL2_TTF_PATCHLEVEL[ \t]+[0-9]+$")
        string(REGEX REPLACE "^#define[ \t]+SDL2_TTF_MAJOR_VERSION[ \t]+([0-9]+)$" "\\1" SDL2_TTF_VERSION_MAJOR "${SDL2_TTF_VERSION_MAJOR_LINE}")
        string(REGEX REPLACE "^#define[ \t]+SDL2_TTF_MINOR_VERSION[ \t]+([0-9]+)$" "\\1" SDL2_TTF_VERSION_MINOR "${SDL2_TTF_VERSION_MINOR_LINE}")
        string(REGEX REPLACE "^#define[ \t]+SDL2_TTF_PATCHLEVEL[ \t]+([0-9]+)$" "\\1" SDL2_TTF_VERSION_PATCH "${SDL2_TTF_VERSION_PATCH_LINE}")
        set(SDL2_TTF_VERSION_STRING ${SDL2_TTF_VERSION_MAJOR}.${SDL2_TTF_VERSION_MINOR}.${SDL2_TTF_VERSION_PATCH})
        unset(SDL2_TTF_VERSION_MAJOR_LINE)
        unset(SDL2_TTF_VERSION_MINOR_LINE)
        unset(SDL2_TTF_VERSION_PATCH_LINE)
        unset(SDL2_TTF_VERSION_MAJOR)
        unset(SDL2_TTF_VERSION_MINOR)
        unset(SDL2_TTF_VERSION_PATCH)
    endif ()

    set(SDL2_TTF_LIBRARIES ${SDL2_TTF_DLLS})
    set(SDL2_TTF_INCLUDE_DIRS ${SDL2_TTF_INCLUDE_DIR})

    include(FindPackageHandleStandardArgs)

    FIND_PACKAGE_HANDLE_STANDARD_ARGS(SDL2_ttf
            REQUIRED_VARS SDL2_TTF_LIBRARIES SDL2_TTF_INCLUDE_DIRS
            VERSION_VAR SDL2_TTF_VERSION_STRING)

    # for backward compatibility
    set(SDL2TTF_LIBRARY ${SDL2_TTF_LIBRARIES})
    set(SDL2TTF_INCLUDE_DIR ${SDL2_TTF_INCLUDE_DIRS})
    set(SDL2TTF_FOUND ${SDL2_TTF_FOUND})

    mark_as_advanced(SDL2_TTF_DLLS SDL2_TTF_INCLUDE_DIR)
endif ()