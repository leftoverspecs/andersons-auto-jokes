#include "widget.h"

#include <SDL_mouse.h>
#include <iostream>

namespace engine {

Widget::Widget(float x, float y, float w, float h, float screen_height)
    : x{x}, y{y}, w{w}, h{h}, screen_height{screen_height}, left_mouse_button_down{false} {}

bool Widget::keypress(int scancode, char c) {
    return on_keypress(scancode, c);
}

void Widget::draw() {
    int mouse_x, mouse_y;
    int buttons = SDL_GetMouseState(&mouse_x, &mouse_y);
    mouse_y = screen_height - mouse_y;
    if (is_inside(mouse_x, mouse_y)) {
        std::cout << left_mouse_button_down << ", " << buttons << '\n';
        if (left_mouse_button_down) {
            if ((SDL_BUTTON(SDL_BUTTON_LEFT) & buttons) == 0) {
                on_clicked(mouse_x, mouse_y, buttons);
                left_mouse_button_down = false;
            }
        } else {
            if ((SDL_BUTTON(SDL_BUTTON_LEFT) & buttons) != 0) {
                left_mouse_button_down = true;
            }
        }
    } else {
        left_mouse_button_down = false;
    }
    on_draw(mouse_x, mouse_y);
}

void Widget::on_draw(float mouse_x, float mouse_y) {}
void Widget::on_clicked(float mouse_x, float mouse_y, int buttons) {}
bool Widget::on_keypress(int scancode, char c) { return false; }

bool Widget::is_inside(float test_x, float test_y) const {
    return test_x >= x && test_x <= x + w && test_y <= y && test_y >= y - h;
}

} // namespace engine
