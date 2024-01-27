#ifndef GGJ2024_PERSON_H
#define GGJ2024_PERSON_H

#include <stats.h>

#include <string>
#include <vector>

namespace engine {

class BoxRenderer;
class SpriteRenderer;
class Font;
class TextBoxRenderer;

}

namespace game {

class Person {
public:
    Person(int screen_height,
           engine::SpriteRenderer &renderer,
           engine::Font &font,
           engine::BoxRenderer &box,
           engine::TextBoxRenderer &textboxes,
           const common::Stats *prototype);

    void stand(float x, float y, bool looks_right_);
    void stand_still();
    void update(float delta_time);
    void walk_to(float x, float y);
    void fly_away();
    void talk();
    void hear();
    void fighting(int own_position,
                  const Person &opponent,
                  int opponent_position,
                  std::vector<Person> &team,
                  std::vector<Person> &opponents);

    void queue(bool silent);

    [[nodiscard]]
    bool arrived() const;
    [[nodiscard]]
    bool fought() const;
    [[nodiscard]]
    bool defeated() const;

    void drop(const common::Stats *new_prototype);

    float get_current_x() const { return current_x; }
    float get_current_y() const { return current_y; }

    [[nodiscard]] const common::Stats *get_stats_prototype() const { return prototype; }
    [[nodiscard]] bool inside(int x, int y) const;
private:
    enum class State {
        STANDING,
        WALKING,
        TALKING,
        HEARING,
        FIGHTING,
        LAUGHING,
        SERIOUS,
    };

    int screen_height;
    engine::SpriteRenderer *renderer;
    engine::Font *font;
    engine::BoxRenderer *box;
    engine::TextBoxRenderer *textboxes;

    State state{State::STANDING};
    float time{0};
    float current_angle{0};
    float current_x{0};
    float current_y{0};
    bool looks_right{true};
    float destination_x{0};
    float destination_y{0};
    float current_capacity;
    float destination_capacity;

    const common::Stats *prototype;
    common::Stats current_stats;
};

}

#endif //GGJ2024_PERSON_H
