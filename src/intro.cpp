#include "intro.h"

#include <font.h>
#include "family.h"
#include "speech.h"
#include "audio_data.h"

#include <arena.png.h>

#include <glm/gtc/matrix_transform.hpp>

namespace game {

Intro::Intro(SDL_Window *window,
             int screen_width, int screen_height,
             engine::Font &font,
             game::Speech &speech,
             game::AudioData &audio_data,
             engine::SpriteMap &family)
    : Scene(screen_height, window),
      screen_height{screen_height},
      font{font},
      speech{speech},
      destination(screen_width, screen_height),
      family_renderer(family, screen_width, screen_height),
      box_renderer(screen_width, screen_height),
      textboxes(font, box_renderer),
      background(screen_width, screen_height, arena, sizeof(arena)),
      father(screen_height, family_renderer, font, box_renderer, textboxes, speech, audio_data, &game::DAD),
      grandfather(screen_height, family_renderer, font, box_renderer, textboxes, speech, audio_data, &game::GRANDPA)
{
    father.stand(575, 200, false);
    grandfather.stand(625, 150, false);
}

void Intro::update(float delta_time) {
    if (state == State::FADE_IN) {
        alpha += delta_time * 0.001f;
        if (alpha > 0.9f) {
            alpha = 1.0;
            state = State::SELECTING;
        }
    } else if (state == State::FADE_OUT) {
        alpha -= delta_time * 0.001f;
        if (alpha < 0.1f) {
            alpha = 0.0f;
            exit();
        }
    }
}

void Intro::on_loop(float delta_time) {
    {
        auto binding = destination.bind_as_target();
        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        family_renderer.clear();
        font.clear();
        box_renderer.clear();
        update(delta_time);

        father.update(delta_time);
        grandfather.update(delta_time);
        father.queue(true);
        grandfather.queue(true);

        glm::mat4 model(1.0f);
        model = glm::translate(model, glm::vec3(150.0f, 400.0f, 0.0f));
        model = glm::scale(model, glm::vec3(35.0f, 50.0f, 1.0f));
        font.write(model, glm::vec4(1.0f, 1.0f, 1.0f, 1.0f), "Anderson's");

        model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(40.0f, 320.0f, 0.0f));
        model = glm::scale(model, glm::vec3(67.0f, 80.0f, 1.0f));
        font.write(model, glm::vec4(1.0f, 1.0f, 1.0f, 1.0f), "Auto Jokes");

        int x;
        int y;
        SDL_GetMouseState(&x, &y);
        y = screen_height - y;
        const glm::vec4 normal = glm::vec4(0.5f, 0.5f, 0.5f, 1.0f);
        const glm::vec4 highlight = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);
        bool inside = engine::TextBoxRenderer::is_inside(40.0f, 285.0f, 400.0f, 25.0f, x, y);
        textboxes.queue(40.0f, 285.0f, 400.0f, 25.0f, 2.5f, 3.0f, "Play with own server", highlight, inside ? highlight : normal);

        inside = engine::TextBoxRenderer::is_inside(40.0f, 225.0f, 400.0f, 25.0f, x, y);
        textboxes.queue(40.0f, 225.0f, 400.0f, 25.0f, 2.5f, 3.0f, "Play on other server", highlight, inside ? highlight : normal);

        inside = engine::TextBoxRenderer::is_inside(40.0f, 80.0f, 720.0f, 25.0f, x, y);
        textboxes.queue(40.0f, 80.0f, 720.0f, 25.0f, 2.5f, 3.0f, "Server IP: " + hostname + (input ? "\x7f" : ""), highlight, inside ? highlight : normal);

        background.draw(1.0f);
        family_renderer.draw();
        box_renderer.draw();
        font.draw();
        speech.draw();
    }
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);
    destination.draw(glm::mat4(1.0f), glm::vec3(0.0f), alpha, glm::vec3(1.0f));
}

void Intro::on_key_pressed(SDL_Keycode code) {
    if (input) {
        if (code == SDLK_BACKSPACE && !hostname.empty()) {
            hostname.pop_back();
        } else if ((code >= 0x30 && code <= 0x39) ||
                   code == SDLK_PERIOD ||
                   (code >= 0x41 && code <= 0x5a) ||
                   (code >= 0x61 && code <= 0x7a)) {
            hostname.push_back(static_cast<char>(code));
        } else if (code == SDLK_RETURN) {
            input = false;
        }
    }
}

void Intro::on_mouse_button_up(int x, int y) {
    if (engine::TextBoxRenderer::is_inside(40.0f, 285.0f, 400.0f, 25.0f, x, y)) {
        own_server = true;
        exit();
    } else if (engine::TextBoxRenderer::is_inside(40.0f, 225.0f, 400.0f, 25.0f, x, y)) {
        own_server = false;
        exit();
    } else if (!input) {
        if (engine::TextBoxRenderer::is_inside(40.0f, 80.0f, 720.0f, 25.0f, x, y)) {
            input = true;
        }
    }
}

}
