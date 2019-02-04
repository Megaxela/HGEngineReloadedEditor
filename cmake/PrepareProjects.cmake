function(prepare_libexecstream_project PATH)

    add_library(libexecstream STATIC
        ${PATH}/exec-stream.cpp
    )

    target_include_directories(libexecstream
        PUBLIC ${PATH}
    )

endfunction()

function(prepare_rectpack_project PATH)
    file(GLOB RECTPACK_SOURCE "${PATH}/src/*.h")

    add_library(rectpack2D STATIC ${RECTPACK_SOURCE})

    target_include_directories(rectpack2D PUBLIC ${PATH}/src)

    set_target_properties(rectpack2D PROPERTIES LINKER_LANGUAGE CXX)
endfunction()