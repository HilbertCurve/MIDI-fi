#include "Application.hpp"

#include <cstdio>
#include <GL/gl.h>
#include <GLFW/glfw3.h>

#include "Input.hpp"
#include "Memory.hpp"
#include "Renderer.hpp"
#include "Utils.hpp"

namespace MidiFi {
    static void printError(int error, const char *description) {
        m_log_warning("GLFW error code %d: %s", error, description);
    }

    static Window window;

    Window getWindow() {
        return window;
    }

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
    
        glfwSetCursorPosCallback(window.glfw_back, _GLFW_mousePos);
        glfwSetScrollCallback(window.glfw_back, _GLFW_mouseScroll);
        glfwSetMouseButtonCallback(window.glfw_back, _GLFW_mouseButton);

        glfwSetKeyCallback(window.glfw_back, _GLFW_keyPress);

        glfwShowWindow(window.glfw_back);

        glViewport(0, 0, window.width, window.height);
        // transparency stuff
        glEnable(GL_BLEND);
        glBlendFunc(GL_ONE, GL_ONE_MINUS_SRC_ALPHA);

        // initialize program
        // memory


        startRenderer();
        // print pc/gl info
    }

    void loop() {
        while (!glfwWindowShouldClose(window.glfw_back)) {
            // poll mouse/keyboard/window events
            glfwPollEvents();
            // set default background
            glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
            glClear(GL_COLOR_BUFFER_BIT);

            if (keyPressed(GLFW_KEY_D)) {
                m_log_message("Key 'd' is pressed");
            }

            updateRenderer();

            glfwSwapBuffers(window.glfw_back);

            endFrame();
        }


        glfwDestroyWindow(window.glfw_back);
        glLinkProgram(0);
        glfwTerminate();

        m_log_memory_usage(true);
    }
} // MidiFi

