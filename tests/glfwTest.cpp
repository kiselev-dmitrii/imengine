#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <imCore/Debug/Debug.h>

int main() {

        if(!glewInit()) return -1;
        if(!glfwInit()) return -1;
        IM_SHOW(glfwGetTime());

        GLFWwindow* window = glfwCreateWindow(800, 600, "Title", NULL, NULL);
        if (!window) {
                glfwTerminate();
                return -1;
        }

        glfwMakeContextCurrent(window);


        while (!glfwWindowShouldClose(window)) {
                IM_GLCALL(glBindTexture(GL_TEXTURE_2D, 33));
                glfwSwapBuffers(window);
                glfwPollEvents();
        }

        glfwTerminate();
        return 0;
}
