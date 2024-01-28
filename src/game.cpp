#include <GL/glew.h>
#include <SDL.h>

#include <audio.h>
#include <chunk.h>

#include <controller.h>
#include <font.h>
#include <image.h>
#include <music.h>

#include <iostream>
#include <cstdlib>

#include "fight.h"
#include "person.h"
#include "shop.h"

#include "audio_data.h"
#include "client.h"
#include "lobby.h"
#include "result.h"

#include <family.png.h>
#include <frame.png.h>
#include <lobby.mp3.h>
#include <boxyfont.h>
#include <boxyfont.png.h>

#ifdef _WIN32
extern "C" {

_declspec(dllexport) uint32_t NvOptimusEnablement = 0x00000001;

}
#endif

const int WIDTH = 800;
const int HEIGHT = 600;

int main() {
    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_GAMECONTROLLER | SDL_INIT_AUDIO) != 0) {
        std::cerr << SDL_GetError() << std::endl;
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
    game::AudioData audio_data;
    engine::Music lobby_music{lobby, sizeof(lobby)};

    common::Client client("localhost", 10000);
    const common::Message hello_answer = client.send(common::Message{common::Message::type::CLIENT_HELLO, "andersons"});
    if (hello_answer.get_type() != common::Message::type::OK) {
        return EXIT_FAILURE;
    }

    engine::Font font(WIDTH, HEIGHT, boxyfont, sizeof(boxyfont), assets::boxyfont_widths);
    engine::SpriteMap family_spritemap{family, sizeof(family), 8, 9};
    game::Background frame_renderer{WIDTH, HEIGHT, frame, sizeof(frame)};

    const common::Stats empty;
    const common::Stats dad{"Dad", 1, 10.0, 5.0, 1.0, 0};
    const common::Stats mum{"Mum", 2, 10.0, 1.0, 0.5, 1};
    const common::Stats big_sister{"Big Sister", 3, 10.0, 1.0, 1.0, 2};
    const common::Stats little_brother{"Little Brother", 4, 10.0, 1.0, 5.0, 3};
    const common::Stats uncle{"Uncle", 5, 10.0, 5.0, 3.0, 4};

    const std::vector<const common::Stats *> all = {&empty, &dad, &mum, &big_sister, &little_brother, &uncle};
    std::vector<const common::Stats *> available{&dad, &mum};
    std::vector<const common::Stats *> team1{&empty};

    game::Shop shop(window, WIDTH, HEIGHT, font, audio_data, family_spritemap);
    game::Fight fight(window, WIDTH, HEIGHT, font, audio_data, family_spritemap);
    game::Result result(HEIGHT, window, frame_renderer, font);
    int losses = 0;
    while (true) {
        shop.startup(team1, available);
        if (!shop.run()) {
            break;
        }
        team1 = shop.get_team();
        game::Lobby lobby(HEIGHT, window, frame_renderer, font, client);
        lobby_music.fade_in(-1, 1000);
        lobby.startup(team1);
        if (!lobby.run()) {
            break;
        }
        lobby_music.fade_out(1000);
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
        if (team1.size() + 2 < all.size()) {
            team1.push_back(&empty);
            available.push_back(all.at(team1.size() + 1));
        } else {
            break;
        }
    }

    client.send(common::Message{common::Message::type::CLIENT_QUIT});
    SDL_GL_DeleteContext(context);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}
