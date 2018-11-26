
message("cmake_module_path: ${CMAKE_MODULE_PATH}")

find_package(googletest)

if(GOOGLETEST_FOUND)
        message("googletest header:" ${googletest_INCLUDE_DIR})
        message("googletest library:" ${googletest_LIBRARY})
        include_directories(${googletest_INCLUDE_DIR})
        AUX_SOURCE_DIRECTORY(unittest/ TEST_FILE)
        add_executable(unittest ${TEST_FILE})
        target_link_libraries(unittest ${googletest_LIBRARY} pthread)
        add_dependencies(unittest build_googletest)
else()
        message("not found GOOGLETEST_FOUND")
endif(GOOGLETEST_FOUND)

include_directories(${CMAKE_SOURCE_DIR}/common)
include_directories(${CMAKE_SOURCE_DIR}/src/system-monitor)
add_executable(proc_test "proc_test.cpp")
target_link_libraries(proc_test utils log)

add_subdirectory(thread_pool)





