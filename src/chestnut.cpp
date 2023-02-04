#include "chestnut.h"

#include <glm/gtc/matrix_transform.hpp>

#include <chestnut.png.h>

namespace game {

Chestnut::Chestnut(float x, float growth_rate, float max_length, float screen_width, float screen_height)
  : sprites(chestnut, sizeof(chestnut), 3, 1),
    renderer(sprites, screen_width, screen_height),
    screen_height(screen_height),
    screen_width(screen_width),
    growth_rate(growth_rate),
    max_length(max_length),
    time(0.0f),
    state(State::HANGING),
    position(x, screen_height),
    velocity(0.0f, 0.0f),
    fruit(position.x, position.y, 64.0f, 64.0f, screen_width, screen_height),
    stem(0.0f, 0.0f, 20.0f, 32.0f, screen_width, screen_height)
{ }

void Chestnut::update(float msec) {
    switch (state) {
    case State::HANGING:
        time += msec;
        position.y = screen_height - max_length * growth_rate * time / (growth_rate * time + max_length);
        break;
    case State::FALLING_CLOSED_PLAYER1:
    case State::FALLING_CLOSED_PLAYER2:
    case State::FALLING_OPEN_PLAYER1:
    case State::FALLING_OPEN_PLAYER2:
        velocity.y += -0.001f * msec;
        position += msec * velocity;
        if (position.y < 25.0f) {
            position.y = 25.0f;
            velocity.y = 0.9f * std::abs(velocity.y);
        }
        if (position.x < 25.0f || position.x > screen_width - 45.0f) {
            velocity.x = -velocity.x;
        }
        break;
    }
    fruit.relocate(position.x - 32.0, position.y - 32.0);
    stem.relocate(position.x - 10.0, position.y + 32.0);
    stem.resize(0.0f, screen_height - position.y);
}

void Chestnut::draw() {
    renderer.clear();

    if (state == State::HANGING) {
        const int num = static_cast<int>(screen_height - position.y / 32.0f);
        for (int i = 0; i < num; ++i) {
            glm::mat4 model(1.0f);
            model = glm::translate(model, glm::vec3(position.x, position.y + 16.0 + i * 64.0, 0.0f));
            model = glm::scale(model, glm::vec3(32.0f, 64.0f, 1.0f));
            model = glm::translate(model, glm::vec3(-0.5f, 0.0f, 0.0f));
            renderer.queue(model, glm::vec4(1.0f, 1.0f, 1.0f, 1.0f), 2, 0);
        }
    }

    glm::mat4 model(1.0f);
    model = glm::translate(model, glm::vec3(position, 0.0f));
    model = glm::scale(model, glm::vec3(64.0f, 64.0f, 1.0f));
    model = glm::translate(model, glm::vec3(-0.5f, -0.5f, 0.0f));
    if (state == State::HANGING || state == State::FALLING_CLOSED_PLAYER1 || state == State::FALLING_CLOSED_PLAYER2) {
        renderer.queue(model, glm::vec4(1.0f, 1.0f, 1.0f, 1.0f), 0, 0);
    } else if (state == State::FALLING_OPEN_PLAYER1 || state == State::FALLING_OPEN_PLAYER2) {
        renderer.queue(model, glm::vec4(1.0f, 1.0f, 1.0f, 1.0f), 1, 0);
    }

    renderer.draw();
    fruit.draw(glm::vec4(1.0f, 1.0f, 1.0f, 1.0f));
    stem.draw(glm::vec4(1.0f, 0.0f, 0.0f, 1.0f));
}

void Chestnut::hit(bool female, const engine::Box &sword, glm::vec2 player_velocity) {
    switch (state) {
    case State::HANGING:
        if (sword.collides_with_box(stem)) {
            state = female ? State::FALLING_CLOSED_PLAYER1 : State::FALLING_CLOSED_PLAYER2;
            velocity = glm::vec2(0.1f * player_velocity.x, 0);
        }
        break;
    case State::FALLING_CLOSED_PLAYER1:
    case State::FALLING_OPEN_PLAYER1:
        if (!female) {
            if (sword.collides_with_box(fruit)) {
                state = State::FALLING_OPEN_PLAYER2;
                velocity = glm::vec2(0.2f * player_velocity.x, 1.1f * std::abs(velocity.y));
            }
        }
        break;
    case State::FALLING_CLOSED_PLAYER2:
    case State::FALLING_OPEN_PLAYER2:
        if (female) {
            if (sword.collides_with_box(fruit)) {
                state = State::FALLING_OPEN_PLAYER1;
                velocity = glm::vec2(0.2f * player_velocity.x, 1.1f * std::abs(velocity.y));
            }
        }
        break;
    }
}

}
