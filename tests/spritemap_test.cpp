#include <GL/glew.h>
#include <SDL.h>
#include <SDL_opengl.h>

#include <spritemap.h>
#include <spriterenderer.h>
#include <texture.h>

#include <iostream>

#include <test_spritemap.png.h>

#ifdef _WIN32
extern "C" {

_declspec(dllexport) uint32_t NvOptimusEnablement = 0x00000001;

}
#endif

int main(int argc, char *argv[]) {
    if (SDL_Init(SDL_INIT_VIDEO) != 0) {
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

    static const int width = 800;
    static const int height = 600;

    SDL_Window *const window = SDL_CreateWindow("visualizer",
                                                SDL_WINDOWPOS_CENTERED,
                                                SDL_WINDOWPOS_CENTERED,
                                                width,
                                                height,
                                                SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN);
    SDL_GLContext context = SDL_GL_CreateContext(window);

    glewExperimental = GL_TRUE;
    if (glewInit() != GLEW_OK) {
        return EXIT_FAILURE;
    }

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    glClearColor(0.0, 0.0, 0.0, 0.0);
    glViewport(0, 0, width, height);

    engine::SpriteMap sprites(test_spritemap, sizeof(test_spritemap), 2, 2);
    engine::SpriteRenderer renderer(sprites, width, height);

    bool quit = false;
    while (!quit) {
        SDL_Event event;
        while(SDL_PollEvent(&event)) {
            switch(event.type) {
            case SDL_QUIT:
                quit = true;
                break;
            case SDL_KEYUP:
                switch (event.key.keysym.sym) {
                case SDLK_ESCAPE:
                    quit = true;
                    break;
                }
            }
        }

        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);
        renderer.clear();
        renderer.queue(10, 10, sprites.get_sprite_width(), sprites.get_sprite_height(), 1.0f, 1.0f, 1.0f, 1.0f, (SDL_GetTicks64() / 1000) % 2, (SDL_GetTicks64() / 2000) % 2);
        renderer.draw();
        SDL_GL_SwapWindow(window);
        SDL_Delay(100);
    }

    SDL_GL_DeleteContext(context);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}
