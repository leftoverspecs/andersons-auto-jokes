#include "fight.h"

#include <font.h>
#include "speech.h"

#include <arena.png.h>
#include <iostream>

namespace game {

Fight::Fight(SDL_Window *window,
             int screen_width, int screen_height,
             engine::Font &font,
             Speech &speech,
             game::AudioData &audio_data,
             engine::SpriteMap &family)
    : Scene(screen_height, window),
      screen_height{screen_height},
      font{font},
      speech{speech},
      audio_data{audio_data},
      family{family},
      destination(screen_width, screen_height),
      family_renderer(family, screen_width, screen_height),
      box_renderer(screen_width, screen_height),
      textboxes(font, box_renderer),
      background(screen_width, screen_height, arena, sizeof(arena)) {}

void Fight::startup(const std::vector<const common::Stats *> &team1_stats,
                    const std::vector<common::Stats> &team2_stats) {
    team1.clear();
    team1.reserve(team1_stats.size());
    float x = 20.0f;
    for (const auto s: team1_stats) {
        if (!s->is_empty()) {
            Person person{screen_height, family_renderer, font, box_renderer, textboxes, speech, audio_data, s};
            person.stand(x, 150.0f, true);
            person.set_battling(true);
            x += 70.0f;
            team1.push_back(person);
        }
    }
    x = 700.0f;
    team2.clear();
    team2.reserve(team2_stats.size());
    for (const auto &s: team2_stats) {
        if (!s.is_empty()) {
            Person person{screen_height, family_renderer, font, box_renderer, textboxes, speech, audio_data, &s};
            person.stand(x, 150.0f, false);
            person.set_battling(true);
            x -= 70.0f;
            team2.push_back(person);
        }
    }
    state = State::FADE_IN;
    current_person1 = 0;
    current_person2 = 0;
    alpha = 0.0f;
}

void Fight::update(float delta_time) {
    if (timer > 1.0f) {
        timer -= delta_time;
    } else {
        timer = 0.0f;
    }
    if (state == State::FADE_IN) {
        alpha += delta_time * 0.001f;
        if (alpha > 0.9f) {
            alpha = 1.0f;
            state = State::PREPARE;
        }
    } else if (state != State::FADE_OUT) {
        Person &person1 = team1[team1.size() - current_person1 - 1];
        Person &person2 = team2[team2.size() - current_person2 - 1];
        if (state == State::PREPARE) {
            person1.walk_to(250.0f, 250.0f);
            person2.walk_to(450.0f, 250.0f);
            state = State::PREPARING;
        } else if (state == State::PREPARING) {
            if (person1.arrived()) {
                person1.stand_still();
            }
            if (person2.arrived()) {
                person2.stand_still();
            }
            if (person1.arrived() && person2.arrived()) {
                state = State::BOTH_PREPARED;
            }
        } else if (state == State::BOTH_PREPARED) {
            timer = 100.0f;
            state = State::STARING;
        } else if (state == State::STARING) {
            if (timer < 1.0f) {
                state = State::FIRST_TALKING;
                person1.talk();
                person2.hear();
                timer = 2000.0f;
            }
        } else if (state == State::FIRST_TALKING) {
            if (timer < 1.0f) {
                state = State::SECOND_TALKING;
                person1.hear();
                person2.talk();
                timer = 2000.0f;
            }
        } else if (state == State::SECOND_TALKING) {
            if (timer < 1.0f) {
                state = State::TENSION;
                person1.fighting(current_person1, person2, current_person2, team1, team2);
                person2.fighting(current_person2, person1, current_person1, team2, team1);
            }
        } else if (state == State::TENSION) {
            if (timer < 1.0f) {
                if (!person1.defeated() && !person2.defeated()) {
                    person1.fighting(current_person1, person2, current_person2, team1, team2);
                    person2.fighting(current_person2, person1, current_person1, team2, team1);
                    timer = 200.0f;
                } else {
                    state = State::REACTION;
                }
            }
        } else if (state == State::REACTION) {
            if (person1.defeated()) {
                person1.fly_away();
                current_person1 = -1;
                for (int i = 0; i < team1.size(); ++i) {
                    if (!team1[team1.size() - i - 1].defeated()) {
                        current_person1 = i;
                        break;
                    }
                }
            }
            if (person2.defeated()) {
                person2.fly_away();
                current_person2 = -1;
                for (int i = 0; i < team2.size(); ++i) {
                    if (!team2[team2.size() - i - 1].defeated()) {
                        current_person2 = i;
                        break;
                    }
                }
            }
            std::cout << "current_person1 = " << current_person1 << ", current_person2 = " << current_person2 << '\n';
            if (current_person1 != -1 && current_person2 != -1) {
                state = State::PREPARE;
            } else {
                state = State::FADE_OUT;
            }
        }
    } else if (state == State::FADE_OUT) {
        alpha -= delta_time * 0.0002f;
        if (alpha < 0.001f) {
            alpha = 0.0f;
            exit();
        }
    }
}

Fight::Winner Fight::get_result() const {
    if (state != State::FADE_OUT) {
        return Winner::UNDECIDED;
    } else {
        bool team1_lost = current_person1 >= team1.size();
        bool team2_lost = current_person2 >= team2.size();
        if (team1_lost && team2_lost) {
            return Winner::NOBODY;
        } else if (team1_lost) {
            return Winner::TEAM2;
        } else if (team2_lost) {
            return Winner::TEAM1;
        } else {
            return Winner::UNDECIDED;
        }
    }
}

void Fight::on_loop(float delta_time) {
    {
        auto binding = destination.bind_as_target();
        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        family_renderer.clear();
        font.clear();
        speech.clear();
        box_renderer.clear();
        update(delta_time);
        for (auto &person: team1) {
            person.update(delta_time);
            person.queue(true);
        }
        for (auto &person: team2) {
            person.update(delta_time);
            person.queue(true);
        }
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

}
