#include <GL/glew.h>
#include <SDL.h>

#include <thread>
#include <vector>
#include <set>
#include <random>
#include <memory>

#include <audio.h>
#include <font.h>
#include <image.h>
#include <music.h>

#include <iostream>
#include <cstdlib>

#include "fight.h"
#include "shop.h"
#include "intro.h"

#include "family.h"
#include "audio_data.h"
#include "server.h"
#include "client.h"
#include "lobby.h"
#include "result.h"
#include "speech.h"
#include "music.h"

#include <boxyfont.h>
#include <boxyfont.png.h>
#include <family.png.h>
#include <frame.png.h>

#ifdef _WIN32
extern "C" {

_declspec(dllexport) uint32_t NvOptimusEnablement = 0x00000001;

}
#endif

const int WIDTH = 800;
const int HEIGHT = 600;

class ServerThread {
public:
    ServerThread() : server(10000, 16) {}
    ~ServerThread() {
        server.shutdown();
        if (thread) {
            thread->join();
        }
    }

    void run() {
        thread = std::make_unique<std::thread>([this]() {
            this->server.run(1000);
        });
    }

private:
    std::unique_ptr<std::thread> thread;
    common::server server;
};


int main(int argc, char *argv[]) {
    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_GAMECONTROLLER | SDL_INIT_AUDIO) != 0) {
        std::cerr << SDL_GetError() << std::endl;
        return EXIT_FAILURE;
    }
    if (SDLNet_Init() != 0) {
        std::cerr << SDLNet_GetError() << '\n';
        return EXIT_FAILURE;
    }

    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
    SDL_GL_SetAttribute(SDL_GL_ACCELERATED_VISUAL, 1);
    SDL_GL_SetAttribute(SDL_GL_RED_SIZE, 8);
    SDL_GL_SetAttribute(SDL_GL_GREEN_SIZE, 8);
    SDL_GL_SetAttribute(SDL_GL_BLUE_SIZE, 8);
    SDL_GL_SetAttribute(SDL_GL_ALPHA_SIZE, 8);

    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 2);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);

    SDL_Window *const window = SDL_CreateWindow("Anderson's Auto Jokes",
                                                SDL_WINDOWPOS_CENTERED,
                                                SDL_WINDOWPOS_CENTERED,
                                                WIDTH,
                                                HEIGHT,
                                                SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN);
    SDL_GLContext context = SDL_GL_CreateContext(window);

    glewExperimental = GL_TRUE;
    if (glewInit() != GLEW_OK) {
        return EXIT_FAILURE;
    }

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    glClearColor(0.0, 0.0, 0.0, 0.0);
    glViewport(0, 0, WIDTH, HEIGHT);

    engine::Audio audio(44100, MIX_DEFAULT_FORMAT, 2, 64);

    bool music_enabled = true;
    for (int i = 1; i < argc; ++i) {
        if (strcmp(argv[i], "--no-music") == 0) {
            music_enabled = false;
        }
    }
    game::AudioData audio_data(music_enabled);
    game::Music music(MIX_MAX_VOLUME / 3);

    engine::Font font(WIDTH, HEIGHT, boxyfont, sizeof(boxyfont), assets::boxyfont_widths);
    game::Speech speech(WIDTH, HEIGHT);
    engine::SpriteMap family_spritemap{family, sizeof(family), 8, 9};
    game::Background frame_renderer{static_cast<float>(WIDTH), static_cast<float>(HEIGHT), frame, sizeof(frame)};

    std::set<const common::Stats *> remaining{game::ALL.begin(), game::ALL.end()};
    std::vector<const common::Stats *> deck;

    std::mt19937 rng{std::random_device{}()};
    std::sample(remaining.begin(), remaining.end(), std::inserter(deck, deck.end()), 2, rng);
    for (const auto p : deck) {
        remaining.erase(p);
    }
    std::vector<const common::Stats *> team1{&game::EMPTY};

    game::Intro intro(window, WIDTH, HEIGHT, font, speech, audio_data, family_spritemap);
    game::Shop shop(window, WIDTH, HEIGHT, font, speech, audio_data, family_spritemap);
    game::Fight fight(window, WIDTH, HEIGHT, font, speech, audio_data, family_spritemap);
    game::Result result(HEIGHT, window, frame_renderer, font);
    int losses = 0;
    std::unique_ptr<common::Client> client;
    std::unique_ptr<ServerThread> server;
    if (music_enabled) {
        music.play_lobby_music();
    }
    if (!intro.run()) {
        return EXIT_SUCCESS;
    }
    if (intro.start_own_server()) {
        server = std::make_unique<ServerThread>();
        server->run();
        client = std::make_unique<common::Client>("localhost", 10000);
    } else {
        client = std::make_unique<common::Client>(intro.get_hostname(), 10000);
    }
    const common::Message hello_answer = client->send(common::Message{ common::Message::type::CLIENT_HELLO, "andersons" });
    if (hello_answer.get_type() != common::Message::type::OK) {
        return EXIT_FAILURE;
    }
    while (true) {
        shop.startup(team1, deck);
        if (!shop.run()) {
            break;
        }
        team1 = shop.get_team();
        game::Lobby lobby(HEIGHT, window, frame_renderer, font, *client);
        if (music_enabled) {
            music.play_fight_music();
        }
        lobby.startup(team1);
        if (!lobby.run()) {
            break;
        }
        fight.startup(team1, lobby.get_opponent_stats());
        if (!fight.run()) {
            break;
        }
        const game::Fight::Winner winner = fight.get_result();
        if (winner == game::Fight::Winner::TEAM2) {
            ++losses;
        }
        result.startup(winner, losses);
        if (!result.run()) {
            break;
        }
        if (team1.size() < 4) {
            team1.push_back(&game::EMPTY);
            if (deck.size() < 4) {
                std::sample(remaining.begin(), remaining.end(), std::inserter(deck, deck.end()), 1, rng);
                for (const auto p : deck) {
                    remaining.erase(p);
                }
            }
        } else {
            break;
        }
    }
    music.stop();
    if (client) {
        client->send(common::Message{common::Message::type::CLIENT_QUIT});
        client.reset();
    }
    server.reset();
    SDL_GL_DeleteContext(context);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}
