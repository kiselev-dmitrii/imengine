#include "Window.h"
#include "../Utils/Debug.h"

namespace imEngine {

Window::Window() :
        m_window(nullptr),
        m_title("No title"),
        m_position(10,10),
        m_size(800, 600),
        m_prevSize(m_size),
        m_isVisible(true),
        m_isFullscreen(false),
        m_keyboard(nullptr),
        m_mouse(nullptr)
{ }

Window::~Window() {
        destroy();
}

bool Window::create() {
        if (!createWindow()) return false;
        if (!createContext()) return false;
        createInput();

        return true;
}

void Window::destroy() {
        if (m_context) SDL_GL_DeleteContext(m_context);
        if (m_window) SDL_DestroyWindow(m_window);
        if (m_keyboard) delete m_keyboard;
        if (m_mouse) delete m_mouse;

        m_context = nullptr;
        m_window = nullptr;
        m_keyboard = nullptr;
        m_mouse = nullptr;
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

IVec2 Window::position() {
        if (m_window) SDL_GetWindowPosition(m_window, &m_position.x, &m_position.y);
        return m_position;
}

void Window::setPosition(const IVec2 &position) {
        if (m_window) SDL_SetWindowPosition(m_window, position.x, position.y);
        m_position = position;
}

void Window::setPosition(int x, int y) {
        setPosition(IVec2(x,y));
}

IVec2 Window::size() {
        if (m_window) SDL_GetWindowSize(m_window, &m_size.x, &m_size.y);
        return m_size;
}

void Window::setSize(const IVec2 &size) {
        if (m_window) SDL_SetWindowSize(m_window, size.x, size.y);
        m_size = size;
}

void Window::setSize(int width, int height) {
        setSize(IVec2(width, height));
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

IVec2 Window::center() {
        return size()/2;
}

Keyboard* Window::keyboard() {
        return m_keyboard;
}

Mouse* Window::mouse() {
        return m_mouse;
}

SDL_Window* Window::rawWindow() {
        return m_window;
}

Vec2 Window::convertSSToCS(const Vec2& vec) {
        IVec2 winSize = size();
        return Vec2(-1.0 + 2.0*vec.x/winSize.x, 1.0 - 2.0*vec.y/winSize.y);
}

bool Window::createWindow() {
        Uint32 flags = SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE;
        if (m_isFullscreen) flags |= SDL_WINDOW_FULLSCREEN_DESKTOP;
        if (!m_isVisible) flags |= SDL_WINDOW_HIDDEN;

        m_window = SDL_CreateWindow(m_title.c_str(), m_position.x, m_position.y, m_size.x, m_size.y, flags);
        if (!m_window) {
                IM_ERROR("Could not create window. SDL: " << SDL_GetError());
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
                IM_ERROR("Could not create context. SDL: " << SDL_GetError());
                return false;
        }
        return true;
}

void Window::createInput() {
        IM_ASSERT(m_window);
        m_keyboard = new Keyboard();
        m_mouse = new Mouse(this);
}

} //namespace imEngine
