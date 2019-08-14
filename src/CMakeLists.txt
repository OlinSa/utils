include_directories(/home/cuijian/do/package/rtmpdump)
link_directories(/home/cuijian/do/package/rtmpdump/librtmp)
AUX_SOURCE_DIRECTORY(thread-pool DIR_SRC)
AUX_SOURCE_DIRECTORY(system-monitor DIR_SRC)
AUX_SOURCE_DIRECTORY(image DIR_SRC)
AUX_SOURCE_DIRECTORY(camera DIR_SRC)
AUX_SOURCE_DIRECTORY(media DIR_SRC)
AUX_SOURCE_DIRECTORY(io DIR_SRC)
AUX_SOURCE_DIRECTORY(redblack-tree DIR_SRC)

ADD_LIBRARY(utils ${DIR_SRC})

INCLUDE_DIRECTORIES(${CMAKE_SOURCE_DIR}/common)

add_subdirectory(unittest)