cmake_minimum_required (VERSION 2.8)

message(${CMAKE_SOURCE_DIR})

set(CMAKE_MODULE_PATH /usr/share/cmake-3.10/Modules)
message("CMAKE_MODULE_PATH:${CMAKE_MODULE_PATH}")

find_package(BZip2)

if (BZIP2_FOUND)
message(${BZIP2_INCLUDE_DIR})
message(${BZIP2_LIBRARIES})
message(${BZIP2_NEED_PREFIX})
message(${BZIP2_VERSION_STRING})
endif(BZIP2_FOUND)

set(SRC_LIB demo.cpp)
add_library(demolib STATIC ${SRC_LIB})

set(DESTINATION ${CMAKE_SOURCE_DIR})

install(TARGETS demolib DESTINATION ${CMAKE_BINARY_DIR})
install(FILES demo.h DESTINATION ${CMAKE_BINARY_DIR})

set(CMAKE_MODULE_PATH ${PROJECT_SOURCE_DIR}/cmake)
message("CMAKE_MODULE_PATH:${CMAKE_MODULE_PATH}")
find_package(DEMOLIB)

if(DEMOLIB_FOUND)
    add_executable(demo_main demo_main.cpp)
    message("found demo9 ${DEMOLIB_INCLUDE_DIR} ${DEMOLIB_LIBRARY}")
    include_directories(${DEMOLIB_INCLUDE_DIR})
    target_link_libraries(demo_main ${DEMOLIB_LIBRARY})
else()
    message("not found DEMOLIB")
endif(DEMOLIB_FOUND)

