cmake_minimum_required(VERSION 3.10)
project(PoP C)

set(CMAKE_C_STANDARD 99)

include_directories(common)

add_subdirectory(common)
add_subdirectory(client)
add_subdirectory(server)
add_subdirectory(lib/led)
add_subdirectory(lib/buzzer)
add_subdirectory(lib/sensor)
add_subdirectory(lib/segment)
