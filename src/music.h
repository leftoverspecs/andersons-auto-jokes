#ifndef ANDERSONS_AUTO_JOKES_MUSIC_H
#define ANDERSONS_AUTO_JOKES_MUSIC_H

#include <music.h>

namespace game {

class Music {
public:
    explicit Music(int volume);

    void play_lobby_music();
    void play_fight_music();
    void stop() const;
private:
    enum class State {
        NOTHING,
        LOBBY,
        FIGHT,
    };
    State state{State::NOTHING};
    engine::Music lobby_music;
    engine::Music fight_music;
};

} // namespace game

#endif//ANDERSONS_AUTO_JOKES_MUSIC_H
