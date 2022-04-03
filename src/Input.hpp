/* date = March 26 2022 12:30 pm */

#ifndef MIDIFI_INPUT_HPP
#define MIDIFI_INPUT_HPP

#include <glm/glm.hpp>
// forward declarations
struct GLFWwindow;

namespace MidiFi {
    /**
     * Checks if mouse button is pressed.
     */
    bool mouseButtonPressed(int button);
    /**
     * Checks if key is pressed.
     */
    bool keyPressed(int key);

    /**
     * Gets current position of mouse.
     */
    glm::vec2 mousePos();
    /**
     * Gets current change in position, namely:<br>
     * `glm::vec2{x - lastx, y - lasty}`
     */
    glm::vec2 mouseDrag();
    /**
     * Gets current change in mouse scroll.
     */
    glm::vec2 mouseScroll();
    /**
     * Updates frame, setting (lastx, lasty) to (x, y) respectively &
     * setting current scroll position to (0, 0).
     */
    void endFrame();

    // glfw callbacks

    void _GLFW_mousePos(GLFWwindow *w, double x, double y);
    void _GLFW_mouseButton(GLFWwindow *w, int button, int action, int mods);
    void _GLFW_mouseScroll(GLFWwindow *w, double x, double y);
    void _GLFW_keyPress(GLFWwindow *w, int key, int scancode, int action, int mods);
}

#endif // MIDIFI_INPUT_HPP

