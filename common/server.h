#ifndef ANDERSONS_AUTO_JOKES_SERVER_H
#define ANDERSONS_AUTO_JOKES_SERVER_H

#include "message.h"

#include <SDL_net.h>

namespace common {

class handler {
public:
    virtual ~handler() = default;
    virtual Message receive(int id, const Message &data) = 0;
    virtual bool idle() = 0;
};

class server {
public:
    server(uint16_t port, int max_connections);
    ~server();

    void run(unsigned int timeout);
    void shutdown();

private:
    SDLNet_SocketSet socket_set;
    std::vector<TCPsocket> clients;
    unsigned int num_clients;
    bool running;
    TCPsocket server_socket;

    int check_server_socket();
    int check_client_socket(int i, handler &h);
    void add_client(TCPsocket client);
    void remove_client(int i);
};

}// namespace common

#endif //ANDERSONS_AUTO_JOKES_SERVER_H
