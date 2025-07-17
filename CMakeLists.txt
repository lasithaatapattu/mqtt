cmake_minimum_required(VERSION 3.10)
project(mqtt_redboltz_project)

set(CMAKE_CXX_STANDARD 14)

include_directories(mqtt_cpp_include)

find_package(Boost REQUIRED COMPONENTS system thread)
include_directories(${Boost_INCLUDE_DIRS})

add_executable(client src/client.cpp)
add_executable(client1 src/client1.cpp)
add_executable(subscriber src/subscriber.cpp)

target_link_libraries(client ${Boost_LIBRARIES})
target_link_libraries(client1 ${Boost_LIBRARIES})
target_link_libraries(subscriber ${Boost_LIBRARIES})
