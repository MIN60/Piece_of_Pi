add_library(buzzer SHARED buzzer.c)

target_include_directories(buzzer PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
target_link_libraries(buzzer wiringPi pthread)
