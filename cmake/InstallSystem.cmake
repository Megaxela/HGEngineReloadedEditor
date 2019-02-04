
function(_append_json_binary_info)

    cmake_parse_arguments(
        ARGS
        ""
        "RESULT_VARIABLE;KEY;FILES_ROOT"
        "TARGETS;FILES"
        ${ARGN}
    )

    # Adding targets subdirectory
    string(APPEND ${ARGS_RESULT_VARIABLE} "\"${ARGS_KEY}\": {")

        # Adding target's binaries
        string(APPEND ${ARGS_RESULT_VARIABLE} "\"binaries\": [")

        set(FIRST_RUN On)
        foreach(TARGET_NAME ${ARGS_TARGETS})

            if (NOT ${FIRST_RUN})
                string(APPEND ${ARGS_RESULT_VARIABLE} ", ")
            endif()

            set(FIRST_RUN Off)

            string(APPEND ${ARGS_RESULT_VARIABLE} "\"$<TARGET_FILE:${TARGET_NAME}>\"")

        endforeach()

        string(APPEND ${ARGS_RESULT_VARIABLE} "],")

        string(APPEND ${ARGS_RESULT_VARIABLE} "\"files\": [")

        set(FIRST_RUN On)
        foreach(TARGET_NAME ${ARGS_FILES})

            if (NOT ${FIRST_RUN})
                string(APPEND ${ARGS_RESULT_VARIABLE} ", ")
            endif()

            set(FIRST_RUN Off)

            set(INTERNAL_FIRST_RUN On)
            foreach(FILE ${ARGS_FILES})

                if (NOT ${INTERNAL_FIRST_RUN})
                    string(APPEND ${ARGS_RESULT_VARIABLE} ", ")
                endif()

                string(REPLACE ${ARGS_FILES_ROOT} "" FILE_RELATIVE ${FILE})

                set(INTERNAL_FIRST_RUN Off)

                string(APPEND ${ARGS_RESULT_VARIABLE} "{")

                    string(APPEND ${ARGS_RESULT_VARIABLE} "\"source\": \"${FILE}\",")
                    string(APPEND ${ARGS_RESULT_VARIABLE} "\"target\": \"${FILE_RELATIVE}\"")

                string(APPEND ${ARGS_RESULT_VARIABLE} "}")
            endforeach()

        endforeach()

        string(APPEND ${ARGS_RESULT_VARIABLE} "]")

    string(APPEND ${ARGS_RESULT_VARIABLE} "}")
    set(${ARGS_RESULT_VARIABLE} ${${ARGS_RESULT_VARIABLE}} PARENT_SCOPE)
endfunction()

# This function performs appending
# json value for targets
function(_append_json_targets_info)

    cmake_parse_arguments(
        ARGS
        ""
        "RESULT_VARIABLE;KEY"
        "TARGETS"
        ${ARGN}
    )

    # Adding targets subdirectory
    string(APPEND ${ARGS_RESULT_VARIABLE} "\"${ARGS_KEY}\": {")

        # Adding target's includes
        string(APPEND ${ARGS_RESULT_VARIABLE} "\"include_directories\": [")
        set(FIRST_RUN On)

        foreach(TARGET_NAME ${ARGS_TARGETS})

            # Getting targets type
            get_target_property(${TARGET_NAME}_TYPE ${TARGET_NAME} TYPE)

            if (${TARGET_NAME}_TYPE STREQUAL "INTERFACE_LIBRARY")
                get_target_property(
                    ${TARGET_NAME}_INCLUDE_DIRS
                    ${TARGET_NAME}
                    INTERFACE_INCLUDE_DIRECTORIES
                )

            else()

                get_target_property(
                    ${TARGET_NAME}_INCLUDE_DIRS
                    ${TARGET_NAME}
                    INCLUDE_DIRECTORIES
                )
            endif()

            # Removing non-engine directories
            foreach(DIR ${${TARGET_NAME}_INCLUDE_DIRS})

                # Searching engine specific path
                string(FIND "${DIR}" "engine/src" FIND_INDEX)

                # If it's engine path - add it to json
                if (NOT ${FIND_INDEX} EQUAL -1)

                    if (NOT ${FIRST_RUN})
                        string(APPEND ${ARGS_RESULT_VARIABLE} ", ")
                    endif()

                    set(FIRST_RUN Off)

                    string(APPEND ${ARGS_RESULT_VARIABLE} "\"${DIR}\"")
                endif()
            endforeach()

        endforeach()
        string(APPEND ${ARGS_RESULT_VARIABLE} "],")

        # Adding target's libraries
        string(APPEND ${ARGS_RESULT_VARIABLE} "\"library_paths\": [")
        set(FIRST_RUN On)

        foreach (TARGET_NAME ${ARGS_TARGETS})

            if (NOT ${TARGET_NAME}_TYPE STREQUAL "INTERFACE_LIBRARY")
                if (NOT ${FIRST_RUN})
                    string(APPEND ${ARGS_RESULT_VARIABLE} ", ")
                endif()

                set(FIRST_RUN Off)

                string(APPEND ${ARGS_RESULT_VARIABLE} "\"$<TARGET_FILE:${TARGET_NAME}>\"")
            endif()

        endforeach()
        string(APPEND ${ARGS_RESULT_VARIABLE} "]")

    string(APPEND ${ARGS_RESULT_VARIABLE} "}")
    set(${ARGS_RESULT_VARIABLE} ${${ARGS_RESULT_VARIABLE}} PARENT_SCOPE)

endfunction()


# This function prepares json for
# install script.
function(prepare_install_script)

    cmake_parse_arguments(
        ARGS
        ""
        "MAIN_TARGET;RESOURCES_ROOT"
        "MODULES;DEPENDENCIES;RESOURCES"
        ${ARGN}
    )

    set(INSTALLATION_JSON_CONTENT "{")

        _append_json_binary_info(
            RESULT_VARIABLE INSTALLATION_JSON_CONTENT
            KEY "__root__"
            TARGETS ${ARGS_MAIN_TARGET}
            FILES ${ARGS_RESOURCES}
            FILES_ROOT ${ARGS_RESOURCES_ROOT}
        )

        string(APPEND INSTALLATION_JSON_CONTENT ", ")

        _append_json_targets_info(
            RESULT_VARIABLE INSTALLATION_JSON_CONTENT
            KEY "engine"
            TARGETS ${ARGS_MODULES}
        )

        string(APPEND INSTALLATION_JSON_CONTENT ", ")

        _append_json_targets_info(
            RESULT_VARIABLE INSTALLATION_JSON_CONTENT
            KEY "3rdparty"
            TARGETS ${ARGS_DEPENDENCIES}
        )

    string(APPEND INSTALLATION_JSON_CONTENT "}")

    file(GENERATE
        OUTPUT "${CMAKE_CURRENT_BINARY_DIR}/install_dependencies.json"
        CONTENT ${INSTALLATION_JSON_CONTENT}
    )
    message(STATUS "Installation \"install_dependencies.json\" file written.")

endfunction()

function(prepare_engine_meta)

    cmake_parse_arguments(
        ARGS
        ""
        ""
        "MODULES"
        ${ARGN}
    )

    set(META_JSON_CONTENT "{")

        string(APPEND META_JSON_CONTENT "\"modules\": [")

        set(FIRST_RUN On)

        foreach(MODULE ${ARGS_MODULES})

            if (NOT ${FIRST_RUN})
                string(APPEND META_JSON_CONTENT ",")
            endif()

            set(FIRST_RUN Off)

            string(APPEND META_JSON_CONTENT "{")

                string(APPEND META_JSON_CONTENT "\"name\": \"${MODULE}\",")
                string(APPEND META_JSON_CONTENT "\"library\": \"$<TARGET_FILE_NAME:${MODULE}>\",")
                string(APPEND META_JSON_CONTENT "\"dependencies\": [")

                get_target_property(${MODULE}_DEPENDENCIES ${MODULE} LINK_LIBRARIES)

                set(INTERNAL_FIRST_RUN On)
                foreach(LIB ${${MODULE}_DEPENDENCIES})
                    if (NOT ${INTERNAL_FIRST_RUN})
                        string(APPEND META_JSON_CONTENT ",")
                    endif()

                    set(INTERNAL_FIRST_RUN Off)

                    string(APPEND META_JSON_CONTENT "\"${LIB}\"")
                endforeach()

                string(APPEND META_JSON_CONTENT "]")

            string(APPEND META_JSON_CONTENT "}")

        endforeach()

        string(APPEND META_JSON_CONTENT "]")

    string(APPEND META_JSON_CONTENT "}")

    file(GENERATE
        OUTPUT "${CMAKE_CURRENT_BINARY_DIR}/engine_meta.json"
        CONTENT ${META_JSON_CONTENT}
    )

    message(STATUS "Engine meta info \"engine_meta.json\" file written.")
endfunction()

function(prepare_installation)

    cmake_parse_arguments(
        ARGS
        ""
        "MAIN_TARGET;RESOURCES_ROOT"
        "MODULES;DEPENDENCIES;RESOURCES"
        ${ARGN}
    )

    prepare_engine_meta(
            MODULES ${ARGS_MODULES}
    )

    prepare_install_script(
        MAIN_TARGET    ${ARGS_MAIN_TARGET}
        RESOURCES_ROOT ${ARGS_RESOURCES_ROOT}
        RESOURCES      ${ARGS_RESOURCES}
        MODULES        ${ARGS_MODULES}
        DEPENDENCIES   ${ARGS_DEPENDENCIES}
    )

endfunction()