
ADD_DEFINITIONS("-Wall -g -DDEBUG")

include_directories(${CMAKE_SOURCE_DIR}/common)
include_directories(${CMAKE_SOURCE_DIR}/src/system-monitor)
add_executable(proc_test "proc_test.cpp")
target_link_libraries(proc_test utils log)

add_subdirectory(thread_pool)
add_subdirectory(unittest)
add_subdirectory(live555)
add_subdirectory(camera)
add_subdirectory(librtmp)
add_subdirectory(ffmpeg)
add_subdirectory(sqlite3)
add_subdirectory(network)