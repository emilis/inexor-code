# Client project definition

set(CLIENT_SOURCES_SHARED
    shared/crypto.cpp
    shared/geom.cpp
    shared/json.cpp
    shared/stream.cpp
    shared/tools.cpp
    shared/zip.cpp
    )

set(CLIENT_SOURCES_MUPARSER
    libraries/muparser/src/muParser.cpp
    libraries/muparser/src/muParserBase.cpp
    libraries/muparser/src/muParserBytecode.cpp
    libraries/muparser/src/muParserCallback.cpp
    libraries/muparser/src/muParserError.cpp
    libraries/muparser/src/muParserInt.cpp
    libraries/muparser/src/muParserTokenReader.cpp
    )

set(CLIENT_SOURCES_ENGINE
    engine/bih.cpp
    engine/blend.cpp
    engine/client.cpp
    engine/command.cpp
    engine/console.cpp
    engine/decal.cpp
    engine/dynlight.cpp
    engine/grass.cpp
    engine/main.cpp
    engine/material.cpp
    engine/menus.cpp
    engine/movie.cpp
    engine/normal.cpp
    engine/octa.cpp
    engine/octaedit.cpp
    engine/octarender.cpp
    engine/physics.cpp
    engine/pvs.cpp
    engine/rendergl.cpp
    engine/rendermodel.cpp
    engine/renderparticles.cpp
    engine/rendersky.cpp
    engine/rendertext.cpp
    engine/renderva.cpp
    engine/server.cpp
    engine/shader.cpp
    engine/sound.cpp
    engine/texture.cpp
    engine/water.cpp
    engine/world.cpp
    engine/worldio.cpp
    engine/serverbrowser.cpp
    engine/shadowmap.cpp
    engine/lightmap.cpp
    engine/glare.cpp
    engine/blob.cpp
    )

set( CLIENT_SOURCES_FPSGAME
    fpsgame/ai.cpp
    fpsgame/client.cpp
    fpsgame/entities.cpp
    fpsgame/fps.cpp
    fpsgame/monster.cpp
    fpsgame/movable.cpp
    fpsgame/render.cpp
    fpsgame/scoreboard.cpp
    fpsgame/server.cpp
    fpsgame/waypoint.cpp
    fpsgame/weapon.cpp
    )

set(CLIENT_SOURCES_UI
    ui/cefapp.cpp
    ui/cefconsole.cpp
    ui/cefcontextmanager.cpp
    ui/cefcontextprovider.cpp
    ui/cefdebug.cpp
    ui/cefdebugoverlay.cpp
    ui/cefevent.cpp
    ui/cefeventmanager.cpp
    ui/cefgame.cpp
    ui/cefkeyboard.cpp
    ui/ceflayer.cpp
    ui/ceflayermanager.cpp
    ui/cefnetwork.cpp
    ui/cefmenu.cpp
    ui/cefmouse.cpp
    ui/cefrenderhandler.cpp
    ui/cefrequestcontexthandler.cpp
    )

set( CLIENT_SOURCES_RPC
    rpc/inexor_service.pb.cc
    rpc/InexorServiceImpl.cpp
    rpc/MCRpcServer.cpp
    rpc/rpc.cpp
    )

set( CLIENT_SOURCES_NET
    net/MCHub.cpp
    net/MCByteBuffer.cpp
    )

# Only used for MSVC
set(CLIENT_HEADERS_SHARED
    shared/command.h
    shared/cube.h
    shared/ents.h
    shared/geom.h
    shared/iengine.h
    shared/igame.h
    shared/json.h
    shared/tools.h
    )

set(CLIENT_HEADERS_MUPARSER
    libraries/muparser/include/muParser.h
    libraries/muparser/include/muParserBase.h
    libraries/muparser/include/muParserBytecode.h
    libraries/muparser/include/muParserCallback.h
    libraries/muparser/include/muParserDef.h
    libraries/muparser/include/muParserError.h
    libraries/muparser/include/muParserFixes.h
    libraries/muparser/include/muParserStack.h
    libraries/muparser/include/muParserToken.h
    libraries/muparser/include/muParserTokenReader.h
    )

set(CLIENT_HEADERS_ENGINE
    engine/animmodel.h
    engine/bih.h
    engine/depthfx.h
    engine/engine.h
    engine/explosion.h
    engine/iqm.h
    engine/lensflare.h
    engine/lightmap.h
    engine/lightning.h
    engine/md2.h
    engine/md3.h
    engine/md5.h
    engine/model.h
    engine/mpr.h
    engine/obj.h
    engine/octa.h
    engine/ragdoll.h
    engine/rendertarget.h
    engine/scale.h
    engine/skelmodel.h
    engine/smd.h
    engine/textedit.h
    engine/texture.h
    engine/varray.h
    engine/vertmodel.h
    engine/world.h
    )

set( CLIENT_HEADERS_FPSGAME
    fpsgame/ai.h
    fpsgame/aiman.h
    fpsgame/bomb.h
    fpsgame/capture.h
    fpsgame/collect.h
    fpsgame/ctf.h
    fpsgame/extinfo.h
    fpsgame/game.h
    fpsgame/hideandseek.h
    )

set( CLIENT_HEADERS_UI
    ui/cefapp.h
    ui/cefbrowsersettings.h
    ui/cefconsole.h
    ui/cefcontextbindings.h
    ui/cefcontextmanager.h
    ui/cefcontextprovider.h
    ui/cefdebug.h
    ui/cefdebugoverlay.h
    ui/cefevent.h
    ui/cefeventmanager.h
    ui/cefgame.h
    ui/cefkeyboard.h
    ui/ceflayer.h
    ui/ceflayermanager.h
    ui/ceflayerprovider.h
    ui/cefmenu.h
    ui/cefmouse.h
    ui/cefnetwork.h
    ui/cefrenderhandler.h
    ui/cefrequestcontexthandler.h
    ui/cefsettings.h
    ui/ceftypes.h
    ui/cefwindowinfo.h
    ui/ui.h
    )

set( CLIENT_HEADERS_RPC
    rpc/inexor_service.pb.h
    rpc/InexorServiceImpl.h
    rpc/MCRpcServer.h
    rpc/rpc.h
    rpc/rpc_sb_compat.h 
    )

set( CLIENT_HEADERS_NET
    net/MCByteBuffer.h
    net/MCHub.h
    net/MCServer.h
    net/MCSocket.h
    net/MCSocketServer
    net/MessageConnect.h
    net/net.h 
    )

# Set Binary name
set(CLIENT_BINARY inexor CACHE INTERNAL "Client binary name.")

# Set Client definition
add_definitions(-DCLIENT)

# Prepend source folder to name
# For VS create filters as well
create_names(CLIENT engine fpsgame shared ui muparser rpc net)

set(CLIENT_SOURCES
    ${CLIENT_SOURCES_SHARED}
    ${CLIENT_SOURCES_ENGINE}
    ${CLIENT_SOURCES_UI}
    ${CLIENT_SOURCES_FPSGAME}
    ${CLIENT_SOURCES_MUPARSER}
    ${CLIENT_SOURCES_NET}
    ${CLIENT_SOURCES_RPC})

# Linux Libraries and Configuration
if(OS_LINUX)
    set(ADDITIONAL_LIBRARIES ${ADDITIONAL_LIBRARIES} dl rt)
endif()

# MACOSX Libraries and Configuration
if(OS_MACOSX)
    set(CLIENT_SOURCES
        ${CLIENT_SOURCES}
        engine/macutils.mm)
endif()

# Windows Libraries and Configuration
if(OS_WINDOWS)
    set(ADDITIONAL_LIBRARIES ${ADDITIONAL_LIBRARIES} opengl32)

    if(MSVC)
        set(ADDITIONAL_LIBRARIES ${ADDITIONAL_LIBRARIES} dbghelp)

        set(CLIENT_HEADERS # just needed for the VS-filters
            ${CLIENT_HEADERS_SHARED}
            ${CLIENT_HEADERS_ENGINE}
            ${CLIENT_HEADERS_FPSGAME}
            ${CLIENT_HEADERS_UI}
            ${CLIENT_HEADERS_MUPARSER}
            ${CLIENT_HEADERS_RPC}
            ${CLIENT_HEADERS_NET})
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
    ${CEF_LIBRARIES}
    ${PROTOBUF_LIBRARIES}
    ${ADDITIONAL_LIBRARIES}
)

# C++ Libraries depending on the stdlib are NOT binary compatible for different VS versions. (Thats also why CEF uses a C++-Wrapper)
# Also a debug .lib is incompatible to Inexor in releasemode then.
if(MSVC)
    target_link_libraries(${CLIENT_BINARY} debug ${PROTOBUF_LIBRARIES_MSVC_DEBUG})
    target_link_libraries(${CLIENT_BINARY} optimized ${PROTOBUF_LIBRARIES_MSVC_RELEASE})
    add_definitions(-DPROTOBUF_USE_DLLS)
endif()

# Status Message
message(STATUS "ENET_LIBRARIES:                 ${ENET_LIBRARIES}")
message(STATUS "SDL2_LIBRARIES:                 ${SDL2_LIBRARIES}")
message(STATUS "OPENGL_LIBRARIES:               ${OPENGL_LIBRARIES}")
message(STATUS "ZLIB_LIBRARIES:                 ${ZLIB_LIBRARIES}")
message(STATUS "CEF_LIBRARIES:                  ${CEF_LIBRARIES}")
message(STATUS "PROTOBUF_LIBRARIES:             ${PROTOBUF_LIBRARIES}")
message(STATUS "ADDITIONAL_LIBRARIES:           ${ADDITIONAL_LIBRARIES}")


# Generate protobuf c++ Headers/Sources from the proto file first TODO rename
add_dependencies(${CLIENT_BINARY} inexor_rpc_protobuf)

# CEF needs a C++ Wrapper Project first
# (Its core is C. Everything dependend on the stdlib are outsourced to this wrapper)

#add_dependencies(${CLIENT_BINARY} ${CEF_WRAPPER_LIBRARY})

if(MSVC)
    set_property(TARGET ${CLIENT_BINARY} PROPERTY FOLDER "executables")
endif()

# Install Binary
install(TARGETS ${CLIENT_BINARY}
        DESTINATION ${EXE_DIR})
