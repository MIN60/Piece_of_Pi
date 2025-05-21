# 자동 toolchain 적용 (최초 CMake 실행 시에만 반영)
if(CMAKE_SOURCE_DIR STREQUAL CMAKE_CURRENT_SOURCE_DIR)
    if(NOT DEFINED CMAKE_TOOLCHAIN_FILE)
        message(STATUS "toolchain-arm64.cmake")
        set(CMAKE_TOOLCHAIN_FILE "${CMAKE_SOURCE_DIR}/toolchain-arm64.cmake" CACHE STRING "" FORCE)
    endif()
endif()

cmake_minimum_required(VERSION 3.10)
project(PoP C)

set(CMAKE_C_STANDARD 99)

# 하위 모듈 빌드
add_subdirectory(common)
# add_subdirectory(client)
add_subdirectory(server)
add_subdirectory(lib/led)
add_subdirectory(lib/buzzer)
add_subdirectory(lib/sensor)
add_subdirectory(lib/segment)
