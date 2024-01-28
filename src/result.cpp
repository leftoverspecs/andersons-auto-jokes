#include "result.h"

#include <font.h>

#include <glm/gtc/matrix_transform.hpp>

namespace game {

Result::Result(int height, SDL_Window *window,
               game::Background &background, engine::Font &font)
    : Scene(height, window),
      background{background},
      font(font) {}

void Result::startup(Fight::Winner new_winner, int new_losses) {
    winner = new_winner;
    losses = new_losses;
    alpha = 0.0f;
    state = State::FADE_IN;
}
void Result::on_loop(float delta_time) {
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);
    font.clear();
    if (state == State::FADE_IN) {
        alpha += delta_time * 0.001f;
        if (alpha > 0.9f) {
            alpha = 1.0f;
            state = State::PRESENTING;
        }
    } else if (state == State::FADE_OUT) {
        alpha -= delta_time * 0.001f;
        if (alpha < 0.1f) {
            alpha = 0.0f;
            exit();
        }
    }
    std::string message;
    glm::mat4 model(1.0f);
    switch (winner) {
    case Fight::Winner::UNDECIDED:
    case Fight::Winner::NOBODY:
        model = glm::translate(model, glm::vec3(250.0f, 300.0f, 0.0f));
        message = "Draw";
        break;
    case Fight::Winner::TEAM1:
        model = glm::translate(model, glm::vec3(275.0f, 300.0f, 0.0f));
        message = "Win!";
        break;
    case Fight::Winner::TEAM2:
        model = glm::translate(model, glm::vec3(200.0f, 300.0f, 0.0f));
        message = "Loose!";
        break;
    }
    model = glm::scale(model, glm::vec3(90.0f, 120.0f, 1.0f));
    font.write(model, glm::vec4(1.0f, 1.0f, 1.0f, alpha), message.c_str());

    model = glm::mat4(1.0f);
    if (losses == 1) {
        model = glm::translate(model, glm::vec3(200.0f, 200.0f, 0.0f));
        model = glm::scale(model, glm::vec3(40.0f, 50.0f, 1.0f));
        message = "You lost once.";
    } else {
        model = glm::translate(model, glm::vec3(150.0f, 200.0f, 0.0f));
        model = glm::scale(model, glm::vec3(40.0f, 50.0f, 1.0f));
        message = std::string("You lost ") + std::to_string(losses) + " times.";
    }
    font.write(model, glm::vec4(1.0f, 1.0f, 1.0f, alpha), message.c_str());
    background.draw(alpha);
    font.draw();
}

void Result::on_mouse_button_up(int x, int y) {
    state = State::FADE_OUT;
    alpha = 1.0f;
}

} // namespace game
