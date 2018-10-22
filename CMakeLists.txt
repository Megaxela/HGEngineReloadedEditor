cmake_minimum_required(VERSION 3.12)
project(HGEngineReloadedImGui)

set(CMAKE_CXX_STANDARD 17)

set(HGRenderingOGL_USE_GLFW On)

# Adding engine
add_subdirectory(engine)

# Adding libraries
add_subdirectory(libraries)

# Setting up application
add_executable(HGEngineReloadedImGui
    src/main.cpp
    src/Scenes/EditorScene.cpp
    include/Scenes/EditorScene.hpp
    src/EditBehaviours/GraphicsInterface.cpp
    include/EditBehaviours/GraphicsInterface.hpp
)

target_include_directories(HGEngineReloadedImGui PUBLIC
    include
)

target_link_libraries(HGEngineReloadedImGui
    HGCore
    HGRenderingBase
    HGRenderingOGL
    HGStandard
    HGUtils
)