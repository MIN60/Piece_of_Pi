add_executable(pop_client client.c)
target_include_directories(pop_client PRIVATE ../common include)
target_link_libraries(pop_client ssl crypto)
