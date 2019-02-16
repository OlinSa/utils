aux_source_directory(. CAMERA_SRC)
add_executable(camera ${CAMERA_SRC})
include_directories(/usr/local/include/opencv4)
target_link_libraries(camera opencv_core opencv_video opencv_highgui opencv_imgproc opencv_ml opencv_videoio)