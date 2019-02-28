include_directories(${CMAKE_SOURCE_DIR}/src/)

AUX_SOURCE_DIRECTORY(${CMAKE_SOURCE_DIR}/src/media H264_WRAP)

add_definitions("-Wunused-variable")

add_executable(h264_wrap h264_wrap.cpp ${H264_WRAP})

target_link_libraries(h264_wrap log)