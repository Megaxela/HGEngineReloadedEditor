
# This function prepares json for
# install script.
function(prepare_install_script)

    cmake_parse_arguments(
        ARGS
        ""
        ""
        "MODULES"
        ${ARGN}
    )

    set(INSTALLATION_JSON_CONTENT "{")

        # Adding engine subdirectory
        string(APPEND INSTALLATION_JSON_CONTENT "\"engine\": {")

            # Adding engine's includes
            string(APPEND INSTALLATION_JSON_CONTENT "\"include_directories\": [")
            set(FIRST_RUN On)

            foreach(MODULE ${ARGS_MODULES})

                if (NOT ${FIRST_RUN})
                    string(APPEND INSTALLATION_JSON_CONTENT ", ")
                endif()

                set(FIRST_RUN Off)

                get_target_property(
                    MODULE_INCLUDE_DIRS
                    ${MODULE}
                    INCLUDE_DIRECTORIES
                )
                # Removing non-engine directories
                set(FILTERED_DIRECTORIES "")
                foreach(DIR ${MODULE_INCLUDE_DIRS})
                    string(FIND "${DIR}" "engine/src" FIND_INDEX)

                    if (NOT ${FIND_INDEX} EQUAL -1)
                        string(APPEND INSTALLATION_JSON_CONTENT "\"${DIR}\"")
                    endif()
                endforeach()

            endforeach()
            string(APPEND INSTALLATION_JSON_CONTENT "],")

            # Adding engine's libraries
            string(APPEND INSTALLATION_JSON_CONTENT "\"library_paths\": [")
            set(FIRST_RUN On)

            foreach (MODULE ${ARGS_MODULES})

                if (NOT ${FIRST_RUN})
                    string(APPEND INSTALLATION_JSON_CONTENT ", ")
                endif()

                set(FIRST_RUN Off)

                get_target_property(
                    ${MODULE}_LOCATION
                    ${MODULE}
                    BINARY_DIR
                )

                string(APPEND INSTALLATION_JSON_CONTENT "\"${${MODULE}_LOCATION}\"")

            endforeach()
            string(APPEND INSTALLATION_JSON_CONTENT "]")

        string(APPEND INSTALLATION_JSON_CONTENT "}")

    string(APPEND INSTALLATION_JSON_CONTENT "}")

    file(WRITE "${CMAKE_CURRENT_BINARY_DIR}/install_dependencies.json" ${INSTALLATION_JSON_CONTENT})
    message(STATUS "Installation \"install_dependencies.json\" file written.")

endfunction()