#ifndef GGJ2024_INITIALIZE_H
#define GGJ2024_INITIALIZE_H

#include <cstdint>

namespace common {

class initialize {
public:
    initialize(int result, void (*quit)(), const char *(*get_error)());
    ~initialize();
private:
    void (*quit)();
};

}

#endif //GGJ2024_INITIALIZE_H
