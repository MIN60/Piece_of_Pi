add_library(game SHARED game.c ${PROJECT_SOURCE_DIR}/common/utils.c)

target_include_directories(game
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}
        ${PROJECT_SOURCE_DIR}/common
)

target_link_libraries(game
    buzzer
    segment
    dl
    pthread
)
