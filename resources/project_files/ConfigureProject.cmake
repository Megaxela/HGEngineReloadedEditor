cmake_minimum_required(VERSION 3.5)

function(prepare_project)
    cmake_parse_arguments(
            ARGS
            ""
            "PROJECT_NAME;ASSETS_DIRECTORY;PATH_TO_ENGINE"
            "DEPENDENCIES"
            ${ARGN}
    )

    # Creating project
    project(${ARGS_PROJECT_NAME})

    # Searching for sources
    file(GLOB_RECURSE SOURCES
        ${ARGS_ASSETS_DIRECTORY}/*.cpp
        ${ARGS_ASSETS_DIRECTORY}/*.cpp
    )

    # Adding library for each source file
    foreach(SOURCE ${SOURCES})

        # Converting path to identifier
        string(MAKE_C_IDENTIFIER ${SOURCE} SOURCE_IDENTITY)

        # Adding library for source file
        add_library(${SOURCE_IDENTITY} SHARED ${SOURCE})

        # Including directories
        target_include_directories(${SOURCE_IDENTITY}
            PUBLIC
                ${ARGS_ASSETS_DIRECTORY}
                ${ARGS_PATH_TO_ENGINE}/include
        )

        # Linkage
        target_link_libraries(${SOURCE_IDENTITY}
            ${ARGS_DEPENDENCIES}
        )

        # Enabling C++ 17 support
        set_target_properties(${SOURCE_IDENTITY}
            PROPERTIES CXX_STANDARD 17
        )

    endforeach()

endfunction()