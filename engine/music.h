#pragma once

#include <SDL_mixer.h>

#include <cstddef>

namespace engine {

class Music {
public:
    Music(const unsigned char *data, std::size_t size);
    ~Music();

    Music(const Music &) = delete;
    Music &operator = (const Music &) = delete;

    void fade_in(int loops, int ms) const;
    void fade_out(int ms) const;

    static void set_volume(int volume);
private:
    Mix_Music *music;
};

}
