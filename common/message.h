#ifndef GGJ2024_MESSAGE_H
#define GGJ2024_MESSAGE_H

#include <SDL_net.h>

#include <cstdint>
#include <span>
#include <stdexcept>
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
void write_uint8(It &pos, It end, uint8_t value) {
    if (std::distance(pos, end) >= 1) {
        *pos++ = value;
    } else {
        throw std::runtime_error("Not enough free space in buffer: can't write uint8");
    }
}

template<typename It>
void write_uint32(It &pos, It end, uint32_t value) {
    if (std::distance(pos, end) >= sizeof(uint32_t)) {
        write_uint8(pos, end, (value & 0xf000) >> 24);
        write_uint8(pos, end, (value & 0x0f00) >> 16);
        write_uint8(pos, end, (value & 0x00f0) >> 8);
        write_uint8(pos, end, value & 0x000f);
    } else {
        throw std::runtime_error("Not enough free space in buffer: can't write uint8");
    }
}

template<typename It>
void write_uint8_vector(It &pos, It end, std::span<const uint8_t> values) {
    if (std::distance(pos, end) >= sizeof(uint32_t) + values.size()) {
        write_uint32(pos, end, values.size());
        for (const uint8_t value : values) {
            write_uint8(pos, end, value);
        }
    } else {
        throw std::runtime_error("Not enough free space in buffer: can't write vector of uint8");
    }
}

template<typename It>
void write_string(It &pos, It end, const std::string &s) {
    if (std::distance(pos, end) >= sizeof(uint32_t) + s.length()) {
        write_uint8_vector(pos, end, std::span<const char>(s.begin(), s.end()));
    } else {
        throw std::runtime_error("Not enough free space in buffer: can't write string");
    }
}

class message {
public:
    enum class type : uint8_t {
        OK,
        ERROR,
        CLIENT_QUIT,
        SERVER_QUIT,
        CLIENT_HELLO,
    };

    message();
    explicit message(std::string msg);
    explicit message(type t);
    message(type t, const std::string &m);

    template<typename It>
    message(It &pos, It end)
        : t{read_uint8(pos, end)},
          content{read_uint8_vector(pos, end)} {}

    [[nodiscard]]
    type get_type() const { return t; }

    [[nodiscard]]
    const std::vector<uint8_t> &get_content() const { return content; }

    [[nodiscard]]
    std::vector<uint8_t> serialize() const;

private:
    type t;
    std::vector<uint8_t> content;
};

} // namespace common

#endif//GGJ2024_MESSAGE_H
