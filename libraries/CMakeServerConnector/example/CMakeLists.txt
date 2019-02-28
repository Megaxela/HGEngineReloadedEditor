cmake_minimum_required(VERSION 3.5)
project(CMakeServerCLI)

# Getting sources
file(GLOB_RECURSE SOURCES src/*.cpp)

# Creating binary
add_executable(CMakeServerCLI
    ${SOURCES}
)

# Includes
target_include_directories(CMakeServerCLI
    PUBLIC
        include
)

target_link_libraries(CMakeServerCLI
    CMakeServerConnector
)

# Fancy C++ again
set_target_properties(CMakeServerCLI
    PROPERTIES
        CXX_STANDARD 17
)