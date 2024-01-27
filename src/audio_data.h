#pragma once

#include <chunk.h>
#include <vector>

namespace game {

class AudioData {
public:
    AudioData();

    [[nodiscard]] const engine::Chunk &geg_snd_laugh(int index) const { return snd_laugh.at(index); }
    [[nodiscard]] const engine::Chunk &get_steps() const { return snd_steps; }

private:
    std::vector<engine::Chunk> snd_laugh;
    engine::Chunk snd_steps;
};

}
