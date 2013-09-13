#include <GL/glew.h>
#include <imCore/System/Window.h>
#include <imCore/Utils/Debug.h>
#include <SDL2/SDL.h>
#undef main

using namespace imCore;

bool isPressed(uint scancode) {
        const Uint8* state = SDL_GetKeyboardState(NULL);
        return state[scancode];
}

int main() {
        if (SDL_Init(SDL_INIT_VIDEO)) {
                IM_ERROR("Could not init SDL");
                return -1;
        }

        Window win;
        win.create();

        if (glewInit() != GLEW_OK) {
                IM_ERROR("Could not init GLEW");
                return -1;
        }

        glClearColor(1,0,0,0);

        SDL_Event event;
        bool running = true;
        while (running) {
                while (SDL_PollEvent(&event)) {
                        switch (event.type) {
                                case SDL_KEYDOWN:{
                                        SDL_Keycode key = event.key.keysym.sym;
                                        //IM_PRINT("Key pressed :" << char(event.key.keysym.sym));
                                        if (key == SDLK_EQUALS) win.setSize(win.size() + IVec2(1,1));
                                        if (key == SDLK_MINUS) win.setSize(win.size() + IVec2(-1,-1));
                                        if (key == SDLK_a) win.setPosition(win.position() + IVec2(-1,0));
                                        if (key == SDLK_d) win.setPosition(win.position() + IVec2(1,0));
                                        if (key == SDLK_w) win.setPosition(win.position() + IVec2(0,-1));
                                        if (key == SDLK_s) win.setPosition(win.position() + IVec2(0,1));
                                        if (key == SDLK_f) win.setFullscreen(!win.isFullscreen());
                                        if (key == SDLK_t) win.setTitle("WindowTest");
                                        if (key == SDLK_s) win.show();
                                        if (key == SDLK_h) win.hide();
                                        if (key == SDLK_m) SDL_WarpMouseInWindow(win.rawWindow(), win.center().x, win.center().y);

                                        if (key == SDLK_F1) {
                                                IM_PRINT(win.size().x << " " << win.size().y);
                                        }
                                }

                                        break;
                                case SDL_KEYUP:
                                        IM_PRINT("Key released :" << char(event.key.keysym.sym));
                                        break;
                                case SDL_MOUSEMOTION:
                                        IM_PRINT("Mouse moved :" << event.motion.x << " " << event.motion.y);
                                        break;
                                case SDL_MOUSEWHEEL:
                                        IM_PRINT("Mouse wheel :" << event.wheel.x << " " << event.wheel.y);
                                        break;
                                case SDL_QUIT:
                                        IM_PRINT("Close");
                                        running = false;
                                        break;
                        }
                }
                if (isPressed(SDL_SCANCODE_LEFT)) win.setPosition(win.position() + IVec2(-1,0));
                if (isPressed(SDL_SCANCODE_RIGHT)) win.setPosition(win.position() + IVec2(1,0));
                if (isPressed(SDL_SCANCODE_UP)) win.setPosition(win.position() + IVec2(0,-1));
                if (isPressed(SDL_SCANCODE_DOWN)) win.setPosition(win.position() + IVec2(0,1));
                glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
                SDL_GL_SwapWindow(win.rawWindow());
        }
        SDL_Quit();

        return 0;
}
