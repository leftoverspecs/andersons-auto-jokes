#include "music.h"

#include <LobbyMusic.mp3.h>
#include <FightMusic.mp3.h>

namespace game {

Music::Music(int volume)
    : lobby_music{LobbyMusic, sizeof(LobbyMusic)},
      fight_music{FightMusic, sizeof(FightMusic)}
{
    engine::Music::set_volume(volume);
}

void Music::play_lobby_music() {
    if (state == State::FIGHT) {
        fight_music.fade_out(1000);
        state = State::NOTHING;
    }
    if (state == State::NOTHING) {
        lobby_music.fade_in(-1, 1000);
    }
}

void Music::play_fight_music() {
    if (state == State::LOBBY) {
        lobby_music.fade_out(1000);
        state = State::NOTHING;
    }
    if (state == State::NOTHING) {
        fight_music.fade_in(-1, 1000);
    }
}

void Music::stop() const {
    if (state == State::LOBBY) {
        lobby_music.fade_out(1000);
    } else if (state == State::FIGHT) {
        fight_music.fade_out(1000);
    }
}

} // namespace game
