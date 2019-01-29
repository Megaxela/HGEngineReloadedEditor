cmake_minimum_required(VERSION 3.12)
project(HGEngineReloadedEditor)

# Setting cmake module path
set(CMAKE_MODULE_PATH ${CMAKE_CURRENT_LIST_DIR}/cmake/)
include(cmake/InstallSystem.cmake)

set(CMAKE_CXX_STANDARD 17)

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
file(GLOB_RECURSE HEADER_FILES include/*.hpp)

# Setting up application
add_executable(HGEngineReloadedEditor ${SOURCE_FILES})

target_include_directories(HGEngineReloadedEditor PUBLIC
    include
    ${CLANG_INCLUDE_DIRS}
)

target_link_libraries(HGEngineReloadedEditor
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

add_sanitizers(HGEngineReloadedEditor)

# Copying resources
file(GLOB_RECURSE RESOURCES resources/*.*)

foreach(RESOURCE ${RESOURCES})
    string(REPLACE "${CMAKE_CURRENT_LIST_DIR}/resources/" "" RESOURCE_RELATIVE_PATH ${RESOURCE} )
    configure_file("resources/${RESOURCE_RELATIVE_PATH}" ${RESOURCE_RELATIVE_PATH} COPYONLY)

    message(STATUS "Copying editor resource \"${RESOURCE_RELATIVE_PATH}\"")
endforeach()

# Preparing install
prepare_install_script(
    MODULES ${HG_MODULES}
)