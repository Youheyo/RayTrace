cmake_minimum_required(VERSION 3.16)
project(RayTrace LANGUAGES CXX)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

include_directories(${OpenCV_INCLUDE_DIRS})

file(GLOB SOURCES "src/*.cpp")

add_executable(RayTrace ${SOURCES})

target_include_directories(RayTrace PRIVATE ${PROJECT_SOURCE_DIR})
target_link_libraries(RayTrace PRIVATE ${OpenCV_LIBS})
target_compile_features(RayTrace PRIVATE cxx_std_20)

set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)

install (TARGETS RayTrace)