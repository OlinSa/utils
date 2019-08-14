include_directories(${CMAKE_SOURCE_DIR}/common)
add_executable(ssl_server ssl_server.cpp)
target_link_libraries(ssl_server ssl crypto)


include_directories(${CMAKE_SOURCE_DIR}/common)
add_executable(ssl_client ssl_client.cpp)
target_link_libraries(ssl_client ssl crypto)