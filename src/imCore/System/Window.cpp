#include "Window.h"
#include "../Debug/Debug.h"

namespace imCore {

Window::Window() :
        m_window(nullptr),
        m_title("No title"),
        m_position(0,0),
        m_size(800, 600),
        m_isVisible(false),
        m_isFullscreen(false)
{ }

Window::~Window() {
        destroy();
}

bool Window::create() {
        glfwWindowHint(GLFW_VISIBLE, m_isVisible);
        m_window = glfwCreateWindow(m_size.x, m_size.y, m_title.c_str(), m_isFullscreen ? glfwGetPrimaryMonitor() : NULL, NULL);
        if (!m_window) {
                IM_ERROR("Cannot create window");
                return false;
        }
        glfwSetWindowPos(m_window, m_position.x, m_position.y);
        glfwMakeContextCurrent(m_window);       //вновь созданное окно берет на себя контекст
        return true;
}

void Window::show() {
        if (m_window) glfwShowWindow(m_window);
        m_isVisible = true;
}

void Window::hide() {
        if (m_window) glfwHideWindow(m_window);
        m_isVisible = false;
}

bool Window::isVisible() {
        return m_isVisible;
}

String Window::title() {
        return m_title;
}

void Window::setTitle(const String &title) {
        if (m_window) glfwSetWindowTitle(m_window, title.c_str());
        m_title = title;
}

ivec2 Window::position() {
        if (m_window) glfwGetWindowPos(m_window, &m_position.x, &m_position.y);
        return m_position;
}

void Window::setPosition(const ivec2 &position) {
        if (m_window) glfwSetWindowPos(m_window, position.x, position.y);
        m_position = position;
}

void Window::setPosition(int x, int y) {
        setPosition(ivec2(x,y));
}

void Window::setSize(const ivec2 &size) {
        if (m_window) glfwSetWindowSize(m_window, size.x, size.y);
        m_size = size;
}

void Window::setSize(int width, int height) {
        setSize(ivec2(width, height));
}

ivec2 Window::size() {
        if (m_window) glfwGetWindowSize(m_window, &m_size.x, &m_size.y);
        return m_size;
}

void Window::setFullscreen(bool isFullscreen) {
        m_isFullscreen = isFullscreen;
        if (m_window) {
                destroy();
                create();
        }
}

bool Window::isFullscreen() {
        return m_isFullscreen;
}

ivec2 Window::center() {
        return size()/2;
}

GLFWwindow* Window::glfwWindow() {
        return m_window;
}

void Window::destroy() {
        if (m_window) {
                glfwMakeContextCurrent(m_window);
                glfwDestroyWindow(m_window);
        }
}

} //namespace imCore
