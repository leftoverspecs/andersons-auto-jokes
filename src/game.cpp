#include <GL/glew.h>
#include <SDL.h>

#include <audio.h>
#include <boxrenderer.h>
#include <controller.h>
#include <font.h>
#include <image.h>

#include <iostream>

#include "fight.h"
#include "person.h"
#include "shop.h"

#include <family.png.h>

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

    SDL_Window *const window = SDL_CreateWindow("Global Game Jam 2024!",
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

    //engine::Audio audio(44100, MIX_DEFAULT_FORMAT, 2, 128);

    engine::Font font(WIDTH, HEIGHT, boxyfont, sizeof(boxyfont), assets::boxyfont_widths);
    engine::SpriteMap family_spritemap{family, sizeof(family), 8, 8};
    engine::SpriteRenderer family_renderer{family_spritemap, WIDTH, HEIGHT};
    engine::BoxRenderer box{WIDTH, HEIGHT};

    std::vector<Person::Stats> team1{
            Person::Stats{0, 10.0, 1.0, 5.0},
            Person::Stats{0, 10.0, 5.0, 2.0},
    };
    std::vector<Person::Stats> team2{
            Person::Stats{0, 10.0, 5.0, 2.0},
            Person::Stats{0, 10.0, 1.0, 5.0},
    };

    Fight fight(window, WIDTH, HEIGHT, font, family_spritemap);
    Shop shop(window, WIDTH, HEIGHT, font, family_spritemap);
    while (true) {
        shop.startup(team1);
        if (!shop.run()) {
            break;
        }
        team1 = shop.get_team();
        fight.startup(team1, team2);
        if (!fight.run()) {
            break;
        }
    }

    SDL_GL_DeleteContext(context);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}
