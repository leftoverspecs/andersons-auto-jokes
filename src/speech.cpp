#include "speech.h"

#include <speech.png.h>

#include <glm/gtc/matrix_transform.hpp>

namespace game {

Speech::Speech(int width, int height)
    : map{speech, sizeof(speech), 4, 1},
      renderer(map, width, height) {}

void Speech::clear() {
    renderer.clear();
}

void Speech::queue(float x, float y, bool flipped, int index) {
    renderer.clear();
    glm::mat4 model(1.0f);
    model = glm::translate(model, glm::vec3(x, y, 0.0f));
    model = glm::scale(model, glm::vec3(flipped ? -256.0f : 256.0f, 128.0f, 0.0f));
    renderer.queue(model, glm::vec4(1.0f, 1.0f, 1.0f, 1.0f), index, 0);
}

void Speech::draw() {
    renderer.draw();
}

}// namespace game