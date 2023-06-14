set(ROOT ${CMAKE_CURRENT_SOURCE_DIR})
set(PLATFORM "linux")

aux_source_directory(. SRC)

if(${PLATFORM} STREQUAL "pi")
aux_source_directory(platform/pi SRC)
elseif(${PLATFORM} STREQUAL "linux")
aux_source_directory(platform/linux SRC)
endif()


message(STATUS "ROOT=${ROOT}")
message(STATUS "SRC=${SRC}")


include_directories(${ROOT})

add_executable(round_lcd ${SRC})

if(${PLATFORM} STREQUAL "pi")
target_link_libraries(round_lcd wiringPi)
endif()