#ifndef GGJ2024_WIDGET_H
#define GGJ2024_WIDGET_H

namespace engine {

class Widget {
public:
    Widget(float x, float y, float w, float h, float screen_height);

    bool keypress(int scancode, char c);
    void draw();

    [[nodiscard]] bool is_inside(float test_x, float test_y) const;

    [[nodiscard]] float get_x() const { return x; }
    [[nodiscard]] float get_y() const { return y; }
    [[nodiscard]] float get_w() const { return w; }
    [[nodiscard]] float get_h() const { return h; }
private:
    float x;
    float y;
    float w;
    float h;
    float screen_height;
    bool left_mouse_button_down;

    virtual void on_draw(float mouse_x, float mouse_y);
    virtual void on_clicked(float mouse_x, float mouse_y, int buttons);
    virtual bool on_keypress(int scancode, char c);
};

}// namespace engine

#endif//GGJ2024_WIDGET_H
