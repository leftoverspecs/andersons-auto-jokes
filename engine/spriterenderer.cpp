#include "spriterenderer.h"

#include "spritemap.h"
#include "texture.h"

#include <stdexcept>

#include <sprite.vert.h>
#include <sprite.frag.h>

namespace engine {

namespace {

const std::vector<GLfloat>::size_type CAPACITY = 10000 * 8;
const int ATTRIBUTE_POSITION = 0;
const int ATTRIBUTE_COLOR = 1;
const int ATTRIBUTE_TEXTURE_COORD = 2;

}

SpriteRenderer::SpriteRenderer(const SpriteMap &map, GLfloat width, GLfloat height)
  : map(map)
{
    shader.attach(Shader(GL_VERTEX_SHADER, reinterpret_cast<const char *>(sprite_vert), sizeof(sprite_vert)));
    shader.attach(Shader(GL_FRAGMENT_SHADER, reinterpret_cast<const char *>(sprite_frag), sizeof(sprite_frag)));
    shader.bind(ATTRIBUTE_POSITION, "position");
    shader.bind(ATTRIBUTE_COLOR, "color");
    shader.bind(ATTRIBUTE_TEXTURE_COORD, "texture_coord");
    shader.link();

    {
        auto usage = shader.use();
        usage.set_uniform("width", width);
        usage.set_uniform("height", height);
        usage.set_uniform("sprite_texture", 0);
    }
    {
        auto binding = vao.bind();
        binding.enable_attribute(ATTRIBUTE_POSITION);
        binding.enable_attribute(ATTRIBUTE_COLOR);
        binding.enable_attribute(ATTRIBUTE_TEXTURE_COORD);

        auto buffer_binding = vertex_buffer.bind(GL_ARRAY_BUFFER);
        buffer_binding.vertex_attrib_pointer(binding, ATTRIBUTE_POSITION, 2, GL_FLOAT, GL_FALSE, sizeof(GLfloat) * 8, (void *)(0 * sizeof(GLfloat)));
        buffer_binding.vertex_attrib_pointer(binding, ATTRIBUTE_COLOR, 4, GL_FLOAT, GL_FALSE, sizeof(GLfloat) * 8, (void *)(2 * sizeof(GLfloat)));
        buffer_binding.vertex_attrib_pointer(binding, ATTRIBUTE_TEXTURE_COORD, 2, GL_FLOAT, GL_FALSE, sizeof(GLfloat) * 8, (void *)(6 * sizeof(GLfloat)));
        buffer_binding.data(CAPACITY * sizeof(GLfloat), nullptr, GL_DYNAMIC_DRAW);
    }
    batch.reserve(CAPACITY);
}

void SpriteRenderer::clear() {
    batch.clear();
}

void SpriteRenderer::queue(GLfloat x, GLfloat y, GLfloat width, GLfloat height,
                           GLfloat r, GLfloat g, GLfloat b, GLfloat a,
                           unsigned int i, unsigned int j) {
    SpriteMap::TextureRect rect = map.get_sprite(i, j);
    const GLfloat data[] = {
        x,         y,          r, g, b, a, rect.u_min, rect.v_max,
        x + width, y,          r, g, b, a, rect.u_max, rect.v_max,
        x + width, y + height, r, g, b, a, rect.u_max, rect.v_min,

        x,         y,          r, g, b, a, rect.u_min, rect.v_max,
        x,         y + height, r, g, b, a, rect.u_min, rect.v_min,
        x + width, y + height, r, g, b, a, rect.u_max, rect.v_min,
    };
    if (batch.size() + 6 * 8 >= CAPACITY) {
        throw std::runtime_error("Can't queue any further sprite");
    }
    batch.insert(batch.end(), data, data + sizeof(data) / sizeof(data[0]));
}

void SpriteRenderer::draw() const {
    auto usage = shader.use();
    auto texture_binding = map.bind(GL_TEXTURE0, GL_TEXTURE_2D);
    auto binding = vao.bind();
    auto buffer_binding = vertex_buffer.bind(GL_ARRAY_BUFFER);
    buffer_binding.subdata(0, batch.size() * sizeof(GLfloat), batch.data());
    //void *data = glMapBuffer(GL_ARRAY_BUFFER, GL_READ_ONLY);
    //glUnmapBuffer(GL_ARRAY_BUFFER);
    glDrawArrays(GL_TRIANGLES, 0, static_cast<GLsizei>(batch.size()) / 8);
}

}
