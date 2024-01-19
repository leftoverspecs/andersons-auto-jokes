add_executable(chestnutchase WIN32
    background.cpp
    background.h
    chestnut.cpp
    chestnut.h
    chestnuts.cpp
    chestnuts.h
    game.cpp
    health.cpp
    health.h
    leaves.cpp
    leaves.h
    player.cpp
    player.h
    score.cpp
    score.h
    screen.cpp
    screen.h
)
target_link_libraries(chestnutchase PRIVATE
    SDL2::SDL2
    SDL2::SDL2main
    GLEW::glew
    OpenGL::GL
    assets
    engine
)

install(TARGETS chestnutchase
    RUNTIME_DEPENDENCIES
        DIRECTORIES "${CMAKE_PREFIX_PATH}/bin"
        PRE_EXCLUDE_REGEXES "api-ms-" "ext-ms-" "hvsifiletrust" "pdmutilities"
        POST_EXCLUDE_REGEXES ".*WINDOWS[\\/]system32.*"
    RUNTIME)
