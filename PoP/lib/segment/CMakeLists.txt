add_library(segment SHARED segment.c ${PROJECT_SOURCE_DIR}/common/utils.c)

target_include_directories(segment
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}
        ${PROJECT_SOURCE_DIR}/common
)

target_link_libraries(segment
    wiringPi
    dl
    pthread
)
