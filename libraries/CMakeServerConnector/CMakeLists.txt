cmake_minimum_required(VERSION 3.5)
project(CMakeServerConnector)

# Setting up build
option(CONNECTOR_BUILD_EXAMPLE "Build CLI example connector" On)

# Adding example if required
if (${CONNECTOR_BUILD_EXAMPLE})
    add_subdirectory(example)
endif()

# Adding 3rdparty
#add_subdirectory(libraries)

# Getting sources
file(GLOB_RECURSE SOURCES src/*.cpp)
file(GLOB_RECURSE INCLUDES include/*.hpp)

# Creating library
add_library(CMakeServerConnector STATIC
    ${SOURCES}
    ${INCLUDES}
)

# Includes
target_include_directories(CMakeServerConnector
    PUBLIC
        include
)

# Linkage
target_link_libraries(CMakeServerConnector
    nlohmann_json
)

# Lets support some fancy C++
set_target_properties(CMakeServerConnector
    PROPERTIES
        CXX_STANDARD 17
)
