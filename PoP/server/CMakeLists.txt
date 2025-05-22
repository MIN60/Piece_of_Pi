add_executable(pop_server main.c command.c web_server.c)
target_include_directories(pop_server PRIVATE ../common include)
target_link_libraries(pop_server cds led buzzer segment game dl pthread)
