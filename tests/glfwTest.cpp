#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <imCore/Debug/Debug.h>

int main() {

        if(!glfwInit()) return -1;
        if(!glewInit()) IM_PRINT("BAD");

        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);
        glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
        GLFWwindow* window = glfwCreateWindow(800, 600, "Title", NULL, NULL);
        if (!window) {
                glfwTerminate();
                return -1;
        }

        glfwMakeContextCurrent(window);

        IM_SHOW(glfwGetWindowAttrib(window, GLFW_CONTEXT_VERSION_MAJOR));
        IM_SHOW(glfwGetWindowAttrib(window, GLFW_CONTEXT_VERSION_MINOR));
        IM_SHOW(glfwGetWindowAttrib(window, GLFW_OPENGL_PROFILE));


        while (!glfwWindowShouldClose(window)) {
                glfwSwapBuffers(window);
                glfwPollEvents();
        }

        glfwTerminate();
        return 0;
}
