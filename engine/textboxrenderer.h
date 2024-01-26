#ifndef GGJ2024_TEXTBOXRENDERER_H
#define GGJ2024_TEXTBOXRENDERER_H

#include <string>

namespace engine {

class Font;
class BoxRenderer;

class TextBoxRenderer {
public:
    explicit TextBoxRenderer(Font &font, BoxRenderer &box_renderer);

    void queue(int x, int y, int width, int height, float char_scale_x, float char_scale_y, const std::string &text);
private:
    Font &font;
    BoxRenderer &box_renderer;
};

}

#endif//GGJ2024_TEXTBOXRENDERER_H
