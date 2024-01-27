#ifndef GGJ2024_SCENE_H
#define GGJ2024_SCENE_H

#include <destination.h>

#include <SDL.h>

class Scene {
public:
    explicit Scene(SDL_Window *window);
    virtual ~Scene();

    bool run();

    void queue(float delta_time);
    void exit();

protected:

private:
    SDL_Window *window;
    bool finished{false};

    virtual void on_loop(float delta_time) = 0;
};


#endif//GGJ2024_SCENE_H
