#ifndef GGJ2024_TEXTBOXRENDERER_H
#define GGJ2024_TEXTBOXRENDERER_H

#include <glm/vec4.hpp>

#include <string>

namespace engine {

class Font;
class BoxRenderer;

class TextBoxRenderer {
public:
    explicit TextBoxRenderer(Font &font, BoxRenderer &box_renderer);

    void queue(float x, float y, float width, float height,
               float char_scale_x, float char_scale_y,
               const std::string &text,
               const glm::vec4 &text_color,
               const glm::vec4 &box_color);

    static bool is_inside(float x, float y, float width, float height, float test_x, float test_y);
private:
    Font &font;
    BoxRenderer &box_renderer;
};

}

#endif//GGJ2024_TEXTBOXRENDERER_H
