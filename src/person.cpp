#include "person.h"

#include "audio_data.h"
#include "speech.h"
#include "family.h"

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
               game::Speech &speech,
               game::AudioData &audio_data,
               const common::Stats *prototype)
    : screen_height{screen_height},
      renderer{&renderer}, font{&font}, box{&box}, textboxes{&textboxes},
      speech(&speech),
      audio_data{&audio_data},
      state{State::STANDING},
      current_capacity{prototype->get_capacity()}, destination_capacity{prototype->get_capacity()},
      prototype{prototype}, current_stats{*prototype} {}

void Person::update(float delta_time) {
    time += delta_time;
    const float speed = (state == State::LAUGHING ? 10.0f : 1.0f) * SPEED;
    if (std::fabs(current_x - destination_x) < 10.0f) {
        current_x = destination_x;
    } else if (current_x < destination_x) {
        current_x += speed * delta_time;
    } else if (current_x > destination_x) {
        current_x -= speed * delta_time;
    }
    if (std::fabs(current_y - destination_y) < 10.0f) {
        current_y = destination_y;
    } else if (current_y < destination_y) {
        current_y += speed * delta_time;
    } else if (current_y > destination_y) {
        current_y -= speed * delta_time;
    }
    if (battling) {
        if (current_capacity > destination_capacity) {
            current_capacity -= CAPACITY_CHANGE * delta_time;
        }
        if (current_capacity <= destination_capacity) {
            current_capacity = destination_capacity;
            if (current_capacity <= 0.0f) {
                fly_away();
                //state = State::LAUGHING;
            }
        }
    }
    if (!current_stats.is_empty() && state == State::LAUGHING) {
        current_angle += 0.1f;
    }
}

void Person::stand(float x, float y, bool looks_right_) {
    state = State::STANDING;
    current_x = destination_x = x;
    current_y = destination_y = y;
    this->looks_right = looks_right_;
}

void Person::walk_to(float x, float y) {
    audio_data->play_steps();
    state = State::WALKING;
    destination_x = x;
    destination_y = y;
}

namespace {

float clamp(float value, float min, float max) {
    return std::min(std::max(value, min), max);
}

}

void Person::queue(bool silent) {
    int x;
    int y;
    SDL_GetMouseState(&x, &y);
    y = screen_height - y;
    const bool inside = this->inside(x, y);
    const float inside_extra = inside ? 1.0f : 0.0f;

    int current_sprite_column = 0;
    if (state == State::LAUGHING) {
        current_sprite_column = 4;
    } else if (state == State::WALKING) {
        current_sprite_column = static_cast<int>(time / 500.0f) % 2 + 1;
    } else if (state == State::STANDING) {
        current_sprite_column = 3 * (static_cast<int>(time / 500.0f) % 2);
    } else if (state == State::TALKING) {
        current_sprite_column = 5 * (static_cast<int>(time / 500.0f) % 2);
    } else if (state == State::HEARING) {
        current_sprite_column = 0;
    }
    glm::mat4 model{1.0f};
    if (looks_right) {
        model = glm::translate(model, glm::vec3(current_x, current_y, 0.0f));
        model = glm::rotate(model, current_angle, glm::vec3(0.0f, 0.0f, 1.0f));
        model = glm::scale(model, glm::vec3(128.0f, 256.0f, 1.0f));
    } else {
        model = glm::translate(model, glm::vec3(current_x + 128.0f, current_y, 0.0f));
        model = glm::rotate(model, current_angle, glm::vec3(0.0f, 0.0f, 1.0f));
        model = glm::scale(model, glm::vec3(-128.0f, 256.0f, 1.0f));
    }
    renderer->queue(model, glm::vec4(1.0f + inside_extra, 1.0f + inside_extra, 1.0f + inside_extra, 1.0f), current_sprite_column, current_stats.get_sprite_row());

    if (state == State::TALKING) {
        const int current_speech = static_cast<int>(time / 500.0f) % 4;
        speech->queue(current_x + (looks_right ? -256.0f : +352.0f), current_y + 180.0f, !looks_right, current_speech);
    }

    if (!current_stats.is_empty() && state != State::LAUGHING) {
        model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(current_x + 30.0f, current_y - 20.0f, 0.0f));
        model = glm::scale(model, glm::vec3(40.0f, 40.0f, 1.0f));
        font->write(model, glm::vec4(1.0f, 1.0f, 1.0f, 1.0f), std::to_string(static_cast<int>(current_stats.get_giddy())).c_str());

        model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(current_x + 90.0f, current_y - 20.0f, 0.0f));
        model = glm::scale(model, glm::vec3(40.0f, 40.0f, 1.0f));
        font->write(model, glm::vec4(1.0f, 1.0f, 1.0f, 1.0f), std::to_string(static_cast<int>(current_stats.get_funny())).c_str());

        model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(current_x + 10.0, current_y - 25.0f, 0.0f));
        float relative_capacity = clamp(current_capacity / prototype->get_capacity(), 0.0f, 1.0f);
        model = glm::scale(model, glm::vec3(118.0f * relative_capacity, 10.0f, 1.0f));
        box->queue_frame(model, glm::vec4(1.0f - relative_capacity, relative_capacity, 0.0f, 1.0f));

        if (inside && !silent) {
            textboxes->queue(current_x + 50.0f, current_y + 350.0f, 300.0f, 40.0f, 2.0f, 3.0f, current_stats.get_description(), glm::vec4(1.0f, 1.0f, 1.0f, 1.0f), glm::vec4(1.0f, 1.0f, 0.0f, 1.0f));
        }
    }
}

bool Person::arrived() const {
    return current_x == destination_x && current_y == destination_y;
}

void Person::fly_away() {
    if (audio_data != nullptr) {
        //audio_data->play_laugh(current_stats.get_laugh_index());
    }
    state = State::LAUGHING;
    destination_y = 1000.0f;
}

void Person::talk() {
    state = State::TALKING;
}

void Person::fighting(int own_position,
                      Person &opponent,
                      int opponent_position,
                      std::vector<Person> &team,
                      std::vector<Person> &opponents) {
    state = State::FIGHTING;
    const bool dad_sister_brother_nerf = (*this->prototype == SISTER || *this->prototype == BROTHER)
            && *opponent.prototype == DAD;
    destination_capacity -= (dad_sister_brother_nerf ? 0.5f : 1.0f) * opponent.current_stats.get_funny() * current_stats.get_giddy();
    if (*this->prototype == GRANDPA) {
        for (auto &p : team) {
            if (&p != this) {
                p.destination_capacity -= 0.5f * current_stats.get_funny() * p.current_stats.get_giddy();
            }
        }
    } else if (*this->prototype == CHILD) {
        for (auto &p : opponents) {
            if (*p.prototype != SISTER && *p.prototype != BROTHER) {
                p.destination_capacity -= 2.0f * current_stats.get_funny() * p.current_stats.get_giddy();
            }
        }
    } else if (*this->prototype == MUM) {
        for (auto &p : team) {
            if (&p != this) {
                p.current_stats.set_giddy(std::max(p.current_stats.get_giddy() - 0.5f, 0.5f));
            }
        }
    } else if (*this->prototype == DAD) {
        if (*opponent.prototype == DAD) {
            destination_capacity = 0.0f;
            opponent.destination_capacity = 0.0f;
        }
        for (auto &p : opponents) {
            if (&p != &opponent && *p.prototype == DAD) {
                p.destination_capacity -= 0.1f * current_stats.get_funny() * p.current_stats.get_giddy();
            }
        }
    } else if (*this->prototype == UNCLE) {
        if (*opponent.prototype == SISTER || *opponent.prototype == BROTHER) {
            opponent.destination_capacity -= 0.25f * current_stats.get_funny() * opponent.current_stats.get_giddy();
        }
    }
    if (own_position < team.size() - 1 && *team[team.size() - own_position - 2].prototype == GRANDMA && !team[team.size() - own_position - 2].defeated()) {
        destination_capacity += 1.0;
    }
}

bool Person::fought() const {
    return destination_capacity == current_capacity;
}

bool Person::defeated() const {
    return destination_capacity <= 0.0f;
}

void Person::hear() {
    state = State::HEARING;
}

bool Person::inside(int x, int y) const {
    return x >= current_x && x <= current_x + 128.0f && y >= current_y && y <= current_y + 256.0f;
}

void Person::drop(const common::Stats *new_prototype) {
    current_stats = *new_prototype;
    prototype = new_prototype;
    current_capacity = prototype->get_capacity();
    destination_capacity = prototype->get_capacity();
    state = State::STANDING;
}
void Person::stand_still() {
    state = State::STANDING;
}
void Person::set_battling(bool value) {
    battling = value;
}

}