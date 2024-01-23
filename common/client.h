//
// Created by philipp on 23.01.24.
//

#ifndef GGJ2024_CLIENT_H
#define GGJ2024_CLIENT_H

#include "message.h"

#include <cstdint>
#include <string>

#include <SDL_net.h>

namespace common {

class client {
public:
    class exception : public std::runtime_error {
    public:
        explicit exception(const std::string &message);
    };
    client(const std::string &hostname, uint16_t port);
    ~client();

    message send(const message &m) const;
private:
    TCPsocket socket;
};

}// namespace common

#endif//GGJ2024_CLIENT_H
