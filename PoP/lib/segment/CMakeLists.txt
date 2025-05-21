add_library(segment SHARED segment.c)
target_include_directories(segment PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
target_link_libraries(segment wiringPi dl pthread)
