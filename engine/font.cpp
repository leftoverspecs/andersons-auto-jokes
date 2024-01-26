#include "font.h"

#include <glm/gtx/transform.hpp>

namespace engine {

Font::Font(GLfloat width, GLfloat height, const unsigned char *png, std::size_t size)
    : Font(width, height, png, size, std::vector<float>(128, 1.0f)) {}

Font::Font(GLfloat width, GLfloat height, const unsigned char *png, std::size_t size,
           std::vector<float> widths)
    : sprites(png, size, 16, 8),
      renderer(sprites, width, height),
      widths(std::move(widths)) {}

void Font::clear() {
    renderer.clear();
}

void Font::write(glm::mat4 model, const glm::vec4 &color, const char *text) {
    glm::mat4 line_start = model;
    glm::mat4 line = line_start;
    const glm::mat4 newline = glm::translate(glm::vec3(0.0f, -1.0f, 0.0f));
    for (const char *c = text; *c != '\0'; ++c) {
        if (*c != '\n') {
            const glm::mat4 t = glm::translate(glm::vec3(widths[*c], 0.0f, 0.0f));
            unsigned int i = *c & 0x0f;
            unsigned int j = (*c >> 4) & 0x0f;
            renderer.queue(line, color, i, j);
            line = line * t;
        } else {
            line_start = line_start * newline;
            line = line_start;
        }
    }
}

void Font::write(glm::mat4 model, const glm::vec4 &color, const std::string &text, float max_line_length) {
    glm::mat4 line_start = model;
    glm::mat4 line = line_start;
    const glm::mat4 newline = glm::translate(glm::vec3(0.0f, -1.0f, 0.0f));
    float line_length = 0;
    std::vector<std::pair<std::string, float>> words;
    words.emplace_back("", 0.0f);
    for (char c : text) {
        std::pair<std::string, float> &last_word = words.back();
        if (c == ' ') {
            if (last_word.second > max_line_length) {
                throw std::runtime_error("Word in text is too long for box");
            }
            words.emplace_back("", 0.0f);
        } else {
            last_word.second += glm::length(model[0] * widths[c]);
            last_word.first.push_back(c);
        }
    }
    const float space_width = widths[' '];
    bool first_in_line = true;
    for (const auto &s : words) {
        const float word_length = s.second;
        const float space = !first_in_line ? glm::length(model[0] * space_width) : 0.0f;
        if (line_length + space + word_length > max_line_length) {
            line_start = line_start * newline;
            line = line_start;
            line_length = 0.0f;
            first_in_line = true;
        }
        if (!first_in_line){
            const glm::mat4 t = glm::translate(glm::vec3(space_width, 0.0f, 0.0f));
            const unsigned int i = ' ' & 0x0f;
            const unsigned int j = (' ' >> 4) & 0x0f;
            renderer.queue(line, color, i, j);
            line = line * t;
            line_length += glm::length(model[0] * space_width);
        }
        first_in_line = false;
        for (char c : s.first) {
            const float char_width = widths[c];
            const glm::mat4 t = glm::translate(glm::vec3(char_width, 0.0f, 0.0f));
            unsigned int i = c & 0x0f;
            unsigned int j = (c >> 4) & 0x0f;
            renderer.queue(line, color, i, j);
            line = line * t;
            line_length += glm::length(model[0] * char_width);
        }
    }
}

void Font::draw() {
    renderer.draw();
}

float Font::get_text_width(const std::string &s) const {
    float result = 0.0f;
    for (char c : s) {
        result += widths[c];
    }
    return result;
}

}
