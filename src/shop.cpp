#include "shop.h"

#include <font.h>

#include <shop.png.h>

namespace game {

Shop::Shop(SDL_Window *window,
           int screen_width, int screen_height,
           engine::Font &font,
           engine::SpriteMap &family)
    : Scene(window),
      screen_height{screen_height},
      font{font},
      family{family},
      destination(screen_width, screen_height),
      family_renderer(family, screen_width, screen_height),
      box_renderer(screen_width, screen_height),
      textboxes{font, box_renderer},
      background(screen_width, screen_height, shop, sizeof(shop))
{}

void Shop::update(float delta_time) {
    if (state == State::FADE_IN) {
        alpha += delta_time * 0.001f;
        if (alpha > 0.9f) {
            alpha = 1.0;
            state = State::SHOPPING;
        }
    } else if (state == State::FADE_OUT) {
        alpha -= delta_time * 0.001f;
        if (alpha < 0.1f) {
            alpha = 0.0f;
            exit();
        }
    }
}

void Shop::on_loop(float delta_time) {
    {
        auto binding = destination.bind_as_target();
        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        family_renderer.clear();
        font.clear();
        box_renderer.clear();
        update(delta_time);
        for (auto &person : team) {
            person.update(delta_time);
            person.queue();
        }
        background.draw();
        family_renderer.draw();
        box_renderer.draw();
        font.draw();
    }
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);
    destination.draw(glm::mat4(1.0f), glm::vec3(0.0f), alpha, glm::vec3(1.0f));
}

void Shop::startup(const std::vector<Person::Stats> &old_team_stats) {
    team.clear();
    team.reserve(old_team_stats.size());
    float x = 100.0f;
    for (const auto &s : old_team_stats) {
        Person person{screen_height, family_renderer, font, box_renderer, textboxes, s};
        person.stand(x, 300.0f);
        x += 50.0f;
        team.push_back(person);
    }
    state = State::FADE_IN;
    alpha = 0.0f;
}

std::vector<Person::Stats> Shop::get_team() const {
    std::vector<Person::Stats> result;
    for (const auto &p : team) {
        result.push_back(p.get_stats());
    }
    return result;
}

void Shop::on_key_pressed(SDL_Keycode code) {
    if (code == SDLK_RETURN) {
        state = State::FADE_OUT;
    }
}

}
