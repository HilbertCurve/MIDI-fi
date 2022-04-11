#include "Input.hpp"

#include <glm/glm.hpp>
#include <GLFW/glfw3.h>

#define M_BUTTONS 6
#define M_FLOATS 6
#define K_KEYS 350

namespace MidiFi {
namespace Input {
    static bool _mButtons[M_BUTTONS];
    static bool _kKeys[K_KEYS];
    static float x = 0, y = 0, lastx = 0, lasty = 0, scrollx = 0, scrolly = 0;

    bool mouseButtonPressed(int button) {
        return _mButtons[button];
    }

    bool keyPressed(int key) {
        return _kKeys[key];
    }

    glm::vec2 mousePos() {
        return {x, y};
    }

    glm::vec2 mouseDrag() {
        return {x - lastx, y - lasty};
    }

    glm::vec2 mouseScroll() {
        return {scrollx, scrolly};
    }

    void endFrame() {
        lastx = x;
        lasty = y;

        scrollx = 0;
        scrolly = 0;
    }

    void _GLFW_mousePos(GLFWwindow *w, double nx, double ny) {
        x = nx;
        y = ny;
    }
    void _GLFW_mouseButton(GLFWwindow *w, int button, int action, int mods) {
        if (action == GLFW_PRESS && button < M_BUTTONS) {
            _mButtons[button] = true;
        } else if (action == GLFW_RELEASE && button < M_BUTTONS) {
            _mButtons[button] = false;
        }
    }
    void _GLFW_mouseScroll(GLFWwindow *w, double x, double y) {
        scrollx = x;
        scrolly = y;
    }

    void _GLFW_keyPress(GLFWwindow *w, int key, int scancode, int action, int mods) {
        if (action == GLFW_PRESS) {
            _kKeys[key] = true;
        } else if (action == GLFW_RELEASE) {
            _kKeys[key] = false;
        }
    }
}
}

