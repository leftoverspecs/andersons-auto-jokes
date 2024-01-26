#include "textboxrenderer.h"

#include "boxrenderer.h"
#include "font.h"

#include <glm/gtc/matrix_transform.hpp>

namespace engine {

TextBoxRenderer::TextBoxRenderer(Font &font, BoxRenderer &box_renderer)
    : font(font), box_renderer(box_renderer) {}

void TextBoxRenderer::queue(float x, float y, float width, float height,
                            float char_scale_x, float char_scale_y,
                            const std::string &text,
                            const glm::vec4 &text_color,
                            const glm::vec4 &box_color) {
    const auto char_width = static_cast<float>(font.get_width()) * char_scale_x;
    const auto char_height = static_cast<float>(font.get_height()) * char_scale_y;

    glm::mat4 model(1.0f);
    model = glm::translate(model, glm::vec3(x, y, 0.0f));
    model = glm::scale(model, glm::vec3(char_width, char_height, 1.0f));
    model = glm::translate(model, glm::vec3(0.0, -1.0, 0.0));
    font.write(model, text_color, text, static_cast<float>(width));

    model = glm::mat4(1.0f);
    model = glm::translate(model, glm::vec3(static_cast<float>(x) - 10.0f, static_cast<float>(y) + 10.0f, 0.0f));
    model = glm::scale(model, glm::vec3(static_cast<float>(width) + 20.0f, -static_cast<float>(height) - 20.0f, 1.0f));
    box_renderer.queue_frame(model, box_color);

}

bool TextBoxRenderer::is_inside(float x, float y, float width, float height, float test_x, float test_y) {
    return (test_x >= x - 10.0f && test_x <= x + width + 10.0f) && (test_y <= y + 10.0f && test_y >= y - height - 10.0f);
}

}
