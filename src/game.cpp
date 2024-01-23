#include <GL/glew.h>
#include <SDL.h>

#include <audio.h>
#include <controller.h>
#include <destination.h>
#include <image.h>

#include <iostream>
#include <string>

#include <glm/gtx/transform.hpp>

#include "background.h"

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

    game::Background background(WIDTH, HEIGHT, 0);
    game::Background background2(WIDTH, HEIGHT, 1);
    engine::Destination destination(WIDTH, HEIGHT);
    engine::Destination destination2(WIDTH, HEIGHT);

    bool quit = false;
    Uint64 last = SDL_GetTicks64();
    while (!quit) {
        SDL_Event event;
        while (SDL_PollEvent(&event)) {
            switch (event.type) {
            case SDL_QUIT:
                quit = true;
                break;
            case SDL_KEYUP:
                switch (event.key.keysym.sym) {
                case SDLK_ESCAPE:
                    quit = true;
                    break;
                }
                break;
            }
        }
        const Uint64 next = SDL_GetTicks64();
        const Uint64 diff = next - last;
        last = next;
        {
            auto binding = destination.bind_as_target();
            glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
            glClear(GL_COLOR_BUFFER_BIT);

            background.draw();
        }
        {
            auto binding = destination2.bind_as_target();
            glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
            glClear(GL_COLOR_BUFFER_BIT);

            background2.draw();
        }
        const float alpha = 0.5f * std::sin(2 * next * M_PI / 5000.0f) + 0.5f;
        destination.draw(glm::translate(glm::vec3(0.0f, 0.0f, 0.0f)), glm::vec3(0.0f), alpha, glm::vec3(1.0f));
        destination2.draw(glm::translate(glm::vec3(0.0f, 0.0f, 0.0f)), glm::vec3(0.0f), 1.0f - alpha, glm::vec3(1.0f));
        SDL_GL_SwapWindow(window);

        if (diff > 0) {
            const float fps = 1000.0f / static_cast<float>(diff);
            SDL_SetWindowTitle(window, std::to_string(fps).c_str());
        }
    }

    SDL_GL_DeleteContext(context);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}
