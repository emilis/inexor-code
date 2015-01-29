# Client Project Definitions

set( SERVER_SOURCES_SHARED
    shared/crypto
    shared/geom
    shared/stream
    shared/tools
    shared/zip )
    
set( SERVER_SOURCES_ENGINE
    engine/server
    engine/command
    engine/worldio )

set( SERVER_SOURCES_FPSGAME
    fpsgame/server
    fpsgame/entities )

# MSVC Filter groups need headers:
set( SERVER_HEADERS_SHARED
    shared/command
    shared/cube
    shared/ents
    shared/geom
    shared/iengine
    shared/igame
    shared/tools )

set( SERVER_HEADERS_ENGINE
    engine/engine
    engine/world )

set( SERVER_HEADERS_FPSGAME
    fpsgame/aiman
    fpsgame/bomb
    fpsgame/capture
    fpsgame/collect
    fpsgame/ctf
    fpsgame/extinfo
    fpsgame/game
    fpsgame/hideandseek )

# Set Binary name
set(SERVER_BINARY server CACHE INTERNAL "Server binary name.")

# Set Server Definition
add_definitions(-DSERVER -DSTANDALONE)

# Prepend source folder to name 
# For VS create filters as well
create_names(SERVER engine fpsgame shared)

set(SERVER_SOURCES
    ${SERVER_SOURCES_SHARED}
    ${SERVER_SOURCES_ENGINE}
    ${SERVER_SOURCES_FPSGAME})

# Add headers to the VS filter groups on windows
if(MSVC)
    set(SERVER_HEADERS
        ${SERVER_HEADERS_SHARED}
        ${SERVER_HEADERS_ENGINE}
        ${SERVER_HEADERS_FPSGAME})
endif()

# Linux Libraries and Configuration
if(OS_LINUX})
    set(BUILD_EXTRA_LIBS ${BUILD_EXTRA_LIBS} dl -pthread)
endif()

# Assemble executable in specific mode
if(OS_WINDOWS)
    add_executable(${SERVER_BINARY} WIN32 ${SERVER_SOURCES} ${SERVER_HEADERS})
elseif(OS_MACOSX)
    add_executable(${SERVER_BINARY} MACOSX_BUNDLE ${SERVER_SOURCES})
else()
    add_executable(${SERVER_BINARY} ${SERVER_SOURCES})
endif()

target_link_libraries(${SERVER_BINARY}
    ${ENET_LIBRARIES}
    ${OPENGL_LIBRARIES}
    ${ZLIB_LIBRARIES}
    ${EXTRA_LIBS})
    
if(MSVC)
    set_property(TARGET ${SERVER_BINARY} PROPERTY FOLDER "executables")
endif()

# Install Binary
install(TARGETS ${SERVER_BINARY}
        DESTINATION ${EXE_DIR})
