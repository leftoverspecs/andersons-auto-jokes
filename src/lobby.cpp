#include "lobby.h"

#include <client.h>
#include <font.h>

#include <glm/gtc/matrix_transform.hpp>

namespace game {

Lobby::Lobby(int height, SDL_Window *window, engine::Font &font, common::Client &client)
    : Scene(height, window),
      font{font},
      client{client} {}

void Lobby::startup(const std::vector<const common::Stats *> &player_stats) {
    state = State::FADE_IN;
    alpha = 0.0f;
    stats.clear();
    for (const auto s : player_stats) {
        stats.push_back(*s);
    }
}

void Lobby::on_loop(float delta_time) {
    if (timer > 1.0f) {
        timer -= delta_time;
    } else {
        timer = 0.0f;
    }
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);
    font.clear();
    glm::mat4 model(1.0f);
    model = glm::translate(model, glm::vec3(10.0f, 300.0f, 0.0f));
    model = glm::scale(model, glm::vec3(40.0f, 40.0f, 1.0f));
    if (state == State::FADE_IN) {
        alpha += delta_time * 0.001f;
        if (alpha > 0.9f) {
            alpha = 1.0f;
            state = State::WAITING;
        }
    } else if (state == State::FADE_OUT) {
        alpha -= delta_time * 0.001f;
        if (alpha < 0.1f) {
            alpha = 0.0f;
            exit();
        }
    } else if (state == State::WAITING) {
        alpha = 1.0f;
        if (timer == 0.0f) {
            common::Message reply = client.send(common::Message{common::Message::type::GET_OPPONENT, stats});
            switch (reply.get_type()) {
            case common::Message::type::OPPONENT: {
                const std::vector<uint8_t>& content = reply.get_content();
                auto pos = content.begin();
                stats = common::read_stats(pos, content.end());
                state = State::FADE_OUT;
                break;
            }
            case common::Message::type::RETRY:
                timer = 100.0f;
                break;
            case common::Message::type::ERROR:
                throw std::runtime_error("Error from server");
            default:
                throw std::runtime_error("Unknown reply from server");
            }
        }
    }
    font.write(model, glm::vec4(1.0f, 1.0f, 1.0f, alpha), "Waiting for opponent...");
    font.draw();
}

}// namespace game