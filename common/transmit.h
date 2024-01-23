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

message receive(TCPsocket source);

void send(TCPsocket destination, const message &m);

}

#endif //GGJ2024_TRANSMIT_H
