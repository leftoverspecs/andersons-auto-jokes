#pragma once

#include <spritemap.h>
#include <spriterenderer.h>

namespace game {

class Background {
public:
    Background(float width, float height, int index);

    void draw();
private:
    float width;
    float height;
    engine::SpriteMap sprite;
    engine::SpriteRenderer renderer;
};

}
