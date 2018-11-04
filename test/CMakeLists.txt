
message("cmake_module_path: ${CMAKE_MODULE_PATH}")

find_package(googletest)

if(GOOGLETEST_FOUND)
        message("###googletest header:" ${googletest_INCLUDE_DIR})
        message("###googletest library:" ${googletest_LIBRARY})
        include_directories(${googletest_INCLUDE_DIR})
        AUX_SOURCE_DIRECTORY(. TEST_FILE)
        add_executable(unittest ${TEST_FILE})
        target_link_libraries(unittest ${googletest_LIBRARY} pthread)
        add_dependencies(unittest build_googletest)
else()
        message("not found GOOGLETEST_FOUND")
endif(GOOGLETEST_FOUND)

