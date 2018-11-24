add_subdirectory(FilesystemWatcher)

file(GLOB RECTPACK_SOURCE rectpack2D/src/*.h)
add_library(rectpack2D ${RECTPACK_SOURCE})
target_include_directories(rectpack2D PUBLIC rectpack2D/src)
set_target_properties(rectpack2D PROPERTIES LINKER_LANGUAGE CXX)