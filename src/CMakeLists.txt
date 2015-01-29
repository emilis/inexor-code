## Definitions for both the Client and the Server

# Wrong folder: User attempted to generate from src instead of the main-inexor folder
if(NOT DEFINED PROJECT_ARCH)
    message(FATAL_ERROR "You are building from the wrong folder! \"Where is the Source:\" (currently: ${MAINDIR}) should point to the Main directory (!) of Inexor")
endif()

find_package(ZLIB REQUIRED)

find_package(OpenGL)
find_package(PkgConfig)

option(BUILD_CLIENT "Build the client" ${OPENGL_FOUND})
option(BUILD_SERVER "Build the server" 1)

message(STATUS "Detecting Libs...")

# Linux Libraries for Server AND Client
if(OS_LINUX)
    if(NOT BUILD_SDL2_LOCAL AND PKG_CONFIG_FOUND)
        message(STATUS "Using pkg-config to detect SDL2...")
        pkg_check_modules(SDL2 QUIET sdl2)
        if(SDL2_FOUND)
            message(STATUS "Found SDL2.")
            message(STATUS "include: ${SDL2_INCLUDE_DIRS}")
        else()
            message(FATAL_ERROR "SDL2 not found.")
        endif()
    else()
        if(PKG_CONFIG_FOUND)
            message(STATUS "Using local SDL2.")
        else()
            message(STATUS "Using local SDL2. (could not find PKG_CONFIG)")
        endif()
        set(SDL2_LIBRARIES "SDL2")
    endif()
    set(SDL2_LIBRARIES "${SDL2_LIBRARIES};SDL2_image;SDL2_mixer")
    add_definitions(-D_REENTRANT -D_THREAD_SAFE)
endif()

# MACOSX Libraries for Server AND Client
if(OS_MACOSX)
    find_library(SDL2_LIBRARY SDL2)
    find_library(SDL2_IMAGE_LIBRARY SDL2_image)
    find_library(SDL2_MIXER_LIBRARY SDL2_mixer)

    find_library(CORESERVICES_LIBRARY CoreServices)
    find_library(COCOA_LIBRARY Cocoa)

    set(SDL2_LIBRARIES "${SDL2_LIBRARY};${SDL2_MIXER_LIBRARY};${SDL2_IMAGE_LIBRARY};${CORESERVICES_LIBRARY};${COCOA_LIBRARY}")

    find_path(SDL2_INCLUDE_DIR SDL.h)
    find_path(SDL2_IMAGE_INCLUDE_DIR SDL2_image.h)
    find_path(SDL2_MIXER_INCLUDE_DIR SDL2_mixer.h)
    set(SDL2_INCLUDE_DIRS "${SDL2_INCLUDE_DIR};${SDL2_IMAGE_INCLUDE_DIR};${SDL2_MIXER_INCLUDE_DIR}")
endif()

# Windows Libraries for Server AND Client
if(OS_WINDOWS)
    if(NOT MSVC)
        set(BUILD_EXTRA_LIBS ${BUILD_EXTRA_LIBS} -static-libgcc)
    endif()
    set(BUILD_EXTRA_LIBS ${BUILD_EXTRA_LIBS} kernel32 ws2_32 winmm)
endif()

# ENet networking library
if(NOT DEFINED ENET_LIBRARIES AND NOT DEFINED LOCAL_ENET)
    set(LOCAL_ENET 1 CACHE INTERNAL "Use local enet library")
elseif(DEFINED LOCAL_ENET)
    set(LOCAL_ENET ${LOCAL_ENET} CACHE INTERNAL "Use local enet library")
else()
    set(LOCAL_ENET 0 CACHE INTERNAL "Use local enet library")
endif()

if(${LOCAL_ENET})
    message(STATUS "Building local ENet")
    add_subdirectory(libraries/enet)
endif()

# Set names of Source Files (prepend correct path)
function(create_names TARGET)
    message(STATUS "Appending names")
    string(TOUPPER "${TARGET}" TARGET)
    string(TOUPPER "${ARGN}" GROUPS)

    foreach(GROUP ${GROUPS})

        # Sources:
        set(SOURCE_GROUP_${GROUP} "")
        foreach(FILE ${${TARGET}_SOURCES_${GROUP}})
            set(SOURCE_GROUP_${GROUP}
                ${SOURCE_GROUP_${GROUP}}
                ${SOURCE_DIR}/${FILE})
            message(STATUS "Prepend path for ${FILE} (becoming ${SOURCE_DIR}/${FILE})")
        endforeach(FILE)

        # Replace old Source Array
        set(${TARGET}_SOURCES_${GROUP} ${SOURCE_GROUP_${GROUP}} PARENT_SCOPE)

        # For Visual Studio create filter groups as well
        if(MSVC)
            message(STATUS "Setting up filter groups.")

            # Prepended .cpp Extension needed
            set(SOURCE_GROUP_${GROUP}__ "")
            foreach(FILE ${SOURCE_GROUP_${GROUP}})
                set(SOURCE_GROUP_${GROUP}__ ${SOURCE_GROUP_${GROUP}__} "${FILE}.cpp")
            endforeach()

            # Since we add the headers as well, tell the parent to use those extensions (replace parent array)
            set(${TARGET}_SOURCES_${GROUP} ${SOURCE_GROUP_${GROUP}__} PARENT_SCOPE)

            # We want the Header files in our Solution Explorer as well:
            set(HEADER_GROUP_${GROUP} "")
            foreach(FILE ${${TARGET}_HEADERS_${GROUP}})
                set(HEADER_GROUP_${GROUP}
                    ${HEADER_GROUP_${GROUP}}
                    ${SOURCE_DIR}/${FILE}.h)
            endforeach(FILE)

            # Replace old Header Array
            set(${TARGET}_HEADERS_${GROUP} ${HEADER_GROUP_${GROUP}} PARENT_SCOPE)

            # Create filter:
            string(TOLOWER ${GROUP} GROUPNAME)
            source_group(${GROUPNAME} FILES ${SOURCE_GROUP_${GROUP}__} ${HEADER_GROUP_${GROUP}})
        endif()

    endforeach(GROUP)
endfunction()

include_directories(
    ${ENET_INCLUDE_DIRS}
    ${SDL2_INCLUDE_DIRS}
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${CMAKE_CURRENT_SOURCE_DIR}/shared
    ${CMAKE_CURRENT_SOURCE_DIR}/engine
    ${CMAKE_CURRENT_SOURCE_DIR}/fpsgame
)

if(${BUILD_CLIENT})
    add_subdirectory(client)
else()
    message(STATUS "not building the client")
endif()

if(${BUILD_SERVER})
    add_subdirectory(server)
else()
    message(STATUS "not building the server")
endif()
