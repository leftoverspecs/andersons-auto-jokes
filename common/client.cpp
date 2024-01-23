#include "client.h"
#include "transmit.h"

#include <stdexcept>

namespace common {

client::exception::exception(const std::string &message)
    : std::runtime_error(message + ": " + SDLNet_GetError())
{}

client::client(const std::string &hostname, uint16_t port) {
    IPaddress address;
    if (SDLNet_ResolveHost(&address, hostname.c_str(), port) == -1) {
        throw exception("Can't resolve host \"" + hostname + ':' + std::to_string(port) + '"');
    }
    socket = SDLNet_TCP_Open(&address);
    if (socket == nullptr) {
        throw exception("Can't open connection to host \"" + hostname + ':' + std::to_string(port) + '"');
    }
}

client::~client() {
    SDLNet_TCP_Close(socket);
}

message client::send(const message &m) const {
    common::send(socket, m);
    return common::receive(socket);
}

} // namespace common
