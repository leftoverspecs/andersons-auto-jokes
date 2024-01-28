#ifndef ANDERSONS_AUTO_JOKES_RESULT_H
#define ANDERSONS_AUTO_JOKES_RESULT_H

#include "scene.h"
#include "fight.h"

namespace engine {

class Font;

}

namespace game {

class Result : public Scene {
public:
    Result(int height, SDL_Window *window, engine::Font &font);

    void startup(Fight::Winner winner, int losses);

private:
    enum class State {
        FADE_IN,
        PRESENTING,
        FADE_OUT,
    };
    engine::Font &font;

    State state;
    Fight::Winner winner{Fight::Winner::UNDECIDED};
    int losses{0};
    float alpha{0.0f};

    void on_loop(float delta_time) override;
    void on_mouse_button_up(int x, int y) override;
};

}// namespace game

#endif//ANDERSONS_AUTO_JOKES_RESULT_H
