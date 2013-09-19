#include <GL/glew.h>
#include <IL/il.h>
#include <IL/ilu.h>
#include "BaseApplication.h"
#include "../Utils/Debug.h"

namespace imCore {

int BaseApplication::exec() {
        if (!init()) return -1;
        loop();
        quit();
        return 0;
}

bool BaseApplication::init() {
        if (!initSDL()) return false;
        if (!createMainWindow()) return false;
        if (!initGLEW()) return false;
        if (!initDevIL()) return false;

        initialize();

        return true;
}

bool BaseApplication::initSDL() {
        if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_TIMER)) {
                IM_ERROR("Could not init SDL: " << SDL_GetError());
                return false;
        }
        IM_LOG("SDL initialized");
        return true;
}

bool BaseApplication::createMainWindow() {
        if (!m_mainWindow.create()) return false;
        IM_LOG("Main window created");
        return true;
}

bool BaseApplication::initGLEW() {
        glewExperimental = GL_TRUE;
        GLenum glewError = glewInit();
        if (glewError != GLEW_OK) {
                IM_ERROR("Could not init GLEW: " << glewGetErrorString(glewError));
                return false;
        }
        IM_LOG("GLEW initialized. Version " << glewGetString(GLEW_VERSION))
        return true;
}

bool BaseApplication::initDevIL() {
        // Проверка версии библиотек
        if (ilGetInteger(IL_VERSION_NUM) < IL_VERSION) {
                IM_ERROR("Incorect DevIL version");
                return false;
        }
        if (iluGetInteger(ILU_VERSION_NUM) < ILU_VERSION) {
                IM_ERROR("Incorect ILU version");
                return false;
        }

        // Собствено инициализация
        ilInit();
        iluInit();
        IM_LOG("DevIL and ILU initialized");

        return true;
}

void BaseApplication::loop() {
        bool running = true;

        SDL_Event event;
        while (running) {
                while (SDL_PollEvent(&event)) {
                        switch (event.type) {
                                // Обработка событий
                                case SDL_KEYDOWN:
                                        keyPressEvent(event.key.keysym.sym);
                                        break;
                                case SDL_KEYUP:
                                        keyReleaseEvent(event.key.keysym.sym);
                                        break;
                                case SDL_MOUSEBUTTONDOWN:
                                        mousePressEvent(event.button.x, event.button.y, event.button.button);
                                        break;
                                case SDL_MOUSEBUTTONUP:
                                        mouseReleaseEvent(event.button.x, event.button.y, event.button.button);
                                        break;
                                case SDL_MOUSEWHEEL:
                                        mouseWheelEvent(event.wheel.y, event.wheel.x);
                                        break;

                                case SDL_WINDOWEVENT:
                                        switch (event.window.event) {
                                                case SDL_WINDOWEVENT_RESIZED:
                                                        windowResizeEvent(event.window.data1, event.window.data2);
                                                        break;
                                                case SDL_WINDOWEVENT_MINIMIZED:
                                                        windowMinimizeEvent();
                                                        break;
                                        }
                                        break;

                                case SDL_QUIT:
                                        running = false;
                                        break;
                        }
                }

                update();
                render();

                SDL_GL_SwapWindow(m_mainWindow.rawWindow());
        }
}

void BaseApplication::quit() {
        destroy();

        m_mainWindow.destroy();
        SDL_Quit();
}

Window* BaseApplication::mainWindow() {
        return &m_mainWindow;
}

} //namespace imCore
