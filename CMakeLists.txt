cmake_minimum_required(VERSION 3.5)
project(SimpleChat)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_AUTOMOC ON)
find_package(Qt5 COMPONENTS Widgets Network REQUIRED)

# Client executable
add_executable(chat_client chatClient.cpp chatClient.h)

# Server executable
add_executable(chat_server chatServer.cpp)

add_subdirectory(cpp-dotenv)

# set_target_properties(chat_client PROPERTIES AUTOMOC TRUE)
target_link_libraries(chat_client
                    Qt5::Widgets
                    Qt5::Network
                    cpp_dotenv
)
target_link_libraries(chat_server Qt5::Widgets)
