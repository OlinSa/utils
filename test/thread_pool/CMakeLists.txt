include_directories(${PROJECT_SOURCE_DIR}/src/thread-pool)
link_directories(${PROJECT_SOURCE_DIR}/src/thread-pool)

add_executable(threadpool main.cpp)
target_link_libraries(threadpool utils log pthread)