cmake_minimum_required(VERSION 3.16)
project(RayTrace LANGUAGES CXX)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
option(BUILD_SHARED_LIBS "Build shared libraries" OFF)

# SET(OpenCV_DIR "{OpenCV Directory}")

find_package(OpenCV REQUIRED)

MESSAGE(STATUS "CHECKING Include dirs ${OpenCV_INCLUDE_DIRS}")
MESSAGE(STATUS "CHECKING Link Libraries ${OpenCV_LIBS}")

file(GLOB SOURCES "src/*.cpp")

add_executable(RayTrace ${SOURCES})

target_include_directories(RayTrace PRIVATE ${PROJECT_SOURCE_DIR})
include_directories(RayTrace PRIVATE ${OpenCV_INCLUDE_DIRS})
target_link_libraries(RayTrace PRIVATE ${OpenCV_LIBS})
target_compile_features(RayTrace PRIVATE cxx_std_20)

install (TARGETS RayTrace)