#include "scene.h"

#include <SDL.h>

namespace game {

Scene::Scene(SDL_Window *window) : window{window} {}

Scene::~Scene() = default;

bool Scene::run() {
    finished = false;
    Uint64 last = SDL_GetTicks64();
    while (!finished) {
        SDL_Event event;
        while (SDL_PollEvent(&event)) {
            switch (event.type) {
            case SDL_QUIT:
                return false;
            case SDL_KEYUP:
                switch (event.key.keysym.sym) {
                case SDLK_ESCAPE:
                    return false;
                default:
                    on_key_pressed(event.key.keysym.sym);
                    break;
                }
                break;
            }
        }

        const Uint64 next = SDL_GetTicks64();
        const auto delta_time = static_cast<float>(next - last);
        last = next;
        on_loop(delta_time);
        SDL_GL_SwapWindow(window);
    }
    return true;
}
void Scene::exit() {
    finished = true;
}

void Scene::on_key_pressed(SDL_Keycode code) {}

}