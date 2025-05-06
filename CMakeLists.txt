cmake_minimum_required(VERSION 3.5)
project(MyApp)

set(CMAKE_CXX_STANDARD 11)
find_package(Qt5 COMPONENTS Widgets REQUIRED)

add_executable(MyApp main.cpp)
target_link_libraries(MyApp Qt5::Widgets)
