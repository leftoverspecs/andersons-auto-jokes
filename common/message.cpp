#include "message.h"

#include "primitives.h"
#include "stats.h"

namespace common {

std::vector<uint8_t> Message::serialize() const {
    std::vector<uint8_t> buffer;
    buffer.reserve(sizeof(uint8_t) + content.size());
    buffer.push_back(static_cast<uint8_t>(t));
    buffer.insert(buffer.end(), content.begin(), content.end());
    return buffer;
}

Message::Message() : t{type::OK} {}

Message::Message(std::string msg) : t{type::OK}, content{msg.begin(), msg.end()} {}

Message::Message(Message::type t) : t{t} {}

Message::Message(Message::type t, const std::string &m) : t{t}, content{m.begin(), m.end()} {}

Message::Message(Message::type t, const std::vector<Stats> &stats)
    : t{t}, content{} {
    auto pos = std::back_inserter(content);
    write_stats(pos, stats);
}

}// namespace common