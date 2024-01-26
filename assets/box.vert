#version 330 core
in vec2 position;
in vec4 color;

out vec4 frag_color;

uniform float width;
uniform float height;

void main() {
    frag_color = color;
    gl_Position = vec4(2 * position.x / width - 1, 2 * position.y / height - 1, 0, 1);
}
