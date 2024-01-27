#ifndef GGJ2024_ARENA_H
#define GGJ2024_ARENA_H

#include <spritemap.h>
#include <spriterenderer.h>

class Arena {
public:
    Arena(float width, float height);

    void draw();
private:
    float width;
    float height;
    engine::SpriteMap sprite;
    engine::SpriteRenderer renderer;
};


#endif//GGJ2024_ARENA_H
