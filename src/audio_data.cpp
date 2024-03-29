#include "audio_data.h"

#include <snd_laugh_child1.mp3.h>
#include <snd_laugh_female1.mp3.h>
#include <snd_laugh_female2.mp3.h>
#include <snd_laugh_female3.mp3.h>
#include <snd_laugh_male1.mp3.h>
#include <snd_laugh_male2.mp3.h>
#include <snd_laugh_male3.mp3.h>
#include <snd_laugh_male4.mp3.h>
#include <snd_laugh_male5.mp3.h>
#include <snd_mumble_child1.mp3.h>
#include <snd_mumble_female1.mp3.h>
#include <snd_mumble_female2.mp3.h>
#include <snd_mumble_male1.mp3.h>
#include <snd_mumble_male2.mp3.h>
#include <snd_mumble_male3.mp3.h>
#include <snd_mumble_male4.mp3.h>
#include <snd_mumble_male5.mp3.h>
#include <snd_steps2.mp3.h>
#include <snd_drag.mp3.h>
#include <snd_drop.mp3.h>


namespace game {

AudioData::AudioData(bool enabled)
    : enabled{enabled},
      snd_steps(snd_steps2, sizeof(snd_steps2)),
      drag_chunk(snd_drag, sizeof(snd_drag)),
      drop_chunk(snd_drop, sizeof(snd_drop))
{
    snd_laugh.emplace_back(snd_laugh_child1, sizeof(snd_laugh_child1));
    snd_laugh.emplace_back(snd_laugh_female1, sizeof(snd_laugh_female1));
    snd_laugh.emplace_back(snd_laugh_female2, sizeof(snd_laugh_female2));
    snd_laugh.emplace_back(snd_laugh_female3, sizeof(snd_laugh_female3));
    snd_laugh.emplace_back(snd_laugh_male1, sizeof(snd_laugh_male1));
    snd_laugh.emplace_back(snd_laugh_male2, sizeof(snd_laugh_male2));
    snd_laugh.emplace_back(snd_laugh_male3, sizeof(snd_laugh_male3));
    snd_laugh.emplace_back(snd_laugh_male4, sizeof(snd_laugh_male4));
    snd_laugh.emplace_back(snd_laugh_male5, sizeof(snd_laugh_male5));

    snd_mumble.emplace_back(snd_mumble_male1, sizeof(snd_mumble_male1));
    snd_mumble.emplace_back(snd_mumble_male2, sizeof(snd_mumble_male2));
    snd_mumble.emplace_back(snd_mumble_male3, sizeof(snd_mumble_male3));
    snd_mumble.emplace_back(snd_mumble_male4, sizeof(snd_mumble_male4));
    snd_mumble.emplace_back(snd_mumble_male5, sizeof(snd_mumble_male5));
    snd_mumble.emplace_back(snd_mumble_child1, sizeof(snd_mumble_child1));
    snd_mumble.emplace_back(snd_mumble_female1, sizeof(snd_mumble_female1));
    snd_mumble.emplace_back(snd_mumble_female2, sizeof(snd_mumble_female2));
}

void AudioData::play_laugh(int index) const {
    if (enabled) {
        snd_laugh.at(index).play(0);
    }
}

void AudioData::play_mumble(int index) const {
    if (enabled) {
        snd_mumble.at(index).play(0);
    }
}

void AudioData::play_steps() const {
    if (enabled) {
        snd_steps.play(0);
    }
}

void AudioData::play_drag() const {
    if (enabled) {
        drag_chunk.play(0);
    }
}

void AudioData::play_drop() const {
    if (enabled) {
        drop_chunk.play(0);
    }
}

}
