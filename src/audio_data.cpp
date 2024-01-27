#include "audio_data.h"
#include <snd_laugh_child1.wav.h>
#include <snd_laugh_female1.wav.h>
#include <snd_laugh_female2.wav.h>
#include <snd_laugh_male1.wav.h>
#include <snd_laugh_male2.wav.h>
#include <snd_laugh_male3.wav.h>
#include <snd_steps.wav.h>
#include <snd_steps2.wav.h>

namespace game {
    const engine::Chunk chunk_snd_laugh_child1(snd_laugh_child1, sizeof(snd_laugh_child1));
    const engine::Chunk chunk_snd_laugh_female1(snd_laugh_female1, sizeof(snd_laugh_female1));
    const engine::Chunk chunk_snd_laugh_female2(snd_laugh_female2, sizeof(snd_laugh_female2));
    const engine::Chunk chunk_snd_laugh_male1(snd_laugh_male1, sizeof(snd_laugh_male1));
    const engine::Chunk chunk_snd_laugh_male2(snd_laugh_male2, sizeof(snd_laugh_male2));
    const engine::Chunk chunk_snd_laugh_male3(snd_laugh_male3, sizeof(snd_laugh_male3));
    const engine::Chunk chunk_snd_steps(snd_steps2, sizeof(snd_steps2));
}
