include_directories(${CMAKE_SOURCE_DIR}/src/)
include_directories(${CMAKE_SOURCE_DIR}/src/media)
include_directories(${CMAKE_SOURCE_DIR}/src/io)

AUX_SOURCE_DIRECTORY(${CMAKE_SOURCE_DIR}/src/media H264_WRAP)
AUX_SOURCE_DIRECTORY(${CMAKE_SOURCE_DIR}/src/io H264_IO)

add_definitions("-Wunused-variable")

add_executable(h264_wrap_example h264_wrap_example.cpp ${H264_WRAP} ${H264_IO})
add_executable(rtmp_server_example rtmp_server_example.cpp ${H264_WRAP} ${H264_IO})

target_link_libraries(h264_wrap_example log)
target_link_libraries(h264_wrap_example rtmp)
target_link_libraries(rtmp_server_example log)
target_link_libraries(rtmp_server_example rtmp)