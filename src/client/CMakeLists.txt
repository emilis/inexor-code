# Client project definition

set(CLIENT_SOURCES_SHARED
    shared/crypto
    shared/geom
    shared/json
    shared/stream
    shared/tools
    shared/zip
    )

set(CLIENT_SOURCES_ENGINE
    engine/3dgui
    engine/bih
    engine/blend
    engine/client
    engine/command
    engine/console
    engine/decal
    engine/dynlight
    engine/grass
    engine/main
    engine/material
    engine/menus
    engine/movie
    engine/normal
    engine/octa
    engine/octaedit
    engine/octarender
    engine/physics
    engine/pvs
    engine/rendergl
    engine/rendermodel
    engine/renderparticles
    engine/rendersky
    engine/rendertext
    engine/renderva
    engine/server
    engine/shader
    engine/sound
    engine/texture
    engine/water
    engine/world
    engine/worldio
    engine/serverbrowser
    engine/shadowmap
    engine/lightmap
    engine/glare
    engine/blob
    )
    
set( CLIENT_SOURCES_FPSGAME
    fpsgame/ai
    fpsgame/client       
    fpsgame/entities
    fpsgame/fps
    fpsgame/monster
    fpsgame/movable
    fpsgame/render
    fpsgame/scoreboard
    fpsgame/server
    fpsgame/waypoint
    fpsgame/weapon
    )

# Only used for MSVC
set(CLIENT_HEADERS_SHARED
    shared/command
    shared/cube
    shared/ents
    shared/geom
    shared/iengine
    shared/igame
    shared/json
    shared/tools
    )

set(CLIENT_HEADERS_ENGINE
    engine/animmodel
    engine/bih
    engine/depthfx
    engine/engine
    engine/explosion
    engine/iqm
    engine/lensflare
    engine/lightmap
    engine/lightning
    engine/md2
    engine/md3
    engine/md5
    engine/model
    engine/mpr
    engine/obj
    engine/octa
    engine/ragdoll
    engine/rendertarget
    engine/scale
    engine/skelmodel
    engine/smd
    engine/textedit
    engine/texture
    engine/varray
    engine/vertmodel
    engine/world
    )

set( CLIENT_HEADERS_FPSGAME
    fpsgame/ai
    fpsgame/aiman
    fpsgame/bomb
    fpsgame/capture
    fpsgame/collect
    fpsgame/ctf
    fpsgame/extinfo
    fpsgame/game
    fpsgame/hideandseek
    )

# Set Binary name
set(CLIENT_BINARY inexor CACHE INTERNAL "Client binary name.")

# Set Client definition
add_definitions(-DCLIENT)

# Prepend source folder to name 
# For VS create filters as well
create_names(CLIENT engine fpsgame shared)

set(CLIENT_SOURCES
    ${CLIENT_SOURCES_SHARED}
    ${CLIENT_SOURCES_ENGINE}
    ${CLIENT_SOURCES_FPSGAME})

# Linux Libraries and Configuration
if(OS_LINUX)
    set(BUILD_EXTRA_LIBS ${BUILD_EXTRA_LIBS} dl rt)
endif()

# MACOSX Libraries and Configuration
if(OS_MACOSX)
    set(CLIENT_SOURCES
        ${CLIENT_SOURCES}
        engine/macutils.mm)
endif()

# Windows Libraries and Configuration
if(OS_WINDOWS)
    set(BUILD_EXTRA_LIBS ${BUILD_EXTRA_LIBS} opengl32)

    if(MSVC)
        set(BUILD_EXTRA_LIBS ${BUILD_EXTRA_LIBS} dbghelp)

        set(CLIENT_HEADERS # just needed for the VS-filters
            ${CLIENT_HEADERS_SHARED}
            ${CLIENT_HEADERS_ENGINE}
            ${CLIENT_HEADERS_FPSGAME})
    else()
        add_definitions(-mwindows) 
    endif()
endif()


# Assemble executable in specific mode
if(OS_WINDOWS)
    add_executable(${CLIENT_BINARY} WIN32 ${CLIENT_SOURCES} ${CLIENT_HEADERS})
elseif(OS_MACOSX)
    add_executable(${CLIENT_BINARY} MACOSX_BUNDLE ${CLIENT_SOURCES})
else()
    add_executable(${CLIENT_BINARY} ${CLIENT_SOURCES})
endif()

target_link_libraries(${CLIENT_BINARY}
    ${ENET_LIBRARIES}
    ${SDL2_LIBRARIES}
    ${OPENGL_LIBRARIES}
    ${ZLIB_LIBRARIES}
    ${BUILD_EXTRA_LIBS}
)

if(MSVC)
    set_property(TARGET ${CLIENT_BINARY} PROPERTY FOLDER "executables")
endif()

# Install Binary
install(TARGETS ${CLIENT_BINARY}
        DESTINATION ${EXE_DIR})
