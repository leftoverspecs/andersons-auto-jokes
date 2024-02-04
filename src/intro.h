#ifndef GGJ2024_INTRO_H
#define GGJ2024_INTRO_H

#include "background.h"
#include "person.h"
#include "scene.h"

#include <boxrenderer.h>
#include <destination.h>
#include <spriterenderer.h>
#include <textboxrenderer.h>

namespace engine {

class Font;
class SpriteMap;

}

namespace game {

class Intro : public Scene {
public:
    Intro(SDL_Window *window,
          int screen_width, int screen_height,
          engine::Font &font,
          game::Speech &speech,
          game::AudioData &audio_data,
          engine::SpriteMap &family);

    bool start_own_server() const { return own_server; }
    const std::string &get_hostname() const { return hostname; }
private:
    enum class State {
        FADE_IN,
        SELECTING,
        FADE_OUT,
    };

    int screen_height;
    engine::Font &font;
    game::Speech &speech;

    engine::Destination destination;
    engine::SpriteRenderer family_renderer;
    engine::BoxRenderer box_renderer;
    engine::TextBoxRenderer textboxes;

    game::Background background;
    Person father;
    Person grandfather;

    State state{State::FADE_IN};
    float alpha{0.0f};
    bool own_server{false};
    std::string hostname{"localhost"};
    bool input{false};

    void update(float delta_time);
    void on_loop(float delta_time) override;
    void on_key_pressed(SDL_Keycode code) override;
    void on_mouse_button_up(int x, int y) override;
};

}

#endif//GGJ2024_INTRO_H
