#include <imCore/System/Window.h>
#include <imCore/Debug/Debug.h>
#include <GLFW/glfw3.h>

using namespace imCore;

int main() {
        if (!glfwInit()) {
                IM_ERROR("Cannot initialize GLFW");
                return -1;
        }

        Window window;
        window.create();
        window.setPosition(30, 30);
        window.show();
        window.setTitle("WindowTest Application");

        while(!glfwWindowShouldClose(window.glfwWindow())) {
                GLFWwindow* win = window.glfwWindow();
                if (glfwGetKey(win, GLFW_KEY_T)) {
                        if (window.isVisible()) window.hide();
                        else window.show();
                }

                if (glfwGetKey(window.glfwWindow(), GLFW_KEY_F)) {
                        window.setFullscreen(!window.isFullscreen());
                }

                if (glfwGetKey(win, GLFW_KEY_LEFT)) window.setPosition(window.position() + ivec2(-1,0));
                if (glfwGetKey(win, GLFW_KEY_RIGHT)) window.setPosition(window.position() + ivec2(1,0));
                if (glfwGetKey(win, GLFW_KEY_DOWN)) window.setPosition(window.position() + ivec2(0,1));
                if (glfwGetKey(win, GLFW_KEY_UP)) window.setPosition(window.position() + ivec2(0,-1));

                if (glfwGetKey(win, GLFW_KEY_MINUS))  window.setSize(window.size() - ivec2(1));
                if (glfwGetKey(win, GLFW_KEY_EQUAL))  window.setSize(window.size() + ivec2(1));

                glfwSwapBuffers(win);
                glfwPollEvents();
        }

}
