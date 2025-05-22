cmake_minimum_required(VERSION 3.10)
project(PoP C)

set(CMAKE_C_STANDARD 99)

# 하위 모듈 빌드
add_subdirectory(common)
add_subdirectory(client)
add_subdirectory(server)
add_subdirectory(lib/led)
add_subdirectory(lib/buzzer)
add_subdirectory(lib/cds)
add_subdirectory(lib/segment)
add_subdirectory(lib/game)
