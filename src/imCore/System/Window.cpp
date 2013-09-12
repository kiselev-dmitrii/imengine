#include "Window.h"
#include "../Debug/Debug.h"

namespace imCore {

Window::Window() :
        m_window(nullptr),
        m_title("No title"),
        m_position(0,0),
        m_size(800, 600),
        m_prevSize(m_size),
        m_isVisible(true),
        m_isFullscreen(false)
{ }

Window::~Window() {
        destroy();
}

bool Window::create() {
        if (!createWindow()) return false;
        return createContext();
}

bool Window::isVisible() {
        return m_isVisible;
}

void Window::show() {
        if (m_window) SDL_ShowWindow(m_window);
        m_isVisible = true;
}

void Window::hide() {
        if (m_window) SDL_HideWindow(m_window);
        m_isVisible = false;
}

String Window::title() {
        return m_title;
}

void Window::setTitle(const String &title) {
        if (m_window) SDL_SetWindowTitle(m_window, title.c_str());
        m_title = title;
}

ivec2 Window::position() {
        if (m_window) SDL_GetWindowPosition(m_window, &m_position.x, &m_position.y);
        return m_position;
}

void Window::setPosition(const ivec2 &position) {
        if (m_window) SDL_SetWindowPosition(m_window, position.x, position.y);
        m_position = position;
}

void Window::setPosition(int x, int y) {
        setPosition(ivec2(x,y));
}

ivec2 Window::size() {
        if (m_window) SDL_GetWindowSize(m_window, &m_size.x, &m_size.y);
        return m_size;
}

void Window::setSize(const ivec2 &size) {
        if (m_window) SDL_SetWindowSize(m_window, size.x, size.y);
        m_size = size;
}

void Window::setSize(int width, int height) {
        setSize(ivec2(width, height));
}

void Window::setFullscreen(bool isFullscreen) {
        if (m_window) {
                if (isFullscreen) {
                        m_prevSize = size();
                        SDL_SetWindowFullscreen(m_window, SDL_WINDOW_FULLSCREEN_DESKTOP);
                } else {
                        SDL_SetWindowFullscreen(m_window, 0);
                        setSize(m_prevSize);
                }
        }
        m_isFullscreen = isFullscreen;
}

bool Window::isFullscreen() {
        return m_isFullscreen;
}

ivec2 Window::center() {
        return size()/2;
}

SDL_Window* Window::rawWindow() {
        return m_window;
}

bool Window::createWindow() {
        Uint32 flags = SDL_WINDOW_OPENGL;
        if (m_isFullscreen) flags |= SDL_WINDOW_FULLSCREEN_DESKTOP;
        if (!m_isVisible) flags |= SDL_WINDOW_HIDDEN;

        m_window = SDL_CreateWindow(m_title.c_str(), m_position.x, m_position.y, m_size.x, m_size.y, flags);
        if (!m_window) {
                IM_ERROR("Could not create window");
                IM_ERROR(SDL_GetError());
                return false;
        }
        return true;
}

bool Window::createContext() {
        SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
        SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);
        SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
        SDL_GL_SetAttribute(SDL_GL_MULTISAMPLESAMPLES, 4);
        SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
        m_context = SDL_GL_CreateContext(m_window);
        if (!m_context) {
                IM_ERROR("Could not create context");
                IM_ERROR(SDL_GetError());
                return false;
        }
        return true;
}

void Window::destroy() {
        if (m_context) SDL_GL_DeleteContext(m_context);
        if (m_window) SDL_DestroyWindow(m_window);
}

} //namespace imCore
