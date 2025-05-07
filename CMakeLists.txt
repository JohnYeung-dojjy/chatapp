cmake_minimum_required(VERSION 3.5)
project(SimpleChat)

set(CMAKE_CXX_STANDARD 11)
find_package(Qt5 COMPONENTS Widgets REQUIRED)

# Client executable
add_executable(chat_client chatClient.cpp)

# Server executable
add_executable(chat_server chatServer.cpp)

target_link_libraries(chat_client Qt5::Widgets)
target_link_libraries(chat_server Qt5::Widgets)
