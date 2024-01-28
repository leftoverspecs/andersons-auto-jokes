//
// Created by philipp on 22.01.24.
//

#ifndef GGJ2024_TRANSMIT_H
#define GGJ2024_TRANSMIT_H

#include <SDL_net.h>

#include <cstdint>
#include <stdexcept>
#include <vector>

#include "message.h"

namespace common {

Message receive(TCPsocket source);

void send(TCPsocket destination, const Message &m);

}

#endif //GGJ2024_TRANSMIT_H
