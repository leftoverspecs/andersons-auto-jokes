#include "stats.h"

#include <utility>

namespace common {

Stats::Stats() : sprite_row{0} {}

Stats::Stats(std::string description, int sprite_row, float capacity, float funny, float giddy)
    : description{std::move(description)},
      sprite_row{sprite_row},
      capacity{capacity},
      funny{funny},
      giddy{giddy}
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

} // namespace common
