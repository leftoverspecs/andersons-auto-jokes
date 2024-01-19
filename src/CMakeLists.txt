add_executable(game WIN32
        background.cpp
        background.h
        game.cpp)
target_link_libraries(game PRIVATE assets engine)
