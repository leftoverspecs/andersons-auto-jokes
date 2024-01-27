#ifndef GGJ2024_SCENE_H
#define GGJ2024_SCENE_H

#include <destination.h>

#include <SDL.h>

namespace game {

class Scene {
public:
    explicit Scene(int screen_height, SDL_Window *window);
    virtual ~Scene();

    bool run();

    void queue(float delta_time);
    void exit();

protected:
private:
    int screen_height;
    SDL_Window *window;
    bool finished{false};

    virtual void on_loop(float delta_time) = 0;
    virtual void on_key_pressed(SDL_Keycode code);
    virtual void on_mouse_button_down(int x, int y);
    virtual void on_mouse_button_up(int x, int y);
};

}

#endif//GGJ2024_SCENE_H
