#include "stats.h"

#include <utility>

namespace common {

Stats::Stats() : sprite_row{0} {}

Stats::Stats(std::string description, int sprite_row,
             float capacity, float funny, float giddy,
             int laugh_index, int mumble_index)
    : description{std::move(description)},
      sprite_row{sprite_row},
      capacity{capacity},
      funny{funny},
      giddy{giddy},
      laugh_index{laugh_index},
      mumble_index{mumble_index}
{}

bool Stats::is_empty() const {
    return sprite_row == 0;
}

const std::string &Stats::get_description() const {
    return description;
}

int Stats::get_sprite_row() const {
    return sprite_row;
}

float Stats::get_capacity() const {
    return capacity;
}

float Stats::get_funny() const {
    return funny;
}

float Stats::get_giddy() const {
    return giddy;
}

int Stats::get_laugh_index() const {
    return laugh_index;
}

bool Stats::operator==(const Stats &other) const {
    return description == other.description;
}

bool Stats::operator!=(const Stats &other) const {
    return description != other.description;
}

void Stats::set_giddy(float new_giddy) {
    giddy = new_giddy;
}
int Stats::get_mumble_index() const {
    return mumble_index;
}

} // namespace common
