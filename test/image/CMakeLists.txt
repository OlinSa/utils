include_directories(${CMAKE_SOURCE_DIR}/src)
AUX_SOURCE_DIRECTORY(. IMAGE_TEST_SRC)
add_executable(image_test ${IMAGE_TEST_SRC})
target_link_libraries(image_test utils log)