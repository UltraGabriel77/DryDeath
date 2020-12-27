#if !defined(GAME_H)
#define GAME_H
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include "layouts.hpp"

class Layout
{
public:
    Layout() {}
    virtual ~Layout() {}
    virtual void Render() {}
    virtual void InputHandle(SDL_Event* e){}
};
enum Layouts {
    CONFIGURAR,
    MENU,
    JOGO
};

extern SDL_Renderer* globalRender;
extern TTF_Font* globalFont;
extern int globalFps;
extern bool* globalQuit;
extern Layout** globalLayout;
extern int globalFPS;
extern bool globalSound;
extern void setLayout(Layouts l);

#endif // GAME_H
