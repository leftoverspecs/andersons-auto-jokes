#include "audio_data.h"

#include <snd_laugh_child1.wav.h>
#include <snd_laugh_female1.wav.h>
#include <snd_laugh_female2.wav.h>
#include <snd_laugh_male1.wav.h>
#include <snd_laugh_male2.wav.h>
#include <snd_laugh_male3.wav.h>
#include <snd_laugh_male4.wav.h>
#include <snd_laugh_male5.wav.h>
#include <snd_steps2.wav.h>

namespace game {

namespace {

std::vector<engine::Chunk> init_snd_laugh() {
    std::vector<engine::Chunk> result;
    result.emplace_back(snd_laugh_male1, sizeof(snd_laugh_male1));
    result.emplace_back(snd_laugh_male2, sizeof(snd_laugh_male2));
    result.emplace_back(snd_laugh_male3, sizeof(snd_laugh_male3));
    result.emplace_back(snd_laugh_male4, sizeof(snd_laugh_male3));
    result.emplace_back(snd_laugh_male5, sizeof(snd_laugh_male3));
    result.emplace_back(snd_laugh_child1, sizeof(snd_laugh_child1));
    result.emplace_back(snd_laugh_female1, sizeof(snd_laugh_female1));
    result.emplace_back(snd_laugh_female2, sizeof(snd_laugh_female2));
    return result;
}

}

const std::vector<engine::Chunk> chunk_snd_laugh = init_snd_laugh();

const engine::Chunk chunk_snd_steps(snd_steps2, sizeof(snd_steps2));

}
