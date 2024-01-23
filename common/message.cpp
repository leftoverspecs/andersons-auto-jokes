#include "message.h"

namespace common {

std::vector<uint8_t> message::serialize() const {
    std::vector<uint8_t> buffer(sizeof(uint8_t) + sizeof(uint32_t) + content.size());
    auto pos = buffer.begin();
    const auto end = buffer.end();
    write_uint8(pos, end, static_cast<uint8_t>(t));
    write_uint8_vector(pos, end, content);
    return buffer;
}

message::message() : t{type::OK} {}

message::message(std::string msg) : t{type::OK}, content{msg.begin(), msg.end()} {}

message::message(message::type t) : t{t} {}

message::message(message::type t, const std::string &m) : t{t}, content{m.begin(), m.end()} {}

}// namespace common