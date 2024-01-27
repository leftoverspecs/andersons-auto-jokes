#include "person.h"

#include <boxrenderer.h>
#include <font.h>
#include <spriterenderer.h>

#include <glm/gtc/matrix_transform.hpp>
#include <iostream>

namespace {

const float SPEED = 0.10f;
const float CAPACITY_CHANGE = 0.05;

}

Person::Person(engine::SpriteRenderer &renderer,
               engine::Font &font,
               engine::BoxRenderer &box,
               const Stats &stats)
    : renderer{&renderer}, font{&font}, box{&box},
      state{State::STANDING},
      current_capacity{stats.capacity}, destination_capacity{stats.capacity},
      stats{stats} {}

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
}

void Person::stand(float x, float y) {
    state = State::STANDING;
    current_x = destination_x = x;
    current_y = destination_y = y;
}

void Person::walk_to(float x, float y) {
    state = State::WALKING;
    destination_x = x;
    destination_y = y;
}

void Person::queue() {
    const int current_sprite_column = 0; //static_cast<int>(state) * 4 + static_cast<int>(time / 1000.0) % 4;
    glm::mat4 model{1.0f};
    model = glm::translate(model, glm::vec3(current_x, current_y, 0.0f));
    model = glm::rotate(model, current_angle, glm::vec3(0.0f, 0.0f, 1.0f));
    model = glm::scale(model, glm::vec3(32.0f, 32.0f, 1.0f));
    renderer->queue(model, glm::vec4(1.0f, 1.0f, 1.0f, 1.0f), current_sprite_column, stats.sprite_row);

    model = glm::mat4(1.0f);
    model = glm::translate(model, glm::vec3(current_x - 10.0f, current_y - 10.0f, 0.0f));
    model = glm::scale(model, glm::vec3(20.0f, 20.0f, 1.0f));
    font->write(model, glm::vec4(1.0f, 1.0f, 1.0f, 1.0f), std::to_string(static_cast<int>(stats.giddy)).c_str());

    model = glm::mat4(1.0f);
    model = glm::translate(model, glm::vec3(current_x + 32.0f, current_y - 10.0f, 0.0f));
    model = glm::scale(model, glm::vec3(20.0f, 20.0f, 1.0f));
    font->write(model, glm::vec4(1.0f, 1.0f, 1.0f, 1.0f), std::to_string(static_cast<int>(stats.funny)).c_str());

    model = glm::mat4(1.0f);
    model = glm::translate(model, glm::vec3(current_x, current_y + 32.0f, 0.0f));
    model = glm::scale(model, glm::vec3(std::max(current_capacity * 4, 0.0f), 10.0f, 1.0f));
    box->queue_frame(model, glm::vec4(1.0f - current_capacity / stats.capacity, current_capacity / stats.capacity, 0.0f, 1.0f));
}

bool Person::arrived() const {
    return current_x == destination_x && current_y == destination_y;
}

void Person::fly_away() {
    state = State::LAUGHING;
    destination_y = 700;
}

void Person::talk() {
    state = State::TALKING;
}

void Person::fighting(const Person &other) {
    state = State::FIGHTING;
    destination_capacity -= other.stats.funny * stats.giddy;
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
