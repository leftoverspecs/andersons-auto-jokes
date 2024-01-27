#ifndef GGJ2024_SHOP_H
#define GGJ2024_SHOP_H

#include "background.h"
#include "person.h"
#include "scene.h"

#include <boxrenderer.h>
#include <destination.h>
#include <spriterenderer.h>

namespace engine {

class Font;
class SpriteMap;

}

class Shop : public Scene {
public:
    Shop(SDL_Window *window,
          int screen_width, int screen_height,
          engine::Font &font,
          engine::SpriteMap &family);

    void startup(const std::vector<Person::Stats> &old_team_stats);
    [[nodiscard]] std::vector<Person::Stats> get_team() const;

private:
    enum class State {
        FADE_IN,
        SHOPPING,
        FADE_OUT,
    };

    engine::Font &font;
    engine::SpriteMap &family;

    engine::Destination destination;
    engine::SpriteRenderer family_renderer;
    engine::BoxRenderer box_renderer;

    game::Background background;
    std::vector<Person> team;

    State state{State::FADE_IN};
    float alpha{0.0f};

    void update(float delta_time);
    void on_loop(float delta_time) override;
    void on_key_pressed(SDL_Keycode code) override;
};


#endif//GGJ2024_SHOP_H
