#include <SDL.h>
#include <SDL_net.h>

#include <algorithm>
#include <iostream>
#include <vector>

#include <initialize.h>
#include <map>
#include <optional>

#include <message.h>
#include <stats.h>
#include <transmit.h>

class handler {
public:
    virtual ~handler() = default;
    virtual common::Message receive(int id, const common::Message &data) = 0;
    virtual bool idle() = 0;
};

class server {
public:
    server(uint16_t port, int max_connections);
    ~server();

    void run(handler &h, unsigned int timeout);

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

namespace {

TCPsocket open_server_socket(uint16_t port) {
    IPaddress addr;
    if (SDLNet_ResolveHost(&addr, nullptr, port) < 0) {
        SDL_Log("Error resolving host: %s", SDLNet_GetError());
        throw std::runtime_error("Error resolving host");
    }
    TCPsocket socket = SDLNet_TCP_Open(&addr);
    if (socket == nullptr) {
        SDL_Log("Error opening socket: %s", SDLNet_GetError());
        throw std::runtime_error("Error opening socket");
    }
    return socket;
}

} // namespace

server::server(uint16_t port, int max_connections)
    : socket_set(SDLNet_AllocSocketSet(max_connections)),
      server_socket(open_server_socket(port)),
      clients(max_connections, nullptr),
      num_clients(0),
      running(true)
{
    if (socket_set == nullptr) {
        SDL_Log("Error allocating socket set: %s", SDLNet_GetError());
        throw std::runtime_error("Error allocating socket set");
    }
    if (SDLNet_TCP_AddSocket(socket_set, server_socket) != 1) {
        SDL_Log("Error adding server socket to socket set: %s", SDLNet_GetError());
        throw std::runtime_error("Error adding server socket to socket set");
    }
}

server::~server() {
    for (int i = 0; i < clients.size(); ++i) {
        remove_client(i);
    }
    SDLNet_TCP_DelSocket(socket_set, server_socket);
    SDLNet_TCP_Close(server_socket);
    SDLNet_FreeSocketSet(socket_set);
}

void server::run(handler &h, unsigned int timeout) {
    SDL_Log("Server: Let's serve!");
    while (running || num_clients > 0) {
        int ready = SDLNet_CheckSockets(socket_set, timeout);
        if (ready < 0) {
            SDL_Log("Error checking sockets: %s", SDLNet_GetError());
            throw std::runtime_error("Error checking sockets");
        } else if (ready == 0) {
            if (!h.idle()) {
                running = false;
            }
        } else {
            ready -= check_server_socket();
            int i = 0;
            while (i < clients.size() && ready > 0) {
                TCPsocket &client = clients[i];
                if (client != nullptr) {
                    ready -= check_client_socket(i, h);
                }
                ++i;
            }
        }
    }
    SDL_Log("Server: Goodbye!");
}

int server::check_client_socket(int i, handler &h) {
    TCPsocket &client = clients[i];
    if (SDLNet_SocketReady(client)) {
        const common::Message received = common::receive(client);
        const common::Message answer = h.receive(i, received);
        if (running) {
            common::send(client, answer);
            switch (received.get_type()) {
            case common::Message::type::CLIENT_QUIT:
                remove_client(i);
                break;
            case common::Message::type::SERVER_QUIT:
                remove_client(i);
                running = false;
                break;
            default:
                break;
            }
        } else {
            send(client, common::Message(common::Message::type::CLIENT_QUIT));
            remove_client(i);
        }
        return 1;
    } else {
        return 0;
    }
}

int server::check_server_socket() {
    if (SDLNet_SocketReady(server_socket)) {
        if (num_clients < clients.size()) {
            TCPsocket client = SDLNet_TCP_Accept(server_socket);
            if (client != nullptr) {
                if (running) {
                    add_client(client);
                } else {
                    SDLNet_TCP_Close(client);
                    SDL_Log("Can't accept more clients since server shuts down");
                }
            } else {
                SDL_Log("Error accepting Client: %s", SDLNet_GetError());
            }
        } else {
            SDL_Log("Can't accept more clients, already accepted %ld", clients.size());
        }
        return 1;
    } else {
        return 0;
    }
}

void server::add_client(TCPsocket client) {
    const auto it = std::find(clients.begin(), clients.end(), nullptr);
    if (it != clients.end()) {
        if (SDLNet_TCP_AddSocket(socket_set, client) > 0) {
            SDL_Log("Accepting Client %d", num_clients);
            *it = client;
            ++num_clients;
        } else {
            SDL_Log("Error adding Client socket to socket set: %s", SDLNet_GetError());
            SDLNet_TCP_Close(client);
        }
    } else {
        SDL_Log("Can't find any free Client slots");
        SDLNet_TCP_Close(client);
    }
}

void server::remove_client(int i) {
    TCPsocket &client = clients[i];
    if (client != nullptr) {
        SDL_Log("Client %d quit", i);
        SDLNet_TCP_DelSocket(socket_set, client);
        SDLNet_TCP_Close(client);
        client = nullptr;
        --num_clients;
    }
}

class test_handler : public handler {
public:
    common::Message receive(int id, const common::Message &received) override {
        const std::vector<uint8_t> &content = received.get_content();
        switch (received.get_type()) {
        case common::Message::type::OK: {
            const std::string message{content.data(), content.data() + content.size()};
            std::cout << "Client " << id << ": OK, \"" << message << "\"\n";
            return common::Message{common::Message::type::OK};
        }
        case common::Message::type::ERROR: {
            const std::string message{content.data(), content.data() + content.size()};
            std::cout << "Client " << id << ": ERROR, \"" << message << "\"\n";
            return common::Message{common::Message::type::OK};
        }
        case common::Message::type::CLIENT_HELLO: {
            const std::string message{content.data(), content.data() + content.size()};
            std::cout << "Client " << id << ": CLIENT_HELLO, \"" << message << "\"\n";
            if (on_client_hello(id, message)) {
                return common::Message{common::Message::type::OK};
            } else {
                return common::Message{common::Message::type::ERROR, "Can't accept new Client"};
            }
        }
        case common::Message::type::CLIENT_QUIT: {
            const std::string message{content.data(), content.data() + content.size()};
            std::cout << "Client " << id << ": CLIENT_QUIT, \"" << message << "\"\n";
            on_client_quit(id);
            return common::Message{common::Message::type::CLIENT_QUIT};
        }
        case common::Message::type::SERVER_QUIT: {
            const std::string message{content.data(), content.data() + content.size()};
            std::cout << "Client " << id << ": SERVER_QUIT, \"" << message << "\"\n";
            return common::Message{common::Message::type::CLIENT_QUIT};
        }
        case common::Message::type::GET_OPPONENT: {
            auto pos = content.begin();
            std::vector<common::Stats> stats = common::read_stats(pos, content.end());
            std::cout << "Client " << id << ": BATTLE\n";
            auto it = served.begin();
            while (it != served.end()) {
                const int opponent_id = it->first;
                if (opponent_id != id) {
                    const auto opponent_stats = it->second;
                    if (opponent_stats.size() == stats.size()) {
                        served.erase(it);
                        return common::Message{common::Message::type::OPPONENT, opponent_stats};
                    }
                }
                ++it;
            }
            it = waiting.begin();
            while (it != waiting.end()) {
                const int opponent_id = it->first;
                if (opponent_id != id) {
                    const auto opponent_stats = it->second;
                    if (opponent_stats.size() == stats.size()) {
                        waiting.erase(it);
                        served.emplace(id, stats);
                        return common::Message{common::Message::type::OPPONENT, opponent_stats};
                    }
                }
                ++it;
            }
            if (waiting.find(id) == waiting.end()) {
                waiting.emplace(id, stats);
            }
            return common::Message{common::Message::type::RETRY};
        }
        default:
            return common::Message{common::Message::type::ERROR, "Unexpected message type"};
        }
    }

    bool idle() override {
        return true;
    }

private:
    std::map<int, std::string> players;
    std::map<int, std::vector<common::Stats>> waiting;
    std::map<int, std::vector<common::Stats>> served;


    bool on_client_hello(int id, const std::string &name) {
        if (players.find(id) != players.end()) {
            SDL_Log("Player with id %d already exists", id);
            return false;
        }
        players.emplace(id, name);
        SDL_Log("Player %s is joining the party", name.c_str());
        return true;
    }

    void on_client_quit(int id) {
        if (players.find(id) == players.end()) {
            SDL_Log("Player with id %d does not exist", id);
        } else {
            players.erase(id);
            waiting.erase(id);
            served.erase(id);
            SDL_Log("Player with id %d quits the party", id);
        }
    }
};


int main(int argc, char *argv[]) {
    common::Initialize sdl_init(SDL_Init(0), &SDL_Quit, &SDL_GetError);
    common::Initialize sdlnet_init(SDLNet_Init(), &SDLNet_Quit, &SDLNet_GetError);
    server s(10000, 10);
    test_handler h;
    s.run(h, 1000);
    return 0;
}

