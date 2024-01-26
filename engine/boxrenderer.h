#ifndef GGJ2024_BOXRENDERER_H
#define GGJ2024_BOXRENDERER_H

#include "buffer.h"
#include "program.h"
#include "vertexarray.h"

#include <vector>

namespace engine {

class BoxRenderer {
public:
    BoxRenderer(float screen_width, float screen_height);

    void clear();
    void queue_frame(const glm::mat4 &model, const glm::vec4 &color);
    void draw();
private:
    Program shader;
    VertexArray vao;
    Buffer vertex_buffer;
    std::vector<GLfloat> batch;
};

}// namespace engine

#endif//GGJ2024_BOXRENDERER_H
