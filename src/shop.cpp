#include "shop.h"

#include <font.h>

#include <iostream>
#include <shop.png.h>

namespace game {

Shop::Shop(SDL_Window *window,
           int screen_width, int screen_height,
           engine::Font &font,
           engine::SpriteMap &family)
    : Scene(screen_height, window),
      screen_height{screen_height},
      font{font},
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
    } else if (state == State::DRAGGING) {
        if (dragging) {
            int x;
            int y;
            SDL_GetMouseState(&x, &y);
            y = screen_height - y;
            dragging->stand(x - 32.0f, y - 32.0f, true);
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
            person.queue(false);
        }
        for (auto &person : available) {
            person.update(delta_time);
            person.queue(false);
        }
        if (dragging) {
            dragging->update(delta_time);
            dragging->queue(true);
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

void Shop::startup(const std::vector<const common::Stats *> &old_team,
                   const std::vector<const common::Stats *> &new_available) {
    team.clear();
    team.reserve(old_team.size());
    float x = 100.0f;
    for (const auto s : old_team) {
        Person person{screen_height, family_renderer, font, box_renderer, textboxes, s};
        person.stand(x, 400.0f, true);
        x += 100.0f;
        team.push_back(person);
    }

    available.clear();
    available.reserve(new_available.size());
    x = 100.0f;
    for (const auto s : new_available) {
        Person person{screen_height, family_renderer, font, box_renderer, textboxes, s};
        person.stand(x, 100.0f, true);
        x += 100.0f;
        available.push_back(person);
    }

    state = State::FADE_IN;
    alpha = 0.0f;
}

std::vector<const common::Stats *> Shop::get_team() const {
    std::vector<const common::Stats *> result;
    for (const auto &p : team) {
        result.push_back(p.get_stats_prototype());
    }
    return result;
}

void Shop::on_key_pressed(SDL_Keycode code) {
    if (code == SDLK_RETURN) {
        state = State::FADE_OUT;
    } else if (code == SDLK_PAGEUP) {
        destination.set_gamma(destination.get_gamma() + 0.1f);
    } else if (code == SDLK_PAGEDOWN) {
        destination.set_gamma(destination.get_gamma() - 0.1f);
    } else if (code == SDLK_UP) {
        destination.set_exposure(destination.get_exposure() + 0.1f);
    } else if (code == SDLK_DOWN) {
        destination.set_exposure(destination.get_exposure() - 0.1f);
    }
}

void Shop::on_mouse_button_down(int x, int y) {
    std::cout << "button down\n";
    if (state == State::SHOPPING) {
        for (auto &p: available) {
            if (p.inside(x, y)) {
                std::cout << "dragging\n";
                dragging = &p;
                dragging_initial_x = p.get_current_x();
                dragging_initial_y = p.get_current_y();
                dragging_source = &p;
                state = State::DRAGGING;
                break;
            }
        }
    }
}

void Shop::on_mouse_button_up(int x, int y) {
    std::cout << "button up\n";
    if (state == State::DRAGGING && dragging != nullptr) {
        for (auto &p : team) {
            if (p.inside(x, y)) {
                std::cout << "dropping onto\n";
                common::Stats empty;
                p.drop(dragging->get_stats_prototype());
                dragging_source->drop(&empty);
                break;
            }
        }
        dragging->stand(dragging_initial_x, dragging_initial_y, true);
        dragging = nullptr;
        dragging_source = nullptr;
        state = State::SHOPPING;
    }
}

}
