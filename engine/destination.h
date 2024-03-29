#pragma once

#include "buffer.h"
#include "framebuffer.h"
#include "program.h"
#include "renderbuffer.h"
#include "texture.h"
#include "vertexarray.h"

#include <glm/mat4x4.hpp>
#include <glm/vec3.hpp>

#include <stdexcept>

namespace engine {

class Destination {
public:
    class Exception : public std::runtime_error {
    public:
        Exception(const char *message) : std::runtime_error(message) { }
    };

    Destination(GLsizei screen_width, GLsizei screen_height);

    Framebuffer::Binding bind_as_target() const;

    void draw(const glm::mat4x4 &projection, const glm::vec3 &color, float alpha, const glm::vec3 &factor) const;

    void set_gamma(float gamma);
    void set_exposure(float exposure);

    float get_gamma() const { return gamma; }
    float get_exposure() const { return exposure; }

    GLsizei get_screen_width() const;
    GLsizei get_screen_height() const;

private:
    Program shader;
    GLsizei screen_width;
    GLsizei screen_height;
    Framebuffer framebuffer;
    Texture destination;
    Renderbuffer depth_stencil;
    VertexArray vao;
    Buffer screen_vertices;
    float gamma;
    float exposure;
};

}
