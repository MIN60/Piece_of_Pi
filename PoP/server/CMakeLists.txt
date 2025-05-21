add_executable(pop_server main.c command.c daemon.c)
target_include_directories(pop_server PRIVATE ../common include)

# target_link_libraries(pop_server ssl crypto dl pthread)

# 수정
target_link_libraries(pop_server dl pthread)