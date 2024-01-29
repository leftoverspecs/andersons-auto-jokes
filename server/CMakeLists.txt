add_executable(server server.cpp)
target_link_libraries(server common SDL2_net::SDL2_net SDL2::SDL2 SDL2::SDL2main)

add_executable(test_client test_client.cpp)
target_link_libraries(test_client common SDL2_net::SDL2_net SDL2::SDL2 SDL2::SDL2main)
