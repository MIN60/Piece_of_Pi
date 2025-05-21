add_library(led SHARED led.c)
target_include_directories(led PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
