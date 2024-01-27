#include "message.h"

#include "primitives.h"
#include "stats.h"

namespace common {

std::vector<uint8_t> message::serialize() const {
    std::vector<uint8_t> buffer;
    buffer.reserve(sizeof(uint8_t) + sizeof(uint32_t) + content.size());
    auto pos = std::back_inserter(buffer);
    write_uint8(pos, static_cast<uint8_t>(t));
    write_uint8_vector(pos, content);
    return buffer;
}

message::message() : t{type::OK} {}

message::message(std::string msg) : t{type::OK}, content{msg.begin(), msg.end()} {}

message::message(message::type t) : t{t} {}

message::message(message::type t, const std::string &m) : t{t}, content{m.begin(), m.end()} {}

message::message(message::type t, const std::vector<Stats> &stats)
    : t{t}, content{} {
    auto pos = std::back_inserter(content);
    write_stats(pos, stats);
}

}// namespace common