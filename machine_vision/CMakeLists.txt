cmake_minimum_required(VERSION 3.10)
project(machine_vision)

set(CMAKE_CXX_STANDARD 17)

set(OpenCV_DIR "D:/Opencv/opencv/build")
find_package(OpenCV REQUIRED)

add_executable(machine_vision main.cpp)
target_include_directories(machine_vision PRIVATE ${OpenCV_INCLUDE_DIRS})
target_link_libraries(machine_vision ${OpenCV_LIBS})
