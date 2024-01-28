#ifndef ANDERSONS_AUTO_JOKES_STATS_H
#define ANDERSONS_AUTO_JOKES_STATS_H

#include "primitives.h"

#include <cstdint>
#include <string>
#include <vector>

namespace common {

class Stats {
public:
    Stats();
    Stats(std::string description, int sprite_row,
          float capacity, float funny, float giddy,
          int laugh_index);

    template<typename It>
    Stats(It &pos, It end);

    template<typename It>
    void write(It &pos) const;

    [[nodiscard]]
    bool is_empty() const;

    [[nodiscard]]
    const std::string &get_description() const;

    [[nodiscard]]
    int get_sprite_row() const;

    [[nodiscard]]
    float get_capacity() const;

    [[nodiscard]]
    float get_funny() const;

    [[nodiscard]]
    float get_giddy() const;

    [[nodiscard]]
    int get_laugh_index() const;

private:
    std::string description;
    int sprite_row;
    float capacity{};
    float funny{};
    float giddy{};
    int laugh_index{};
};

template<typename It>
Stats::Stats(It &pos, It end)
    : description{read_string(pos, end)},
      sprite_row{read_uint8(pos, end)},
      capacity{read_float(pos, end)},
      funny{read_float(pos, end)},
      giddy{read_float(pos, end)},
      laugh_index{read_uint8(pos, end)}
{}

template<typename It>
std::vector<Stats> read_stats(It &pos, It end) {
    uint32_t size = read_uint32(pos, end);
    std::vector<Stats> result;
    result.reserve(size);
    for (uint32_t i = 0; i < size; ++i) {
        result.emplace_back(pos, end);
    }
    return result;
}

template<typename It>
void Stats::write(It &pos) const {
    write_string(pos, description);
    write_uint8(pos, sprite_row);
    write_float(pos, capacity);
    write_float(pos, funny);
    write_float(pos, giddy);
    write_uint8(pos, laugh_index);
}

template<typename It>
void write_stats(It &pos, const std::vector<Stats> &stats) {
    write_uint32(pos, stats.size());
    for (const auto &stat : stats) {
        stat.write(pos);
    }
}


} // namespace common

#endif//ANDERSONS_AUTO_JOKES_STATS_H
