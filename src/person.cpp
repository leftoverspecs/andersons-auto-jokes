#include "person.h"

#include <boxrenderer.h>
#include <font.h>
#include <spriterenderer.h>
#include <textboxrenderer.h>

#include <glm/gtc/matrix_transform.hpp>

#include <SDL_mouse.h>

namespace {

const float SPEED = 0.10f;
const float CAPACITY_CHANGE = 0.05;

}

namespace game {

Person::Person(int screen_height, engine::SpriteRenderer &renderer,
               engine::Font &font,
               engine::BoxRenderer &box,
               engine::TextBoxRenderer &textboxes,
               const Stats *prototype)
    : screen_height{screen_height},
      renderer{&renderer}, font{&font}, box{&box}, textboxes{&textboxes},
      state{State::STANDING},
      current_capacity{prototype->capacity}, destination_capacity{prototype->capacity},
      prototype{prototype}, current_stats{*prototype} {}

void Person::update(float delta_time) {
    time += delta_time;
    const float speed = (state == State::LAUGHING ? 10.0f : 1.0f) * SPEED;
    if (std::fabs(current_x - destination_x) < 1) {
        current_x = destination_x;
    } else if (current_x < destination_x) {
        current_x += speed * delta_time;
    } else if (current_x > destination_x) {
        current_x -= speed * delta_time;
    }
    if (std::fabs(current_y - destination_y) < 1) {
        current_y = destination_y;
    } else if (current_y < destination_y) {
        current_y += speed * delta_time;
    } else if (current_y > destination_y) {
        current_y -= speed * delta_time;
    }
    if (std::fabs(current_capacity - destination_capacity) < 1) {
        current_capacity = destination_capacity;
        if (current_capacity < 1.0f) {
            state = State::LAUGHING;
        } else {
            state = State::SERIOUS;
        }
    } else {
        current_capacity -= CAPACITY_CHANGE * delta_time;
    }
    if (current_stats.sprite_row > 0 && state == State::LAUGHING) {
        current_angle += 0.1f;
    }
}

void Person::stand(float x, float y, bool looks_right) {
    state = State::STANDING;
    current_x = destination_x = x;
    current_y = destination_y = y;
    this->looks_right = looks_right;
}

void Person::walk_to(float x, float y) {
    state = State::WALKING;
    destination_x = x;
    destination_y = y;
}

void Person::queue(bool silent) {
    int x;
    int y;
    SDL_GetMouseState(&x, &y);
    y = screen_height - y;
    const bool inside = this->inside(x, y);
    const float inside_extra = inside ? 1.0f : 0.0f;

    const int current_sprite_column = 0; //static_cast<int>(state) * 4 + static_cast<int>(time / 1000.0) % 4;
    glm::mat4 model{1.0f};
    if (looks_right) {
        model = glm::translate(model, glm::vec3(current_x, current_y, 0.0f));
        model = glm::rotate(model, current_angle, glm::vec3(0.0f, 0.0f, 1.0f));
        model = glm::scale(model, glm::vec3(64.0f, 128.0f, 1.0f));
    } else {
        model = glm::translate(model, glm::vec3(current_x + 64.0f, current_y, 0.0f));
        model = glm::rotate(model, current_angle, glm::vec3(0.0f, 0.0f, 1.0f));
        model = glm::scale(model, glm::vec3(-64.0f, 128.0f, 1.0f));
    }
    renderer->queue(model, glm::vec4(1.0f + inside_extra, 1.0f + inside_extra, 1.0f + inside_extra, 1.0f), current_sprite_column, current_stats.sprite_row);

    if (current_stats.sprite_row > 0 && state != State::LAUGHING) {
        model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(current_x + 10.0f, current_y - 10.0f, 0.0f));
        model = glm::scale(model, glm::vec3(20.0f, 20.0f, 1.0f));
        font->write(model, glm::vec4(1.0f, 1.0f, 1.0f, 1.0f), std::to_string(static_cast<int>(current_stats.giddy)).c_str());

        model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(current_x + 54.0f, current_y - 10.0f, 0.0f));
        model = glm::scale(model, glm::vec3(20.0f, 20.0f, 1.0f));
        font->write(model, glm::vec4(1.0f, 1.0f, 1.0f, 1.0f), std::to_string(static_cast<int>(current_stats.funny)).c_str());

        model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(current_x, current_y + 138.0f, 0.0f));
        model = glm::scale(model, glm::vec3(std::max(current_capacity * 6, 0.0f), 10.0f, 1.0f));
        box->queue_frame(model, glm::vec4(1.0f - current_capacity / current_stats.capacity, current_capacity / current_stats.capacity, 0.0f, 1.0f));

        if (inside && !silent) {
            textboxes->queue(current_x + 50.0f, current_y - 20.0f, 300.0f, 40.0f, 2.0f, 3.0f, current_stats.description, glm::vec4(1.0f, 1.0f, 1.0f, 1.0f), glm::vec4(1.0f, 1.0f, 0.0f, 1.0f));
        }
    }
}

bool Person::arrived() const {
    return current_x == destination_x && current_y == destination_y;
}

void Person::fly_away() {
    state = State::LAUGHING;
    destination_y = 800;
}

void Person::talk() {
    state = State::TALKING;
}

void Person::fighting(int own_position,
                      const Person &opponent,
                      int opponent_position,
                      std::vector<Person> &team,
                      std::vector<Person> &opponents) {
    state = State::FIGHTING;
    destination_capacity -= opponent.current_stats.funny * current_stats.giddy;
}

bool Person::fought() const {
    return destination_capacity == current_capacity;
}

bool Person::defeated() const {
    return current_capacity < 1.0f;
}

void Person::hear() {
    state = State::HEARING;
}

bool Person::inside(int x, int y) const {
    return x >= current_x && x <= current_x + 64.0f && y >= current_y && y <= current_y + 128.0f;
}

void Person::drop(const Person::Stats *new_prototype) {
    current_stats = *new_prototype;
    prototype = new_prototype;
    current_capacity = prototype->capacity;
    destination_capacity = prototype->capacity;
}

}