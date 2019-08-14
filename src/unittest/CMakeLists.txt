include_directories(${CMAKE_SOURCE_DIR}/src/)
include_directories(${CMAKE_SOURCE_DIR}/src/media)
include_directories(${CMAKE_SOURCE_DIR}/src/io)
include_directories(${CMAKE_SOURCE_DIR}/src/redblack-tree)

AUX_SOURCE_DIRECTORY(${CMAKE_SOURCE_DIR}/src/media H264_WRAP)
AUX_SOURCE_DIRECTORY(${CMAKE_SOURCE_DIR}/src/io H264_IO)
AUX_SOURCE_DIRECTORY(${CMAKE_SOURCE_DIR}/src/redblack-tree REDBLACK_TREE)

add_definitions("-Wunused-variable")

add_executable(h264_wrap_example h264_wrap_example.cpp ${H264_WRAP} ${H264_IO})
add_executable(rtmp_server_example rtmp_server_example.cpp ${H264_WRAP} ${H264_IO})
add_executable(rbt_example rbt_example.cpp ${REDBLACK_TREE})

target_link_libraries(h264_wrap_example log)
target_link_libraries(h264_wrap_example rtmp)
target_link_libraries(rtmp_server_example log)
target_link_libraries(rtmp_server_example rtmp)
target_link_libraries(rbt_example log)