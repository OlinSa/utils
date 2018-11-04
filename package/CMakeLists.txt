set(GOOGLETEST_PACKAGE_DIR ${CMAKE_CURRENT_SOURCE_DIR})
add_custom_target(build_googletest ALL
        COMMAND echo "build googletest"
        COMMAND sh -x ${GOOGLETEST_PACKAGE_DIR}"/googletest.mk"  ${GOOGLETEST_PACKAGE_DIR}
        WORKING_DIRECTORY .)
