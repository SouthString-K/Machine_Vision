cmake_minimum_required(VERSION 3.10)
project(machine_vision)

set(CMAKE_CXX_STANDARD 17)

set(OpenCV_DIR "D:/Opencv/opencv/build")
find_package(OpenCV REQUIRED)

foreach(t paperclips coins holes)
    add_executable(${t} src/${t}.cpp)
    target_include_directories(${t} PRIVATE ${OpenCV_INCLUDE_DIRS})
    target_link_libraries(${t} ${OpenCV_LIBS})
endforeach()
