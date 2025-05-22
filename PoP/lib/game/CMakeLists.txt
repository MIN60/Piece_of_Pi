add_library(game SHARED game.c)

target_include_directories(game PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
target_link_libraries(game buzzer segment) 
