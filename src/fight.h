#ifndef GGJ2024_FIGHT_H
#define GGJ2024_FIGHT_H

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

class Fight : public Scene {
public:
    enum class Winner {
        UNDECIDED,
        TEAM1,
        TEAM2,
        NOBODY,
    };

    Fight(SDL_Window *window,
          int screen_width, int screen_height,
          engine::Font &font,
          AudioData &audio_data,
          engine::SpriteMap &family);

    void startup(const std::vector<const common::Stats *> &team1_stats,
                 const std::vector<common::Stats> &team2_stats);

    [[nodiscard]]
    Winner get_result() const;

private:
    enum class State {
        FADE_IN,
        PREPARE,
        PREPARING,
        BOTH_PREPARED,
        STARING,
        FIRST_TALKING,
        SECOND_TALKING,
        TENSION,
        REACTION,
        FADE_OUT,
    };

    void update(float delta_time);
    void on_loop(float delta_time) override;

    int screen_height;
    engine::Font &font;
    AudioData &audio_data;
    engine::SpriteMap &family;

    engine::Destination destination;
    engine::SpriteRenderer family_renderer;
    engine::BoxRenderer box_renderer;
    engine::TextBoxRenderer textboxes;

    game::Background background;
    std::vector<Person> team1;
    std::vector<Person> team2;

    State state{State::FADE_IN};
    float timer{0.0f};
    float alpha{1.0f};
    int current_person1{0};
    int current_person2{0};
};

}

#endif//GGJ2024_FIGHT_H
