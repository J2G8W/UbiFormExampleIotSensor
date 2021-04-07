cmake_minimum_required(VERSION 3.17)
project(UbiFormExampleIotSensor)

set(CMAKE_CXX_STANDARD 14)


include_directories(libs/UbiForm/include)
add_subdirectory(libs/UbiForm/CMake build)

add_executable(UbiFormExampleIotSensor main.cpp)
target_link_libraries(UbiFormExampleIotSensor UbiForm)
configure_file(Manifest.json ${CMAKE_CURRENT_BINARY_DIR}/Manifest.json COPYONLY)