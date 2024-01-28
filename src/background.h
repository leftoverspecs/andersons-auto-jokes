#pragma once

#include <spritemap.h>
#include <spriterenderer.h>

namespace game {

class Background {
public:
    Background(float width, float height, const unsigned char *image, std::size_t image_size);

    void draw(float alpha);
private:
    float width;
    float height;
    engine::SpriteMap sprite;
    engine::SpriteRenderer renderer;
};

}
