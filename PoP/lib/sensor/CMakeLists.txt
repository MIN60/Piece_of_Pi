add_library(sensor SHARED sensor.c)
target_include_directories(sensor PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
