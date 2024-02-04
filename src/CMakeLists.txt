add_executable(andersonsautojokes WIN32
        audio_data.cpp
        audio_data.h
        background.cpp
        background.h
        game.cpp
        person.cpp
        person.h
        fight.cpp
        fight.h
        scene.cpp
        scene.h
        shop.h
        shop.cpp
        lobby.cpp
        lobby.h
        result.cpp
        result.h
        speech.cpp
        speech.h
        family.cpp
        family.h
        music.cpp
        music.h
        intro.cpp
        intro.h
)
target_link_libraries(andersonsautojokes PRIVATE game_assets boxyfont engine common SDL2::SDL2main)
add_dependencies(andersonsautojokes server)
