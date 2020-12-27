#pragma once
#include <SDL2/SDL.h>
#include "layouts.hpp"
#include "Game.h"

class Application
{
private:
    SDL_Window* window;
    SDL_Renderer* renderer;
    Layout* layout;
public:
    int fps = 0;
    bool quit = false;
    Application() {
        SDL_Init(SDL_INIT_EVERYTHING);
        TTF_Init();
        window = SDL_CreateWindow("Dry Death", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 640, 480, SDL_WINDOW_OPENGL);
        renderer = SDL_CreateRenderer(window, -1, 0);
        globalRender = renderer;
        globalQuit = &quit;
        globalFont = TTF_OpenFont("assets/dogica.ttf", 16);
        cout << TTF_GetError();
        layout = new Menu();
        globalLayout = &layout;
    }
    ~Application() {
        SDL_DestroyWindow(window);
    }
    void Loop(){
        SDL_RenderClear(renderer);
        layout->Render();
        SDL_RenderPresent(renderer);
    }

    void InputHandle(SDL_Event* e){
        layout->InputHandle(e);
    }

    
};

