cmake_minimum_required(VERSION 3.12)
project(HGEngineReloadedImGui)

# Setting cmake module path
set(CMAKE_MODULE_PATH ${CMAKE_CURRENT_LIST_DIR}/cmake/)

set(CMAKE_CXX_STANDARD 17)

set(HGRenderingOGL_USE_GLFW On)

# Adding engine
add_subdirectory(engine)

# Adding libraries
add_subdirectory(libraries)

# Internal libraries

# Find LLVM
find_package(LLVM REQUIRED CONFIG)
message(STATUS "Found LLVM " ${LLVM_PACKAGE_VERSION})

# Find clang
find_package(Clang REQUIRED)

# Finding components
llvm_map_components_to_libnames(LLVM_COMPONENTS_MODULES core)

# Adding sources
file(GLOB_RECURSE SOURCE_FILES src/*.cpp)
file(GLOB_RECURSE HEADER_FILES include/*.cpp)

# Setting up application
add_executable(HGEngineReloadedImGui ${SOURCE_FILES})

target_include_directories(HGEngineReloadedImGui PUBLIC
    include
    ${CLANG_INCLUDE_DIRS}
)

target_link_libraries(HGEngineReloadedImGui
    # HGEngine
    HGCore
    HGRenderingBase
    HGRenderingOGL
    HGStandard
    HGUtils

    # Libraries
    FilesystemWatcher
    rectpack2D

    # LLVM
    LLVM
    ${CLANG_LIBS}
)

configure_file(resources/images/folder.png images/folder.png COPYONLY )
configure_file(resources/images/text.png images/text.png COPYONLY )
configure_file(resources/images/error.png images/error.png COPYONLY )