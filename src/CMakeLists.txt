add_executable(game WIN32
        background.cpp
        background.h
        game.cpp
        person.cpp
        person.h
        fight.cpp
        fight.h
        arena.cpp
        arena.h
        scene.cpp
        scene.h
)
target_link_libraries(game PRIVATE game_assets boxyfont engine)
