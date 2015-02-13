# Client Project Definitions

set( SERVER_SOURCES_SHARED
    shared/crypto.cpp
    shared/geom.cpp
    shared/stream.cpp
    shared/tools.cpp
    shared/zip.cpp )

set( SERVER_SOURCES_ENGINE
    engine/server.cpp
    engine/command.cpp
    engine/worldio.cpp )

set( SERVER_SOURCES_FPSGAME
    fpsgame/server.cpp
    fpsgame/entities.cpp )

set( SERVER_SOURCES_RPC
    rpc/inexor_service.pb.cc
    rpc/InexorServiceImpl.cpp
    rpc/MCRpcServer.cpp
    rpc/rpc.cpp )

set( SERVER_SOURCES_NET
    net/MCHub.cpp
    net/MCByteBuffer.cpp )

# MSVC Filter groups need headers:
set( SERVER_HEADERS_SHARED
    shared/command.h
    shared/cube.h
    shared/ents.h
    shared/geom.h
    shared/iengine.h
    shared/igame.h
    shared/tools.h )

set( SERVER_HEADERS_ENGINE
    engine/engine.h
    engine/world.h )

set( SERVER_HEADERS_FPSGAME
    fpsgame/aiman.h
    fpsgame/bomb.h
    fpsgame/capture.h
    fpsgame/collect.h
    fpsgame/ctf.h
    fpsgame/extinfo.h
    fpsgame/game.h
    fpsgame/hideandseek.h )

set( SERVER_HEADERS_RPC
    rpc/inexor_service.pb.h
    rpc/InexorServiceImpl.h
    rpc/MCRpcServer.h
    rpc/rpc.h
    rpc/rpc_sb_compat.h )

set( SERVER_HEADERS_NET
    net/MCByteBuffer.h
    net/MCHub.h
    net/MCServer.h
    net/MCSocket.h
    net/MCSocketServer.h
    net/MessageConnect.h
    net/net.h )

# Generate the protobuf c++ header
set_source_files_properties(
    ${SOURCE_DIR}/rpc/inexor_service.pb.cc
    PROPERTIES GENERATED TRUE)
    
# Set Binary name
set(SERVER_BINARY server CACHE INTERNAL "Server binary name.")

# Set Server Definition
add_definitions(-DSERVER -DSTANDALONE)

# Prepend source folder to name
# For VS create filters as well
create_names(SERVER engine fpsgame shared rpc net)

set(SERVER_SOURCES
    ${SERVER_SOURCES_SHARED}
    ${SERVER_SOURCES_ENGINE}
    ${SERVER_SOURCES_FPSGAME}
    ${SERVER_SOURCES_RPC}
    ${SERVER_SOURCES_NET})

# Add headers to the VS filter groups on windows
if(MSVC)
    set(SERVER_HEADERS
        ${SERVER_HEADERS_SHARED}
        ${SERVER_HEADERS_ENGINE}
        ${SERVER_HEADERS_FPSGAME}
        ${SERVER_HEADERS_RPC}
        ${SERVER_HEADERS_NET})
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
    ${BUILD_EXTRA_LIBS}
    ${PROTOBUF_LIBRARIES}
)

# generate protobuf c++ headers/sources first
add_dependencies(${SERVER_BINARY} inexor_rpc_protobuf)

if(MSVC)
    set_property(TARGET ${SERVER_BINARY} PROPERTY FOLDER "executables")
endif()

# Install Binary
install(TARGETS ${SERVER_BINARY}
        DESTINATION ${EXE_DIR})
