
aux_source_directory(. SRC)
add_executable(round_lcd ${SRC})
target_link_libraries(round_lcd wiringPi)