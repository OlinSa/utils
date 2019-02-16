AUX_SOURCE_DIRECTORY(. TEST_FILE)
add_executable(unittest ${TEST_FILE})
target_link_libraries(unittest gtest pthread)