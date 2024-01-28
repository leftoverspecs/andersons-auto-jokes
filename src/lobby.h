#ifndef ANDERSONS_AUTO_JOKES_LOBBY_H
#define ANDERSONS_AUTO_JOKES_LOBBY_H

#include "scene.h"

#include <stats.h>

#include <vector>

namespace engine {

class Font;

}

namespace common {

class Client;

}

namespace game {

class Lobby : public Scene {
public:
    Lobby(int height, SDL_Window *window, engine::Font &font, common::Client &client);

    void startup(const std::vector<const common::Stats *> &player_stats);
    [[nodiscard]] const std::vector<common::Stats> &get_opponent_stats() const { return stats; }
private:
    enum class State {
        FADE_IN,
        WAITING,
        FADE_OUT,
        FINISHED,
    };
    engine::Font &font;
    common::Client &client;
    State state{State::FADE_IN};
    float alpha{0.0f};
    float timer{0.0f};
    std::vector<common::Stats> stats;

    void on_loop(float delta_time) override;
};

} // namespace game

#endif //ANDERSONS_AUTO_JOKES_LOBBY_H
