
ADD_DEFINITIONS("-Wall -g -DDEBUG")

AUX_SOURCE_DIRECTORY(unittest/ TEST_FILE)
add_executable(unittest ${TEST_FILE})
target_link_libraries(unittest gtest pthread)

include_directories(${CMAKE_SOURCE_DIR}/common)
include_directories(${CMAKE_SOURCE_DIR}/src/system-monitor)
add_executable(proc_test "proc_test.cpp")
target_link_libraries(proc_test utils log)

add_subdirectory(thread_pool)

include_directories(${CMAKE_SOURCE_DIR}/src)
AUX_SOURCE_DIRECTORY(image/ IMAGE_TEST_SRC)
add_executable(image_test ${IMAGE_TEST_SRC})
target_link_libraries(image_test utils log)

aux_source_directory(live555 LIVE_TEST_SRC)
add_executable(live ${LIVE_TEST_SRC})
include_directories(/usr/local/include/BasicUsageEnvironment)
include_directories(/usr/local/include/groupsock)
include_directories(/usr/local/include/liveMedia)
include_directories(/usr/local/include/UsageEnvironment)
target_link_libraries(live liveMedia BasicUsageEnvironment groupsock UsageEnvironment)




