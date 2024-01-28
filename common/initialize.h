#ifndef GGJ2024_INITIALIZE_H
#define GGJ2024_INITIALIZE_H

#include <cstdint>

namespace common {

class Initialize {
public:
    Initialize(int result, void (*quit)(), const char *(*get_error)());
    ~Initialize();
private:
    void (*quit)();
};

}

#endif //GGJ2024_INITIALIZE_H
