#include "Application.hpp"

#include <cstdio>
#include <GLFW/glfw3.h>

#include "Utils.hpp"

namespace MidiFi {

static void printError(int error, const char *description) {
    m_log_warning("GLFW error code %d: %s", error, description);
}

struct Window {
    int width, height;
    GLFWwindow *glfw_back;
    const char *title;
};
static Window window;

void init() {
    glfwSetErrorCallback(printError);

    m_assert(glfwInit(), "Couldn't initialize GLFW.");

    glfwDefaultWindowHints();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_VISIBLE, GLFW_FALSE);
    glfwWindowHint(GLFW_RESIZABLE, GLFW_TRUE);
    glfwWindowHint(GLFW_MAXIMIZED, GLFW_FALSE);
    
    glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, GLFW_TRUE);

    window = {600, 400, nullptr, "MIDI-fi"};

    window.glfw_back = glfwCreateWindow(window.width, window.height, window.title, NULL, NULL);

    m_assert(window.glfw_back, "Could not initialize window.");

    glfwMakeContextCurrent(window.glfw_back);

    // v-sync
    glfwSwapInterval(1);

    // setup callbacks
    glfwSetWindowSizeCallback(window.glfw_back, [](GLFWwindow *r, int newWidth, int newHeight)
                                {
                                    window.width = newWidth;
                                    window.height = newHeight;
                                    glViewport(0, 0, window.width, window.height);
                                });
    
    // TODO: mouse/keyboard callbacks
    /*
    glfwSetCursorPosCallback(window.glfw_back, mousePosCallback);
    glfwSetScrollCallback(window.glfw_back, mouseScrollCallback);
    glfwSetMouseButtonCallback(window.glfw_back, mouseButtonCallback);

    glfwSetKeyCallback(window.glfw_back, keyPressedCallback);
    */

    glfwShowWindow(window.glfw_back);

    glViewport(0, 0, window.width, window.height);
    // transparency stuff
    glEnable(GL_BLEND);
    glBlendFunc(GL_ONE, GL_ONE_MINUS_SRC_ALPHA);

    // initialize program


    // print pc/gl info
}

void loop() {
    while (!glfwWindowShouldClose(window.glfw_back)) {
        
        // poll mouse/keyboard/window events
        glfwPollEvents();

        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        
        // set default background
        glClear(GL_COLOR_BUFFER_BIT);


        glfwSwapBuffers(window.glfw_back);
    }

    glfwDestroyWindow(window.glfw_back);
    glLinkProgram(0);
    glfwTerminate();
}

}
