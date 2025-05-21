# lib/led/CMakeLists.txt

# 공유 라이브러리 libled.so로 빌드
add_library(led SHARED led.c)
target_include_directories(led PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

target_compile_options(led PRIVATE -Wall -O2 -fPIC)
target_link_libraries(led
    wiringPi
    wiringPiDev
    crypt
    pthread
    dl
)
