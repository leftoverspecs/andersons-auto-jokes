#pragma once

#include "spritemap.h"
#include "spriterenderer.h"

#include <glm/mat4x4.hpp>
#include <glm/vec4.hpp>

#include <vector>

namespace engine {

class Font {
public:
    Font(GLfloat width, GLfloat height, const unsigned char *png, std::size_t size);
    Font(GLfloat width, GLfloat height, const unsigned char *png, std::size_t size,
         std::vector<float> widths);

    void clear();
    void write(glm::mat4 model, const glm::vec4 &color, const char *text);
    void write(glm::mat4 model, const glm::vec4 &color, const std::string &text, float max_line_length);
    void draw();

    [[nodiscard]] float get_width() const { return sprites.get_sprite_width(); }
    [[nodiscard]] float get_height() const { return sprites.get_sprite_height(); }
private:
    SpriteMap sprites;
    SpriteRenderer renderer;
    std::vector<float> widths;

    [[nodiscard]] float get_text_width(const std::string &s) const;
};

}
