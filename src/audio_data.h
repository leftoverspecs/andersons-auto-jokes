#pragma once

#include <chunk.h>
#include <vector>

namespace game {

class AudioData {
public:
    AudioData();

    void play_laugh(int index) const;
    void play_steps() const;

private:
    std::vector<engine::Chunk> snd_laugh;
    engine::Chunk snd_steps;
};

}
