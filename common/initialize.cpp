#include "initialize.h"

#include <stdexcept>

namespace common {

initialize::initialize(int result, void (*quit)(), const char *(*get_error)()) : quit{quit} {
    if (result != 0) {
        throw std::runtime_error(std::string("Can't initialize SDL: ") + get_error());
    }
}

initialize::~initialize() {
    quit();
}

}
