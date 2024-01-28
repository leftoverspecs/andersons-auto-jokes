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

class Message {
public:
    enum class type : uint8_t {
        OK,
        ERROR,
        CLIENT_QUIT,
        SERVER_QUIT,
        CLIENT_HELLO,
        GET_OPPONENT,
        RETRY,
        OPPONENT
    };

    Message();
    explicit Message(std::string msg);
    explicit Message(type t);
    Message(type t, const std::string &m);

    Message(type t, const std::vector<Stats> &stats);

    template<typename It>
    Message(It &pos, It end)
        : t{read_uint8(pos, end)},
          content{pos, end} {}

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
