#include "scene.h"

#include <SDL.h>

namespace game {

Scene::Scene(int screen_height, SDL_Window *window)
    : screen_height{screen_height}, window{window} {}

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
            case SDL_MOUSEBUTTONUP:
                if (event.button.button == SDL_BUTTON_LEFT) {
                    on_mouse_button_up(event.button.x, screen_height - event.button.y);
                }
                break;
            case SDL_MOUSEBUTTONDOWN:
                if (event.button.button == SDL_BUTTON_LEFT) {
                    on_mouse_button_down(event.button.x, screen_height - event.button.y);
                }
                break;
            }
        }

        const Uint64 next = SDL_GetTicks64();
        const auto delta_time = static_cast<float>(next - last);
        last = next;
        on_loop(delta_time);
        SDL_GL_SwapWindow(window);

        /*if (delta_time > 0) {
            const float fps = 1000.0f / delta_time;
            SDL_SetWindowTitle(window, std::to_string(fps).c_str());
        }*/
    }
    return true;
}
void Scene::exit() {
    finished = true;
}

void Scene::on_key_pressed(SDL_Keycode code) {}
void Scene::on_mouse_button_down(int x, int y) {}
void Scene::on_mouse_button_up(int x, int y) {}

}