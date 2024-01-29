#pragma once

#include <chunk.h>
#include <vector>

namespace game {

class AudioData {
public:
    AudioData();

    void play_laugh(int index) const;
    void play_mumble(int index) const;
    void play_steps() const;
    void play_drag() const;
    void play_drop() const;

private:
    std::vector<engine::Chunk> snd_laugh;
    std::vector<engine::Chunk> snd_mumble;
    //engine::Chunk snd_steps;
    //engine::Chunk drag_chunk;
    //engine::Chunk drop_chunk;
};

}
