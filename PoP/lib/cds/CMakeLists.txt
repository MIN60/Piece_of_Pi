add_library(cds SHARED cds.c ${PROJECT_SOURCE_DIR}/common/utils.c) 

target_include_directories(cds
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}
        ${PROJECT_SOURCE_DIR}/common
)

target_link_libraries(cds
    wiringPi
    dl
    pthread
)
