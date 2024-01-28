#include <client.h>
#include <initialize.h>
#include <message.h>

#include <iostream>
#include <string>
#include <vector>

#include <SDL.h>
#include <SDL_net.h>

int main() {
    common::Initialize sdl{SDL_Init(0), SDL_Quit, SDL_GetError};
    common::Initialize net{SDLNet_Init(), SDLNet_Quit, SDLNet_GetError};
    common::Client c("localhost", 10000);
    const common::Message hello_answer = c.send(common::Message{common::Message::type::CLIENT_HELLO, "test_client"});
    if (hello_answer.get_type() != common::Message::type::OK) {
        return EXIT_FAILURE;
    }

    std::string payload;
    bool quit = false;
    while (!quit) {
        getline(std::cin, payload);
        common::Message answer;
        if (payload == "CLIENT_QUIT") {
            answer = c.send(common::Message{common::Message::type::CLIENT_QUIT});
        } else if (payload == "SERVER_QUIT") {
            answer = c.send(common::Message{common::Message::type::SERVER_QUIT});
        } else {
            answer = c.send(common::Message{payload});
        }
        const std::vector<uint8_t> &content = answer.get_content();
        switch (answer.get_type()) {
        case common::Message::type::OK:
            std::cout << "OK: ";
            break;
        case common::Message::type::CLIENT_QUIT:
            std::cout << "CLIENT_QUIT: ";
            break;
        case common::Message::type::SERVER_QUIT:
            std::cout << "SERVER_QUIT: ";
            break;
        default:
            std::cout << static_cast<uint8_t>(answer.get_type()) << ": ";
        }
        std::cout << std::string(content.data(), content.data() + content.size()) << std::endl;
        if (answer.get_type() == common::Message::type::CLIENT_QUIT) {
            quit = true;
        }
    }
    return 0;
}
