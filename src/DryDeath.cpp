#include <iostream>
#include <SDL2/SDL.h>
#include "layouts.hpp"
#include "app.hpp"
#include "Game.h"
using namespace std;

int main(int argc, char const *argv[])
{

    Application* app = new Application();
    int DelayTime = 1000 / globalFPS;

    Uint32 StartTime;
    Uint32 FrameTime;
    Uint32 FinalTime;

    while(!app->quit){
        DelayTime = 1000 / globalFPS;
        StartTime = SDL_GetTicks();

        SDL_Event event;
        while(SDL_PollEvent(&event)){
            if(event.type == SDL_QUIT){
                app->quit = true;
            }
            app->InputHandle(&event);
        }

        app->Loop();

        FinalTime = SDL_GetTicks();
        FrameTime = FinalTime - StartTime;
        
        if (DelayTime > FrameTime){
            SDL_Delay(DelayTime - FrameTime);
        }
        FinalTime = SDL_GetTicks();
        FrameTime = FinalTime - StartTime;
        if(FrameTime == 0){
            globalFps = 60;
        } else {
            globalFps = 1000/(int)FrameTime;
        }
    }
    cout << SDL_GetError();
    return 0;
}


SDL_Renderer* globalRender;
TTF_Font* globalFont;
int globalFps = 0;
int globalFPS = 60;
bool* globalQuit;
Layout** globalLayout;
bool globalSound = true;
void setLayout(Layouts l){
        switch (l)
        {
        case CONFIGURAR:
            delete *globalLayout;
            *globalLayout = new Configurar();
            break;
        case MENU:
            delete *globalLayout;
            *globalLayout = new Menu();
            break;
        case JOGO:
            delete *globalLayout;
            *globalLayout = new Game();
            break;
        default:
            break;
        }
    }
