#ifndef ANDERSONS_AUTO_JOKES_PRIMITIVES_H
#define ANDERSONS_AUTO_JOKES_PRIMITIVES_H

#include <cstdint>
#include <exception>
#include <iterator>
#include <span>
#include <vector>

namespace common {

template<typename It>
uint8_t read_uint8(It &pos, It end) {
    if (std::distance(pos, end) >= 1) {
        uint8_t result = *pos++;
        return result;
    } else {
        throw std::runtime_error("Not enough data in buffer: can't read uint8_t");
    }
}

template<typename It>
uint32_t read_uint32(It &pos, It end) {
    if (std::distance(pos, end) >= sizeof(uint32_t)) {
        const uint32_t b1 = read_uint8(pos, end) << 24;
        const uint32_t b2 = read_uint8(pos, end) << 16;
        const uint32_t b3 = read_uint8(pos, end) << 8;
        const uint32_t b4 = read_uint8(pos, end);
        return b1 | b2 | b3 | b4;
    } else {
        throw std::runtime_error("Not enough data in buffer: can't read uint32_t");
    }
}

template<typename It>
float read_float(It &pos, It end) {
    if (std::distance(pos, end) >= sizeof(float)) {
        uint8_t bytes[sizeof(float)];
        for (unsigned char & byte : bytes) {
            byte = read_uint8(pos, end);
        }
        return *reinterpret_cast<float *>(&bytes[0]);
    } else {
        throw std::runtime_error("Not enough data in buffer: can't read uint32_t");
    }
}

template<typename It>
std::vector<uint8_t> read_uint8_vector(It &pos, It end) {
    const uint32_t size = read_uint32(pos, end);
    if (std::distance(pos, end) >= size) {
        std::vector<uint8_t> result;
        result.reserve(size);
        for (uint32_t i = 0; i < size; ++i) {
            result.push_back(read_uint8(pos, end));
        }
        return result;
    } else {
        throw std::runtime_error("Not enough data in buffer: can't read uint8 vector");
    }
}

template<typename It>
std::string read_string(It &pos, It end) {
    std::vector<uint8_t> result = read_uint8_vector(pos, end);
    return {result.begin(), result.end()};
}

template<typename It>
void write_uint8(It &pos, uint8_t value) {
    *pos++ = value;
}

template<typename It>
void write_uint32(It &pos, uint32_t value) {
    write_uint8(pos, (value & 0xf000) >> 24);
    write_uint8(pos, (value & 0x0f00) >> 16);
    write_uint8(pos, (value & 0x00f0) >> 8);
    write_uint8(pos, value & 0x000f);
}

template<typename It>
void write_float(It &pos, float value) {
    const auto *const bytes = reinterpret_cast<const uint8_t *>(&value);
    for (size_t i = 0; i < sizeof(float); ++i) {
        write_uint8(pos, bytes[i]);
    }
}

template<typename It>
void write_uint8_vector(It &pos, std::span<const uint8_t> values) {
    write_uint32(pos, values.size());
    for (const uint8_t value : values) {
        write_uint8(pos, value);
    }
}

template<typename It>
void write_string(It &pos, const std::string &s) {
    write_uint32(pos, s.length());
    for (const char c : s) {
        write_uint8(pos, c);
    }
}

}// namespace common

#endif//ANDERSONS_AUTO_JOKES_PRIMITIVES_H
