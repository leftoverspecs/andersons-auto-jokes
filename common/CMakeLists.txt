add_library(common
        client.cpp
        client.h
        initialize.h
        initialize.cpp
        message.h
        message.cpp
        transmit.h
        transmit.cpp
        stats.cpp
        stats.h
        primitives.cpp
        primitives.h
)
target_include_directories(common PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
target_link_libraries(common PUBLIC
        SDL2::SDL2
        SDL2_net::SDL2_net)
