#include "boxrenderer.h"

#include <box.frag.h>
#include <box.vert.h>

namespace engine {

namespace {

const std::vector<GLfloat>::size_type CAPACITY = 100 * 6;
const int ATTRIBUTE_POSITION = 0;
const int ATTRIBUTE_COLOR = 1;

}

BoxRenderer::BoxRenderer(float screen_width, float screen_height)
{
    shader.attach(Shader(GL_VERTEX_SHADER, reinterpret_cast<const char *>(box_vert), sizeof(box_vert)));
    shader.attach(Shader(GL_FRAGMENT_SHADER, reinterpret_cast<const char *>(box_frag), sizeof(box_frag)));
    shader.bind(ATTRIBUTE_POSITION, "position");
    shader.bind(ATTRIBUTE_COLOR, "color");
    shader.link();

    {
        auto usage = shader.use();
        usage.set_uniform("width", screen_width);
        usage.set_uniform("height", screen_height);
    }
    {
        auto binding = vao.bind();
        binding.enable_attribute(ATTRIBUTE_POSITION);
        binding.enable_attribute(ATTRIBUTE_COLOR);

        auto buffer_binding = vertex_buffer.bind(GL_ARRAY_BUFFER);
        buffer_binding.vertex_attrib_pointer(binding, ATTRIBUTE_POSITION, 2, GL_FLOAT, GL_FALSE, sizeof(GLfloat) * 6, (void *)(0 * sizeof(GLfloat)));
        buffer_binding.vertex_attrib_pointer(binding, ATTRIBUTE_COLOR, 4, GL_FLOAT, GL_FALSE, sizeof(GLfloat) * 6, (void *)(2 * sizeof(GLfloat)));
        buffer_binding.data(CAPACITY * sizeof(GLfloat), nullptr, GL_DYNAMIC_DRAW);
    }
    batch.reserve(CAPACITY);
}

void BoxRenderer::clear() {
    batch.clear();
}

void BoxRenderer::queue_frame(const glm::mat4 &model, const glm::vec4 &color) {
    const glm::vec4 a = model * glm::vec4(0.0f, 0.0f, 0.0f, 1.0f);
    const glm::vec4 b = model * glm::vec4(1.0f, 0.0f, 0.0f, 1.0f);
    const glm::vec4 c = model * glm::vec4(0.0f, 1.0f, 0.0f, 1.0f);
    const glm::vec4 d = model * glm::vec4(1.0f, 1.0f, 0.0f, 1.0f);

    const GLfloat data[] = {
            a.x, a.y, color.r, color.g, color.b, color.a,
            b.x, b.y, color.r, color.g, color.b, color.a,
            d.x, d.y, color.r, color.g, color.b, color.a,
            a.x, a.y, color.r, color.g, color.b, color.a,
            c.x, c.y, color.r, color.g, color.b, color.a,
            d.x, d.y, color.r, color.g, color.b, color.a };
    const size_t size = sizeof(data) / sizeof(data[0]);
    if (batch.size() + size >= CAPACITY) {
        throw std::runtime_error("Can't queue any further boxes");
    }
    batch.insert(batch.end(), data, data + size);
}

void BoxRenderer::draw() {
    auto usage = shader.use();
    auto binding = vao.bind();
    auto buffer_binding = vertex_buffer.bind(GL_ARRAY_BUFFER);
    buffer_binding.subdata(0, batch.size() * sizeof(GLfloat), batch.data());
    //void *d = glMapBuffer(GL_ARRAY_BUFFER, GL_READ_ONLY);
    //glUnmapBuffer(GL_ARRAY_BUFFER);
    glDrawArrays(GL_TRIANGLES, 0, static_cast<GLsizei>(batch.size()) / 6);
}

} // namespace engine
