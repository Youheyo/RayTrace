cmake_minimum_required(VERSION 3.16)
project(RayTrace LANGUAGES CXX)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
option(BUILD_SHARED_LIBS "Build shared libraries" OFF)

SET(OpenCV_DIR "C:/Users/Bryan/Downloads/opencv/sources/build/install")

find_package(OpenCV REQUIRED)

MESSAGE(STATUS "CHECKING Include dirs ${OpenCV_INCLUDE_DIRS}")
MESSAGE(STATUS "CHECKING Link Libraries ${OpenCV_LIBS}")

file(GLOB SOURCES "src/*.cpp")

add_executable(RayTrace ${SOURCES})

target_include_directories(RayTrace PRIVATE ${PROJECT_SOURCE_DIR} ${OpenCV_INCLUDE_DIRS})
target_link_libraries(RayTrace PRIVATE ${OpenCV_LIBS})
target_compile_features(RayTrace PRIVATE cxx_std_20)

if(WIN32)
    set_target_properties(RayTrace PROPERTIES WIN32_EXECUTABLE TRUE)

    add_custom_command(
        TARGET RayTrace
        COMMENT "Copy OpenCV DLL Files"
        PRE_BUILD COMMAND ${CMAKE_COMMAND} -E copy_if_different
            # ${OpenCV_DIR}/x64/mingw/bin/*.dll
            ${OpenCV_DIR}/x64/mingw/bin/libopencv_video490d.dll 
            ${OpenCV_DIR}/x64/mingw/bin/libopencv_videoio490d.dll 
            ${OpenCV_DIR}/x64/mingw/bin/libopencv_calib3d490d.dll 
            ${OpenCV_DIR}/x64/mingw/bin/libopencv_core490d.dll 
            ${OpenCV_DIR}/x64/mingw/bin/libopencv_dnn490d.dll 
            ${OpenCV_DIR}/x64/mingw/bin/libopencv_features2d490d.dll 
            ${OpenCV_DIR}/x64/mingw/bin/libopencv_flann490d.dll 
            ${OpenCV_DIR}/x64/mingw/bin/libopencv_gapi490d.dll 
            ${OpenCV_DIR}/x64/mingw/bin/libopencv_highgui490d.dll 
            ${OpenCV_DIR}/x64/mingw/bin/libopencv_imgcodecs490d.dll 
            ${OpenCV_DIR}/x64/mingw/bin/libopencv_imgproc490d.dll 
            ${OpenCV_DIR}/x64/mingw/bin/libopencv_ml490d.dll 
            ${OpenCV_DIR}/x64/mingw/bin/libopencv_objdetect490d.dll 
            ${OpenCV_DIR}/x64/mingw/bin/libopencv_photo490d.dll 
            ${OpenCV_DIR}/x64/mingw/bin/libopencv_stitching490d.dll
            $<TARGET_FILE_DIR:RayTrace>
        VERBATIM)

endif()

set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)

install (TARGETS RayTrace)