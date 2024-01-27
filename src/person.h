#ifndef GGJ2024_PERSON_H
#define GGJ2024_PERSON_H

namespace engine {

class BoxRenderer;
class SpriteRenderer;
class Font;

}

class Person {
public:
    struct Stats {
        int sprite_row;
        float capacity;
        float funny;
        float giddy;
    };

    Person(engine::SpriteRenderer &renderer,
           engine::Font &font,
           engine::BoxRenderer &box,
           const Stats &stats);

    void stand(float x, float y);
    void update(float delta_time);
    void walk_to(float x, float y);
    void fly_away();
    void talk();
    void hear();
    void fighting(const Person &other);

    void queue();

    [[nodiscard]] bool arrived() const;
    [[nodiscard]] bool fought() const;
    [[nodiscard]] bool defeated() const;
    [[nodiscard]] const Stats &get_stats() const { return stats; }
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

    engine::SpriteRenderer *renderer;
    engine::Font *font;
    engine::BoxRenderer *box;

    State state{State::STANDING};
    float time{0};
    float current_angle{0};
    float current_x{0};
    float current_y{0};
    float destination_x{0};
    float destination_y{0};
    float current_capacity;
    float destination_capacity;

    Stats stats;
};

#endif //GGJ2024_PERSON_H
