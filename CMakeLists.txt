cmake_minimum_required(VERSION 2.8.12.2)
cmake_policy(SET CMP0012 NEW) #whats that?

### Different Configuration Types (Debug release)

# Only generate Debug and Release configuration types.
set(CMAKE_CONFIGURATION_TYPES Debug Release)

# First choice is Release
set(CMAKE_BUILD_TYPE Release)

# CMAKE_BUILD_TYPE should be specified when using Unix Makefiles.
if(NOT CMAKE_BUILD_TYPE AND ${CMAKE_GENERATOR} STREQUAL "Unix Makefiles")
  
  message(WARNING "No CMAKE_BUILD_TYPE value selected, using ${CMAKE_BUILD_TYPE}")
endif()

# Project Name
project(Inexor)

# Warning if paths arent set well
get_filename_component(SRCDIR "${CMAKE_SOURCE_DIR}" REALPATH)
get_filename_component(BINDIR "${CMAKE_BINARY_DIR}" REALPATH) 

if(${SRCDIR} STREQUAL ${BINDIR})
    message(FATAL_ERROR "Keep your directories clean, don't generate the project-files in the main directory! ${SRCDIR} ${BINDIR}")
endif()

# Use folders in the resulting project files.
set_property(GLOBAL PROPERTY OS_FOLDERS ON)

### Platform Detection:

# Determine the project architecture.
if(NOT DEFINED PROJECT_ARCH)
  if(CMAKE_SIZEOF_VOID_P MATCHES 8)
    set(PROJECT_ARCH "x86_64")
    set(X64 1)
  else()
    set(PROJECT_ARCH "x86")
  endif()

  if(OS_MACOSX)
    # PROJECT_ARCH should be specified on Mac OS X.
    message(WARNING "No PROJECT_ARCH value specified, using ${PROJECT_ARCH}")
  endif()
endif()

# Determine the platform.
if("${CMAKE_SYSTEM_NAME}" STREQUAL "Darwin")
  set(OS_MACOSX 1)
  set(OS_POSIX 1)
  message(STATUS "Platform: OSX ${PROJECT_ARCH}")
elseif("${CMAKE_SYSTEM_NAME}" STREQUAL "Linux")
  set(OS_LINUX 1)
  set(OS_POSIX 1)
  message(STATUS "Platform: Linux ${PROJECT_ARCH}")
elseif("${CMAKE_SYSTEM_NAME}" STREQUAL "Windows")
  set(OS_WINDOWS 1)
  message(STATUS "Platform: Windows ${PROJECT_ARCH}")
endif()

# Directory paths
set(BINDIR_DEBUG    "${BINDIR}/Debug")
set(BINDIR_RELEASE  "${BINDIR}/Release")

# Allow C++ programs to use stdint.h macros specified in the C99 standard that
# aren't in the C++ standard (e.g. UINT8_MAX, INT64_MIN, etc).
add_definitions(-D__STDC_CONSTANT_MACROS -D__STDC_FORMAT_MACROS)


### Platform-specific compiler/linker flags.

#
# Linux configuration.
#
if(OS_LINUX)
    set(I_LIBTYPE                 SHARED)
    # -fomit-frame-pointer            = Don't keep the frame pointer for functions that don't need one
    # -fno-strict-aliasing            = Avoid assumptions regarding non-aliasing of objects of different types
    # -fPIC                           = Generate position-independent code for shared libraries
    # -fstack-protector               = Protect some vulnerable functions from stack-smashing (security feature)
    # -funwind-tables                 = Support stack unwinding for backtrace()
    # -fvisibility=hidden             = Give hidden visibility to declarations that are not explicitly marked as visible
    # --param=ssp-buffer-size=4       = Set the minimum buffer size protected by SSP (security feature, related to stack-protector)
    # -pipe                           = Use pipes rather than temporary files for communication between build stages
    # -pthread                        = Use the pthread library
    # -Wall                           = Enable all warnings
    set(I_COMPILER_FLAGS          "-fomit-frame-pointer -fno-strict-aliasing -fPIC -fstack-protector -funwind-tables -fvisibility=hidden --param=ssp-buffer-size=4 -pipe -pthread -Wall")
    # -std=c99                        = Use the C99 language standard
    set(I_C_COMPILER_FLAGS        "-std=c99")
    # -fno-exceptions                 = Disable exceptions
    # -fno-rtti                       = Disable real-time type information
    # -fno-threadsafe-statics         = Don't generate thread-safe statics
    # -fvisibility-inlines-hidden     = Give hidden visibility to inlined class member functions
    # -std=gnu++11                    = Use the C++11 language standard including GNU extensions
    set(I_CXX_COMPILER_FLAGS      "-fno-exceptions -fno-rtti -fno-threadsafe-statics -fvisibility-inlines-hidden -std=gnu++11")
    # -Og                             = Enable optimizations that do not interfere with debugging
    # -g                              = Generate debug information
    # -D_DEBUG                        = Debug build
    set(I_COMPILER_FLAGS_DEBUG    "-Og -g -D_DEBUG")
    # -O3                             = Optimize for maximum speed
    # -fdata-sections                 = Enable linker optimizations to improve locality of reference for data sections
    # -ffunction-sections             = Enable linker optimizations to improve locality of reference for function sections
    # -fno-ident                      = Ignore the #ident directive
    # -DNDEBUG                        = Not a debug build
    # -U_FORTIFY_SOURCE               = Undefine _FORTIFY_SOURCE in case it was previously defined
    # -D_FORTIFY_SOURCE=2             = Add memory and string function protection (security feature, related to stack-protector)
    set(I_COMPILER_FLAGS_RELEASE  "-O3 -fdata-sections -ffunction-sections -fno-ident -DNDEBUG -U_FORTIFY_SOURCE -D_FORTIFY_SOURCE=2")
    # -Wl,--disable-new-dtags         = Don't generate new-style dynamic tags in ELF
    # -Wl,-rpath,.                    = Set rpath so that libraries can be placed next to the executable
    # -Wl,-z,noexecstack              = Mark the stack as non-executable (security feature)
    # -Wl,-z,now                      = Resolve symbols on program start instead of on first use (security feature)
    # -Wl,-z,relro                    = Mark relocation sections as read-only (security feature)
    set(I_LINKER_FLAGS            "-fPIC -pthread -Wl,--disable-new-dtags -Wl,-rpath,. -Wl,-z,noexecstack -Wl,-z,now -Wl,-z,relro")
    # -Wl,-O1                         = Enable linker optimizations
    # -Wl,--as-needed                 = Only link libraries that export symbols used by the binary
    # -Wl,--gc-sections               = Remove unused code resulting from -fdata-sections and -function-sections
    set(I_LINKER_FLAGS_RELEASE    "-Wl,-O1 -Wl,--as-needed -Wl,--gc-sections")

    #include(CheckCCompilerFlag)
    #include(CheckCXXCompilerFlag)

    if(X64)
        # 64-bit architecture.
        set(I_COMPILER_FLAGS        "${I_COMPILER_FLAGS} -m64 -march=x86-64")
        set(I_LINKER_FLAGS          "${I_LINKER_FLAGS} -m64")
    else()
        # 32-bit architecture.
        set(I_COMPILER_FLAGS        "${I_COMPILER_FLAGS} -msse2 -mfpmath=sse -mmmx -m32")
        set(I_LINKER_FLAGS          "${I_LINKER_FLAGS} -m32")
    endif()

    # Allow the Large File Support (LFS) interface to replace the old interface.
    add_definitions(-D_FILE_OFFSET_BITS=64)
    
    # Define where to move the binary by install
    set(BIN_INSTALL_DIR_PARENT "bin/linux")
    if(X64)
        set(BIN_INSTALL_DIR "${BIN_INSTALL_DIR_PARENT}/x86_64")
    else()
        set(BIN_INSTALL_DIR "${BIN_INSTALL_DIR_PARENT}/x86")
    endif()
endif()


#
# Mac OS X configuration.
#

if(OS_MACOSX)
    # See also SET_XCODE_TARGET_PROPERTIES in macros.cmake.
    set(I_LIBTYPE                 SHARED)
    # -fomit-frame-pointer            = Don't keep the frame pointer for functions that don't need one
    # -fno-strict-aliasing            = Avoid assumptions regarding non-aliasing of objects of different types
    # -fstack-protector               = Protect some vulnerable functions from stack-smashing (security feature)
    # -funwind-tables                 = Support stack unwinding for backtrace()
    # -fvisibility=hidden             = Give hidden visibility to declarations that are not explicitly marked as visible
    # -Wall                           = Enable all warnings
    set(I_COMPILER_FLAGS          "-fomit-frame-pointer -fno-strict-aliasing -fstack-protector -funwind-tables -fvisibility=hidden -Wall")
    # -std=c99                        = Use the C99 language standard
    set(I_C_COMPILER_FLAGS        "-std=c99")
    # -fno-exceptions                 = Disable exceptions
    # -fno-rtti                       = Disable real-time type information
    # -fno-threadsafe-statics         = Don't generate thread-safe statics
    # -fobjc-call-cxx-cdtors          = Call the constructor/destructor of C++ instance variables in ObjC objects
    # -fvisibility-inlines-hidden     = Give hidden visibility to inlined class member functions
    # -std=gnu++11                    = Use the C++11 language standard including GNU extensions
    # -Wno-narrowing                  = Don't warn about type narrowing
    # -Wsign-compare                  = Warn about mixed signed/unsigned type comparisons
    set(I_CXX_COMPILER_FLAGS      "-fno-exceptions -fno-rtti -fno-threadsafe-statics -fobjc-call-cxx-cdtors -fvisibility-inlines-hidden -std=gnu++11 -Wno-narrowing -Wsign-compare")
    # -O0                             = Disable optimizations
    # -g                              = Generate debug information
    # -D_DEBUG                        = Debug build
    set(I_COMPILER_FLAGS_DEBUG    "-O0 -g -D_DEBUG")
    # -O3                             = Optimize for maximum speed plus a few extras
    set(I_COMPILER_FLAGS_RELEASE  "-O3")
    # -Wl,-search_paths_first         = Search for static or shared library versions in the same pass
    # -Wl,-ObjC                       = Support creation of creation of ObjC static libraries 
    # -Wl,-pie                        = Generate position-independent code suitable for executables only
    set(I_LINKER_FLAGS            "-Wl,-search_paths_first -Wl,-ObjC -Wl,-pie")
    # -Wl,-dead_strip                 = Strip dead code
    set(I_LINKER_FLAGS_RELEASE    "-Wl,-dead_strip")

    # Standard libraries.
    set(I_STANDARD_LIBS "-lpthread" "-framework Cocoa" "-framework AppKit")

    # Find the newest available base SDK.
    execute_process(COMMAND xcode-select --print-path OUTPUT_VARIABLE XCODE_PATH OUTPUT_STRIP_TRAILING_WHITESPACE)
    foreach(OS_VERSION 10.10 10.9 10.8 10.7)
        set(SDK "${XCODE_PATH}/Platforms/MacOSX.platform/Developer/SDKs/MacOSX${OS_VERSION}.sdk")
        if(NOT "${CMAKE_OSX_SYSROOT}" AND EXISTS "${SDK}" AND IS_DIRECTORY "${SDK}")
            set(CMAKE_OSX_SYSROOT ${SDK})
        endif()
    endforeach()

    # Target SDK.
    set(I_TARGET_SDK               "10.6")
    set(I_COMPILER_FLAGS           "${I_COMPILER_FLAGS} -mmacosx-version-min=${I_TARGET_SDK}")
    set(CMAKE_OSX_DEPLOYMENT_TARGET  ${I_TARGET_SDK})

    # Target architecture.
    if(X64)
        set(CMAKE_OSX_ARCHITECTURES "x86_64")
    else()
        set(CMAKE_OSX_ARCHITECTURES "i386")
    endif()
    
    # Define where to move the binary by install
    set(BIN_INSTALL_DIR "bin/mac/${CMAKE_OSX_ARCHITECTURES}")
    
    # Submodule providing osx-specific build stuff (e.g. libs)
    set(PLATFORM_FILE "${CMAKE_SOURCE_DIR}/src/platform_mac")
endif()


#
# Windows configuration.
#

if(OS_WINDOWS)
    set(I_LIBTYPE                 STATIC)
    # /MP                                   = Multiprocess compilation
    # /Gy                                   = Enable function-level linking
    # /GR-                                  = Disable run-time type information
    # /Zi                                   = Enable program database
    # /W3                                   = Warning level 3
    set(I_COMPILER_FLAGS          "/MP /Gy /GR- /Zi /W3")
    # /MTd                                  = Multithreaded debug runtime
    # /Od                                   = Disable optimizations
    # /RTC1                                 = Enable basic run-time checks
    set(I_COMPILER_FLAGS_DEBUG    "/MTd /RTC1 /Od")
    # /MT                                   = Multithreaded release runtime
    # /O2                                   = Optimize for maximum speed
    # /Ob2                                  = Inline any suitable function
    # /GF                                   = Enable string pooling
    # /D NDEBUG /D _NDEBUG                  = Not a debug build
    set(I_COMPILER_FLAGS_RELEASE  "/MT /O2 /Ob2 /GF /D NDEBUG /D _NDEBUG")
    # /DEBUG                                = Generate debug information
    set(I_LINKER_FLAGS_DEBUG      "/DEBUG")
    # /MANIFEST:NO                          = No default manifest
    set(I_EXE_LINKER_FLAGS        "/MANIFEST:NO")

    # Standard definitions
    # -DWIN32 -D_WIN32 -D_WINDOWS           = Windows platform
    # -DUNICODE -D_UNICODE                  = Unicode build
    # -DWINVER=0x0602 -D_WIN32_WINNT=0x602  = Targeting Windows 8
    # -DNOMINMAX                            = Use the standard's templated min/max
    # -DWIN32_LEAN_AND_MEAN                 = Exclude less common API declarations
    # -D_HAS_EXCEPTIONS=0                   = Disable exceptions
    add_definitions(-DWIN32 -D_WIN32 -D_WINDOWS -DUNICODE -D_UNICODE -DWINVER=0x0602
                  -D_WIN32_WINNT=0x602 -DNOMINMAX -DWIN32_LEAN_AND_MEAN -D_HAS_EXCEPTIONS=0)
    if(X64)
        add_definitions(-DWIN64)
    endif()

    # Configure use of the sandbox.
    option(USE_SANDBOX "Enable or disable use of the sandbox." ON)
    if(USE_SANDBOX AND NOT MSVC_VERSION EQUAL 1800)
        # The I_sandbox.lib static library is currently built with VS2013. It will
        # not link successfully with other VS versions.
        set(USE_SANDBOX OFF)
    endif()

    if(USE_SANDBOX)
        # Definition required by I_sandbox.lib.
        add_definitions(-DPSAPI_VERSION=1)
        # Definition used by apps to test if the sandbox is enabled.
        add_definitions(-DI_USE_SANDBOX)
    endif()

    # Configure use of ATL (Active Template Library)
    option(USE_ATL "Enable or disable use of ATL." ON)
    if(USE_ATL)
        # Determine if the Visual Studio install supports ATL.
        get_filename_component(VC_BIN_DIR ${CMAKE_CXX_COMPILER} DIRECTORY)
        get_filename_component(VC_DIR ${VC_BIN_DIR} DIRECTORY)
        if(NOT IS_DIRECTORY "${VC_DIR}/atlmfc")
          set(USE_ATL OFF)
        endif()
    endif()

    if(USE_ATL)
        # Definition used by apps to test if ATL support is enabled.
        add_definitions(-DI_USE_ATL)
    endif()
    
    # Define where to move the binary by install
    set(BIN_INSTALL_DIR_PARENT "bin/windows")
    if(X64)
        set(BIN_INSTALL_DIR "${BIN_INSTALL_DIR_PARENT}/64")
    else()
        set(BIN_INSTALL_DIR "${BIN_INSTALL_DIR_PARENT}/32")
    endif()
    
    # Submodule providing windows-specific build stuff (e.g. libs)
    set(PLATFORM_FILE "${CMAKE_SOURCE_DIR}/src/platform_windows")
endif()

# Merge compiler/linker flags.
set(CMAKE_C_FLAGS                     "${I_COMPILER_FLAGS} ${I_C_COMPILER_FLAGS}")
set(CMAKE_CXX_FLAGS                   "${I_COMPILER_FLAGS} ${I_CXX_COMPILER_FLAGS}")
set(CMAKE_CXX_FLAGS_DEBUG             "${I_COMPILER_FLAGS_DEBUG} ${I_CXX_COMPILER_FLAGS_DEBUG}")
set(CMAKE_CXX_FLAGS_RELEASE           "${I_COMPILER_FLAGS_RELEASE} ${I_CXX_COMPILER_FLAGS_RELEASE}")
set(CMAKE_EXE_LINKER_FLAGS            "${I_LINKER_FLAGS} ${I_EXE_LINKER_FLAGS}")
set(CMAKE_EXE_LINKER_FLAGS_DEBUG      "${I_LINKER_FLAGS_DEBUG} ${I_EXE_LINKER_FLAGS_DEBUG}")
set(CMAKE_EXE_LINKER_FLAGS_RELEASE    "${I_LINKER_FLAGS_RELEASE} ${I_EXE_LINKER_FLAGS_RELEASE}")

message(STATUS "Current mode:                   ${CMAKE_BUILD_TYPE}")
message(STATUS "Compiler:                       ${CMAKE_CXX_COMPILER_ID}")

message(STATUS "Compiler definitions:           ${DEFINITIONS}")

message(STATUS "C_FLAGS:                        ${CMAKE_C_FLAGS}")
message(STATUS "CXX_FLAGS:                      ${CMAKE_CXX_FLAGS}")
message(STATUS "CXX_FLAGS_DEBUG:                ${CMAKE_CXX_FLAGS_DEBUG}")
message(STATUS "CXX_FLAGS_RELEASE:              ${CMAKE_CXX_FLAGS_RELEASE}")
message(STATUS "EXE_LINKER_FLAGS:               ${CMAKE_EXE_LINKER_FLAGS}")
message(STATUS "EXE_LINKER_FLAGS_DEBUG:         ${CMAKE_EXE_LINKER_FLAGS_DEBUG}")
message(STATUS "EXE_LINKER_FLAGS_RELEASE:       ${CMAKE_EXE_LINKER_FLAGS_RELEASE}")

# Temp file directories
set(BUILD_TEMP_DIR         ${CMAKE_BINARY_DIR}/tmp)
set(BUILD_TEMP_INCLUDE_DIR ${BUILD_TEMP_DIR}/include)
file(MAKE_DIRECTORY        ${BUILD_TEMP_DIR})
file(MAKE_DIRECTORY        ${BUILD_TEMP_INCLUDE_DIR})

# Install paths

if(NOT BUILD_INSTALL_DIR)
    if(${RELEASE_BUILD})
        set(BUILD_INSTALL_DIR ${PROJECT_SOURCE_DIR})
    else()
        set(BUILD_INSTALL_DIR ${PROJECT_SOURCE_DIR})
    endif()
endif()

if(NOT BUILD_INSTALL_BIN_DIR)
    set(BUILD_INSTALL_BIN_DIR "${BUILD_INSTALL_DIR}/${BIN_INSTALL_DIR}")

    set(INSTALL_HEADER_DIR      ${BUILD_TEMP_INCLUDE_DIR})

    set(INSTALL_RESOURCES_DIR   ${BUILD_INSTALL_DIR}/resources  CACHE INTERNAL "")

    message(STATUS "Installing to ${BIN_INSTALL_DIR} (bin: ${CMAKE_CURRENT_BINARY_DIR}, resources: ${INSTALL_RESOURCES_DIR}, header: ${INSTALL_HEADER_DIR})")
endif()

if(EXISTS ${PLATFORM_FILE})
    message(STATUS "Found platform definition. (${PLATFORM_FILE})")
    add_subdirectory(${PLATFORM_FILE})
    link_directories(${PLATFORM_LINK_DIRECTORIES})
    include_directories(${PLATFORM_INCLUDE_DIRECTORIES})
else()
    message(WARNING "Could not find platform definition. (${PLATFORM_FILE})")
endif()

add_subdirectory(src)
