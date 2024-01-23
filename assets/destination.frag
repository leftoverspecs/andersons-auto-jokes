#version 330 core
in vec2 frag_position;
in vec2 frag_texture_coord;

uniform sampler2D source_texture;
uniform vec3 effect;
uniform vec3 factor;
uniform float alpha;
uniform float gamma;
uniform float exposure;

out vec4 FragColor;

void main() {
    vec3 color = texture(source_texture, frag_texture_coord).rgb + (0.1 * dot(frag_position, frag_position) * effect);
    vec3 result = vec3(1.0) - exp(-(color * factor) * exposure);
    result = pow(result, vec3(1.0 / gamma));
    FragColor = vec4(result, alpha);
}
