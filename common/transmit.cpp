#include "transmit.h"

namespace common {

message receive(TCPsocket source) {
    std::vector<uint8_t> buffer(256);
    const int received = SDLNet_TCP_Recv(source, buffer.data(), static_cast<int>(buffer.size()));
    if (received < 0) {
        throw std::runtime_error(std::string("Can't receive data: ") + SDLNet_GetError());
    } else {
        buffer.resize(received);
        auto pos = buffer.begin();
        return {pos, buffer.end()};
    }
}

void send(TCPsocket destination, const message &m) {
    std::vector<uint8_t> buffer{m.serialize()};
    int sent = SDLNet_TCP_Send(destination, buffer.data(), static_cast<int>(buffer.size()));
    if (sent < buffer.size()) {
        throw std::runtime_error(std::string("Can't send data: ") + SDLNet_GetError());
    }
}

}
