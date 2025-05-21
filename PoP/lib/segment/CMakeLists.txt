add_library(segment SHARED segment.c)
target_include_directories(segment PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
