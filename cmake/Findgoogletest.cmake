set(GOOGLETEST_SOURCE_PATH ${CMAKE_SOURCE_DIR}/package/googletest-release-1.8.1/googletest)

FIND_PATH(googletest_INCLUDE_DIR gtest/gtest.h ${GOOGLETEST_SOURCE_PATH}/include)
FIND_LIBRARY(googletest_LIBRARY libgtest.a ${GOOGLETEST_SOURCE_PATH}/build/)

if(googletest_INCLUDE_DIR AND googletest_LIBRARY)
        set(GOOGLETEST_FOUND TRUE)
endif()

