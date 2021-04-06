#------------------------------------------------------------------------------
# Usage: find_package(SDL2_image [REQUIRED])
#
# Sets variables:
#     SDL2_IMAGE_INCLUDE_DIRS
#     SDL2_IMAGE_LIBRARIES
#     SDL2_IMAGE_DLLS
#------------------------------------------------------------------------------

include(FindPackageHandleStandardArgs)

if (WIN32)
    # Search for SDL2_image in extern/SDL2_image-2.0.5
    find_path(SDL2_IMAGE_ROOT "include/SDL_image.h" PATHS "${CMAKE_CURRENT_LIST_DIR}/../extern/SDL2_image-2.0.5" NO_DEFAULT_PATH)
    if (SDL2_IMAGE_ROOT)
        set(SDL2_IMAGE_INCLUDE_DIRS "${SDL2_IMAGE_ROOT}/include")
        if ("${CMAKE_GENERATOR}" MATCHES "Win64")
            set(SDL2_IMAGE_LIBRARIES "${SDL2_IMAGE_ROOT}/lib/x64/SDL2_image.lib")
            set(SDL2_IMAGE_DLLS "${SDL2_IMAGE_ROOT}/lib/x64/SDL2_image.dll")
        else ()
            set(SDL2_IMAGE_LIBRARIES "${SDL2_IMAGE_ROOT}/lib/x86/SDL2_image.lib")
            set(SDL2_IMAGE_DLLS "${SDL2_IMAGE_ROOT}/lib/x86/SDL2_image.dll")
        endif ()
    endif ()

    mark_as_advanced(SDL2_IMAGE_ROOT)
    find_package_handle_standard_args(SDL2_image DEFAULT_MSG SDL2_IMAGE_INCLUDE_DIRS SDL2_IMAGE_LIBRARIES SDL2_IMAGE_DLLS)
else ()
    # On MacOS, should be installed via Macports
    # On Ubuntu, install with: apt-get install libsdl2-image-dev
    if (NOT SDL2_IMAGE_INCLUDE_DIR AND SDL2IMAGE_INCLUDE_DIR)
        set(SDL2_IMAGE_INCLUDE_DIR ${SDL2IMAGE_INCLUDE_DIR} CACHE PATH "directory cache entry initialized from old variable name")
    endif ()
    find_path(SDL2_IMAGE_INCLUDE_DIR SDL_image.h
            HINTS
            ENV SDL2IMAGEDIR
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

    if (NOT SDL2_IMAGE_LIBRARY AND SDL2IMAGE_LIBRARY)
        set(SDL2_IMAGE_LIBRARY ${SDL2IMAGE_LIBRARY} CACHE FILEPATH "file cache entry initialized from old variable name")
    endif ()
    find_library(SDL2_IMAGE_LIBRARY
            NAMES SDL2_image
            HINTS
            ENV SDL2IMAGEDIR
            ENV SDL2DIR
            ${SDL2_DIR}
            PATH_SUFFIXES lib ${VC_LIB_PATH_SUFFIX}
            )

    if (SDL2_IMAGE_INCLUDE_DIR AND EXISTS "${SDL2_IMAGE_INCLUDE_DIR}/SDL2_image.h")
        file(STRINGS "${SDL2_IMAGE_INCLUDE_DIR}/SDL2_image.h" SDL2_IMAGE_VERSION_MAJOR_LINE REGEX "^#define[ \t]+SDL2_IMAGE_MAJOR_VERSION[ \t]+[0-9]+$")
        file(STRINGS "${SDL2_IMAGE_INCLUDE_DIR}/SDL2_image.h" SDL2_IMAGE_VERSION_MINOR_LINE REGEX "^#define[ \t]+SDL2_IMAGE_MINOR_VERSION[ \t]+[0-9]+$")
        file(STRINGS "${SDL2_IMAGE_INCLUDE_DIR}/SDL2_image.h" SDL2_IMAGE_VERSION_PATCH_LINE REGEX "^#define[ \t]+SDL2_IMAGE_PATCHLEVEL[ \t]+[0-9]+$")
        string(REGEX REPLACE "^#define[ \t]+SDL2_IMAGE_MAJOR_VERSION[ \t]+([0-9]+)$" "\\1" SDL2_IMAGE_VERSION_MAJOR "${SDL2_IMAGE_VERSION_MAJOR_LINE}")
        string(REGEX REPLACE "^#define[ \t]+SDL2_IMAGE_MINOR_VERSION[ \t]+([0-9]+)$" "\\1" SDL2_IMAGE_VERSION_MINOR "${SDL2_IMAGE_VERSION_MINOR_LINE}")
        string(REGEX REPLACE "^#define[ \t]+SDL2_IMAGE_PATCHLEVEL[ \t]+([0-9]+)$" "\\1" SDL2_IMAGE_VERSION_PATCH "${SDL2_IMAGE_VERSION_PATCH_LINE}")
        set(SDL2_IMAGE_VERSION_STRING ${SDL2_IMAGE_VERSION_MAJOR}.${SDL2_IMAGE_VERSION_MINOR}.${SDL2_IMAGE_VERSION_PATCH})
        unset(SDL2_IMAGE_VERSION_MAJOR_LINE)
        unset(SDL2_IMAGE_VERSION_MINOR_LINE)
        unset(SDL2_IMAGE_VERSION_PATCH_LINE)
        unset(SDL2_IMAGE_VERSION_MAJOR)
        unset(SDL2_IMAGE_VERSION_MINOR)
        unset(SDL2_IMAGE_VERSION_PATCH)
    endif ()

    set(SDL2_IMAGE_LIBRARIES ${SDL2_IMAGE_LIBRARY})
    set(SDL2_IMAGE_INCLUDE_DIRS ${SDL2_IMAGE_INCLUDE_DIR})

    include(FindPackageHandleStandardArgs)

    FIND_PACKAGE_HANDLE_STANDARD_ARGS(SDL2_image
            REQUIRED_VARS SDL2_IMAGE_LIBRARIES SDL2_IMAGE_INCLUDE_DIRS
            VERSION_VAR SDL2_IMAGE_VERSION_STRING)

    # for backward compatibility
    set(SDL2IMAGE_LIBRARY ${SDL2_IMAGE_LIBRARIES})
    set(SDL2IMAGE_INCLUDE_DIR ${SDL2_IMAGE_INCLUDE_DIRS})
    set(SDL2IMAGE_FOUND ${SDL2_IMAGE_FOUND})

    mark_as_advanced(SDL2_IMAGE_LIBRARY SDL2_IMAGE_INCLUDE_DIR)
endif ()