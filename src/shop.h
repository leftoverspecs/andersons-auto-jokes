#ifndef GGJ2024_SHOP_H
#define GGJ2024_SHOP_H

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

class Shop : public Scene {
public:
    Shop(SDL_Window *window,
          int screen_width, int screen_height,
          engine::Font &font,
          engine::SpriteMap &family);

    void startup(const std::vector<const common::Stats *> &old_team,
                 const std::vector<const common::Stats *> &new_available);

    [[nodiscard]] std::vector<const common::Stats *> get_team() const;

private:
    enum class State {
        FADE_IN,
        SHOPPING,
        DRAGGING,
        DROPPING,
        FADE_OUT,
    };

    int screen_height;
    engine::Font &font;

    engine::Destination destination;
    engine::SpriteRenderer family_renderer;
    engine::BoxRenderer box_renderer;
    engine::TextBoxRenderer textboxes;

    game::Background background;
    std::vector<Person> team;
    std::vector<Person> available;

    Person *dragging{nullptr};
    float dragging_initial_x{0};
    float dragging_initial_y{0};
    Person *dragging_source{nullptr};

    State state{State::FADE_IN};
    float alpha{0.0f};

    void update(float delta_time);
    void on_loop(float delta_time) override;
    void on_key_pressed(SDL_Keycode code) override;
    void on_mouse_button_down(int x, int y) override;
    void on_mouse_button_up(int x, int y) override;
};

}

#endif//GGJ2024_SHOP_H
