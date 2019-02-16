include_directories(/home/cuijian/do/package/rtmpdump)
link_directories(/home/cuijian/do/package/rtmpdump/librtmp)
add_executable(send_flv send_flv.cpp)
target_link_libraries(send_flv rtmp)