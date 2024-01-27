#ifndef GGJ2024_MESSAGE_H
#define GGJ2024_MESSAGE_H

#include <SDL_net.h>

#include "stats.h"
#include "primitives.h"

#include <cstdint>
#include <span>
#include <stdexcept>
#include <vector>

namespace common {

class message {
public:
    enum class type : uint8_t {
        OK,
        ERROR,
        CLIENT_QUIT,
        SERVER_QUIT,
        CLIENT_HELLO,
        BATTLE,
        RETRY,
        OPPONENT
    };

    message();
    explicit message(std::string msg);
    explicit message(type t);
    message(type t, const std::string &m);

    message(type t, const std::vector<Stats> &stats);

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
