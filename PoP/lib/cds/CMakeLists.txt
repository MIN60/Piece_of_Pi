add_library(cds SHARED cds.c)
target_include_directories(cds PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
target_link_libraries(cds wiringPi dl pthread)
