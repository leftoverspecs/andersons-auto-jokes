#include "background.h"

#include <glm/gtc/matrix_transform.hpp>

namespace game {

Background::Background(float width, float height, const unsigned char *image, std::size_t image_size)
  : width(width),
    height(height),
    sprite(image, image_size, 1, 1),
    renderer(sprite, width, height)
{ }

void Background::draw() {
    renderer.clear();

    glm::mat4 model(1.0f);
    model = glm::translate(model, glm::vec3(0.0f, 0.0f, 0.0f));
    model = glm::scale(model, glm::vec3(width, height, 1.0f));
    renderer.queue(model, glm::vec4(1.0f, 1.0f, 1.0f, 1.0f), 0, 0);
    renderer.draw();
}

}
