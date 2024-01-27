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
)
target_link_libraries(andersonsautojokes PRIVATE game_assets boxyfont engine common)
