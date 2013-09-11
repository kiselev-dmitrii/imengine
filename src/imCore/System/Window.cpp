#include "Window.h"
#include "../Debug/Debug.h"

namespace imCore {

Window::Window() :
        m_window(nullptr),
        m_title("No title"),
        m_position(0,0),
        m_size(800, 600),
        m_isFullscreen(false)
{
        m_context.majorVersion = 3;
        m_context.minorVersion = 3;
        m_context.profile = GLFW_OPENGL_CORE_PROFILE;
        m_context.samples = 4;
}

Window::~Window() {
        if (m_window) glfwDestroyWindow(m_window);
}

void Window::show() {
        if (!m_window) createWindow();
        glfwShowWindow(m_window);
}

void Window::hide() {
        if (!m_window) createWindow();
        glfwHideWindow(m_window);
}

void Window::setTitle(const String &title) {
        if (m_window) glfwSetWindowTitle(m_window, title.c_str());
        m_title = title;
}

String Window::title() {
        return m_title;
}

void Window::setPosition(const ivec2 &position) {
        if (m_window) glfwSetWindowPos(m_window, position.x, position.y);
        m_position = position;
}

ivec2 Window::position() {
        if (m_window) glfwGetWindowPos(m_window, &m_position.x, &m_position.y);
        return m_position;
}

void Window::setFullscreen(bool enable) {
        m_isFullscreen = enable;
        if (m_window) {
                glfwDestroyWindow(m_window);
                createWindow();
        }
}

bool Window::fullscreen() {
        return m_isFullscreen;
}

void Window::setSize(const ivec2 &size) {
        if (m_window) glfwSetWindowSize(m_window, size.x, size.y);
        m_size = size;
}

ivec2 Window::size() {
        if (m_window) glfwGetWindowSize(m_window, &m_size.x, &m_size.y);
        return m_size;
}

ivec2 Window::framebufferSize() {
        ivec2 size(0,0);

        if (m_window) glfwGetFramebufferSize(m_window, &size.x, &size.y);
        else IM_ERROR("Attempt to get framebuffer size without window");
        return size;
}

GLFWwindow* Window::rawWindow() {
        return m_window;
}

void Window::createWindow() {
        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, m_context.majorVersion);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, m_context.minorVersion);
        glfwWindowHint(GLFW_OPENGL_PROFILE, m_context.profile);
        glfwWindowHint(GLFW_SAMPLES, m_context.samples);

        m_window = glfwCreateWindow(m_size.x, m_size.y, m_title.c_str(), m_isFullscreen ? glfwGetPrimaryMonitor() : NULL, NULL);
        if (!m_window) {
                IM_ERROR("Cannot create window");
                glfwTerminate();
        }
        glfwSetWindowPos(m_window, m_position.x, m_position.y);
}

} //namespace imCore
