#include "fight.h"

#include <font.h>

#include <arena.png.h>

namespace game {

Fight::Fight(SDL_Window *window,
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
      textboxes(font, box_renderer),
      background(screen_width, screen_height, arena, sizeof(arena)) {}

void Fight::startup(const std::vector<Person::Stats> &team1_stats,
                    const std::vector<Person::Stats> &team2_stats) {
    team1.clear();
    team1.reserve(team1_stats.size());
    float x = 100.0f;
    for (const auto &s: team1_stats) {
        Person person{screen_height, family_renderer, font, box_renderer, textboxes, s};
        person.stand(x, 50.0f);
        x += 50.0f;
        team1.push_back(person);
    }
    x = 700.0f;
    team2.clear();
    team2.reserve(team2_stats.size());
    for (const auto &s: team2_stats) {
        Person person{screen_height, family_renderer, font, box_renderer, textboxes, s};
        person.stand(x, 50.0f);
        x -= 50.0f;
        team2.push_back(person);
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
        Person &person1 = team1[current_person1];
        Person &person2 = team2[current_person2];
        if (state == State::PREPARE) {
            person1.walk_to(250.0f, 100.0f);
            person2.walk_to(350.0f, 100.0f);
            state = State::PREPARING;
        } else if (state == State::PREPARING) {
            if (person1.arrived() && person2.arrived()) {
                state = State::BOTH_PREPARED;
            }
        } else if (state == State::BOTH_PREPARED) {
            timer = 2000.0f;
            state = State::STARING;
        } else if (state == State::STARING) {
            if (timer < 1.0f) {
                state = State::FIRST_TALKING;
                person1.talk();
                person2.hear();
                timer = 1000.0f;
            }
        } else if (state == State::FIRST_TALKING) {
            if (timer < 1.0f) {
                state = State::SECOND_TALKING;
                person1.hear();
                person2.talk();
                timer = 1000.0f;
            }
        } else if (state == State::SECOND_TALKING) {
            if (timer < 1.0f) {
                state = State::TENSION;
                timer = 1000.0f;
            }
        } else if (state == State::TENSION) {
            if (timer < 1.0f) {
                state = State::REACTION;
                person1.fighting(person2);
                person2.fighting(person1);
            }
        } else if (state == State::REACTION) {
            bool person1_fought = person1.fought();
            bool person2_fought = person2.fought();
            if (person1_fought) {
                if (person1.defeated()) {
                    person1.fly_away();
                    ++current_person1;
                }
            }
            if (person2_fought) {
                if (person2.defeated()) {
                    person2.fly_away();
                    ++current_person2;
                }
            }
            if (person1_fought && person2_fought) {
                if (current_person1 < team1.size() && current_person2 < team2.size()) {
                    state = State::PREPARE;
                } else {
                    state = State::FADE_OUT;
                }
            }
        }
    } else if (state == State::FADE_OUT) {
        alpha -= delta_time * 0.001f;
        if (alpha < 0.001f) {
            alpha = 0.0f;
            exit();
        }
    }
}

Fight::Winner Fight::finished() const {
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
        box_renderer.clear();
        update(delta_time);
        for (auto &person: team1) {
            person.update(delta_time);
            person.queue();
        }
        for (auto &person: team2) {
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

}
