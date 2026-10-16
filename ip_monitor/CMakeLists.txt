# CMakeLists.txt
cmake_minimum_required(VERSION 3.16)
project(ip_monitor)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_AUTOMOC ON)

find_package(Qt6 REQUIRED COMPONENTS Core)   # Qt5 also works

add_executable(ip_monitor main.cpp)

target_link_libraries(ip_monitor Qt6::Core)
