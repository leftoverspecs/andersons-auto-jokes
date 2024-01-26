#include <GL/glew.h>
#include <SDL.h>

#include <textboxrenderer.h>
#include <boxrenderer.h>
#include <font.h>

#include <iostream>

#include <glm/gtc/matrix_transform.hpp>

#include <boxyfont.h>
#include <boxyfont.png.h>

#ifdef _WIN32
extern "C" {

_declspec(dllexport) uint32_t NvOptimusEnablement = 0x00000001;

}
#endif

int main() {
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


    engine::Font font(width, height, boxyfont, sizeof(boxyfont), assets::boxyfont_widths);
    engine::BoxRenderer frame(width, height);
    engine::TextBoxRenderer textbox(font, frame);

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

        glClearColor(0.0f, 0.5f, 0.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);
        frame.clear();
        font.clear();
        int x, y;
        SDL_GetMouseState(&x, &y);
        /*const float angle = std::atan2(static_cast<float>(height - y - (height - font.get_height()) / 2.0f), static_cast<float>(x));
        const float pos_x = 50.0f;
        const float pos_y = height / 2.0f;
        const float font_height = 4 * font.get_height();
        const float font_width = 3 * font.get_width();
        glm::mat4 model(1.0f);
        model = glm::translate(model, glm::vec3(pos_x, pos_y, 0.0f));
        model = glm::rotate(model, angle, glm::vec3(0.0f, 0.0f, 1.0f));
        model = glm::scale(model, glm::vec3(font_width, font_height, 1.0f));
        font.write(model, glm::vec4(1.0f, 1.0f, 1.0f, 1.0f), "This is a text for us!", 300.0f);
        model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(pos_x, pos_y, 0.0f));
        model = glm::rotate(model, angle, glm::vec3(0.0f, 0.0f, 1.0f));
        model = glm::translate(model, glm::vec3(-10.0f, font_height + 10.0f, 0.0f));
        model = glm::scale(model, glm::vec3(300.0f + 20.0f, -2.0f * font_height - 20.0f, 1.0f));
        frame.queue_frame(model, glm::vec4(1.0f, 1.0f, 1.0f, 0.25f));*/
        textbox.queue(x, height - y, 300, 100, 3, 4, "This is a text for us!");
        frame.draw();
        font.draw();
        SDL_GL_SwapWindow(window);
        SDL_Delay(10);
    }

    SDL_GL_DeleteContext(context);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}
