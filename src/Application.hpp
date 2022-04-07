/* date = March 21 2022 9:27 am */

#ifndef MIDIFI_APPLICATION_HPP
#define MIDIFI_APPLICATION_HPP

struct GLFWwindow;

namespace MidiFi {
    
    struct Window {
        int width, height;
        GLFWwindow *glfw_back;
        const char *title;
    };

    void init();
    Window getWindow();
    // No scenes until UI stuff is finished
    //void setScene(Scene *s);
    //Scene &getScene();
    void loop();
}

#endif // MIDIFI_APPLICATION_HPP

