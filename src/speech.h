#ifndef ANDERSONS_AUTO_JOKES_SPEECH_H
#define ANDERSONS_AUTO_JOKES_SPEECH_H

#include <spritemap.h>
#include <spriterenderer.h>

namespace game {

class Speech {
public:
    Speech(int width, int height);

    void clear();
    void queue(float x, float y, bool flipped, int index);
    void draw();
private:
    engine::SpriteMap map;
    engine::SpriteRenderer renderer;
};

}// namespace game

#endif //ANDERSONS_AUTO_JOKES_SPEECH_H
