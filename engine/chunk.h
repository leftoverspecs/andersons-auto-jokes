#pragma once

#include <SDL_mixer.h>

#include <cstddef>

namespace engine {

class Chunk {
public:
    Chunk(const unsigned char *data, std::size_t size);
    ~Chunk();

    Chunk(const Chunk &) = delete;
    Chunk &operator = (const Chunk &) = delete;

    Chunk(Chunk &&) noexcept;
    Chunk &operator = (Chunk &&) noexcept;

    void play(int loops) const;
    void set_volume(int volume) const;
private:
    Mix_Chunk *chunk;
};

}
