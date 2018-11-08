cmake_minimum_required(VERSION 3.12)
project(HGEngineReloadedImGui)

set(CMAKE_CXX_STANDARD 17)

set(HGRenderingOGL_USE_GLFW On)

# Adding engine
add_subdirectory(engine)

# Adding libraries
add_subdirectory(libraries)

# Adding sources
file(GLOB_RECURSE SOURCE_FILES src/*.cpp)
file(GLOB_RECURSE HEADER_FILES include/*.cpp)

# Setting up application
add_executable(HGEngineReloadedImGui ${SOURCE_FILES})

target_include_directories(HGEngineReloadedImGui PUBLIC
    include
)

target_link_libraries(HGEngineReloadedImGui
    HGCore
    HGRenderingBase
    HGRenderingOGL
    HGStandard
    HGUtils

    FilesystemWatcher
)

configure_file(resources/images/folder.png images/folder.png COPYONLY )
configure_file(resources/images/text.png images/text.png COPYONLY )