#pragma once
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <string>
#include <set>
#include <iostream>
#include <vector>
#include <initializer_list>
#include "Game.h"
#include "entt/entt.hpp"
//#include <entt/entt.hpp>
using namespace std;

enum Aling {
    CENTER,
    LEFT_TOP,
    RIGHT_TOP,
    LEFT_BOTTOM,
    RIGHT_BOTTOM
};

class WriteableText {
private:
	TTF_Font* font;
	SDL_Renderer* renderer;
	int x = 0;
	int y = 0;
	string text;
	SDL_Color color = {255,255,255};
    

	int textW = 0;
	int textH = 0;

public:
    Aling aling = CENTER;
	SDL_Texture* texture;
	SDL_Rect dsrect;
	WriteableText(TTF_Font* f, SDL_Renderer* r, string t){
		font = f;
		renderer = r;
		text = t;
		createTexture(text);
	}
    void createTexture(string t){
        SDL_Surface* surface = TTF_RenderText_Solid(globalFont, t.c_str(), color);
		texture = SDL_CreateTextureFromSurface(renderer, surface);
		SDL_FreeSurface(surface);
		SDL_QueryTexture(texture, NULL, NULL, &textW, &textH);
        createBox();
    }
    void createBox(){
        int tx;
        int ty;
        switch (aling)
        {
        case CENTER:
            tx = x - textW/2;
            ty = y - textH/2;
            break;
        case LEFT_TOP:
            tx = x - textW;
            ty = y;
            break;
        case LEFT_BOTTOM:
            tx = x - textW;
            ty = y - textH;
            break;
        case RIGHT_TOP:
            tx = x;
            ty = y;
            break;
        case RIGHT_BOTTOM:
            tx = x;
            ty = y - textH;
            break;
        
        default:
            tx = x;
            ty = y;
            break;
        }
		dsrect = {tx, ty, textW, textH};
    }
	void setText(string t){
		text = t;
		createTexture(text);
	}
	void setPosition(int X, int Y){
		x = X;
		y = Y;
		createBox();
	}
	void setArrow(bool b){
		if (b){
			string arrow = "->";
			string n = arrow + text;
			createTexture(n);
		} else {
			createTexture(text);
		}
	}
    void draw(){
        SDL_RenderCopy(renderer, texture,NULL,&dsrect);
    }
};

class OptionsMenu {
    private:
        vector<WriteableText*> options;
        TTF_Font* font;
        SDL_Renderer* renderer;
        int x;
        int y;

    public:
        int actualOption = 0;

        OptionsMenu(int X, int Y, TTF_Font* f, SDL_Renderer* r, initializer_list<string> opt){
            font = f;
            renderer = r;
            x = X;
            y = Y;
            for (string text : opt)
            {
                options.push_back(new WriteableText(globalFont, renderer, text));
            }
            
            for(int i = 0; i < options.size(); i++){
                options[i]->setPosition(x, y+20*i);
            }
            options[actualOption]->setArrow(true);
        }
        void Draw(){
            for(int i = 0; i < options.size(); i++){
                options[i]->draw();
            }
        }
        void goDown(){
            options[actualOption]->setArrow(false);
            actualOption++;
            if(actualOption >= options.size()) actualOption = 0;
            options[actualOption]->setArrow(true);
        }
        void goUp(){
            options[actualOption]->setArrow(false);
            actualOption--;
            if(actualOption < 0) actualOption = options.size()-1;
            options[actualOption]->setArrow(true);
        }

        void setText(int idx, string text){
            options[idx]->setText(text);
        }
};

class Configurar : public Layout
{
private:
    OptionsMenu* opcoes;
public:
    Configurar(/* args */) {
        string sound;
        string FPS;
        if (globalSound)
        {
            sound = "Som     LIGADO";
        } else
        {
            sound = "Som     DESLIGADO";
        }
        if (globalFPS == 60)
        {
            FPS = "fps     60";
        }
        else
        {
            FPS = "fps     30";
        }
        
        
        
        opcoes = new OptionsMenu(320,240,globalFont,globalRender,{sound,FPS, "Voltar"});
    }
    ~Configurar() {}

    void Render() override {
        opcoes->Draw();
    }
    void InputHandle(SDL_Event* e) override {
        if (e->type == SDL_KEYDOWN){
                if(e->key.keysym.sym == SDLK_DOWN){
                    opcoes->goDown();
                }
                if(e->key.keysym.sym == SDLK_UP){
                    opcoes->goUp();
                }
                if(e->key.keysym.sym == SDLK_RETURN || e->key.keysym.sym == SDLK_z){
                    switch (opcoes->actualOption)  
                    {
                    case 0:
                        if(globalSound){
                            globalSound = false;
                            opcoes->setText(0, "Som     DESLIGADO");
                        } else {
                            globalSound = true;
                            opcoes->setText(0, "Som     LIGADO");
                        }
                        break;
                    case 1:
                        if(globalFPS == 60){
                            globalFPS = 30;
                            opcoes->setText(1, "fps     30");
                        } else {
                            globalFPS = 60;
                            opcoes->setText(1, "fps     60");
                        }
                        break;
                    case 2:
                        setLayout(MENU);
                        break;
                    
                    
                    default:
                        break;
                    }
                }
            }
    }
};

class Menu: public Layout{
    private:
        set<SDL_Keycode> pressedKeys;

        OptionsMenu* options;

        WriteableText* fpsCounter;

        int opcaoAtual = 0;
    public:
        Menu(){
            
            options = new OptionsMenu(
                320,
                240,
                globalFont,
                globalRender,
                {
                    "Jogar",
                    "Configurar",
                    "Sair"
                }
            );

            fpsCounter = new WriteableText(globalFont, globalRender, "Fps: ");
            fpsCounter->aling = RIGHT_TOP;
            fpsCounter->setPosition(0,0);
        }
        void Render() override {
            fpsCounter->setText("Fps: " + to_string(globalFps));
            options->Draw();
            fpsCounter->draw();
        }

        void InputHandle(SDL_Event* e) override {
            if (e->type == SDL_KEYDOWN){
                pressedKeys.insert(e->key.keysym.sym);

                if(e->key.keysym.sym == SDLK_DOWN){
                    options->goDown();
                }
                if(e->key.keysym.sym == SDLK_UP){
                    options->goUp();
                }
                if(e->key.keysym.sym == SDLK_RETURN || e->key.keysym.sym == SDLK_z){
                    switch (options->actualOption)  
                    {
                    case 1:
                        setLayout(CONFIGURAR);
                        break;
                    case 2:
                        *globalQuit = true;
                        break;
                    
                    
                    default:
                        break;
                    }
                }
            }
            if (e->type == SDL_KEYUP){
                pressedKeys.erase(e->key.keysym.sym);
            }
        }
};

struct Position {
    int x;
    int y;
};
struct Velocity {
    int dx;
    int dy;
};
struct Texture {
    SDL_Texture* texture;
};


void Update(entt::registry &registry){
    auto view = registry.view<const Position, const Velocity, const Texture>();

    view.each([](const auto &pos, auto &vel, auto &tex) {
        

    });
}

class Game: public Layout
{
private:
    entt::registry registry;
public:
    Game() {
        entt::entity entidade = registry.create();
        SDL_Surface* sur = SDL_CreateRGBSurface(0,10,10,8,8,8,8,8);
        registry.emplace<Position>(entidade, 0, 0);
        registry.emplace<Velocity>(entidade, 1, 1);
        registry.emplace<Texture>(entidade, 
            SDL_CreateTextureFromSurface(globalRender, sur)
        );
    }
    ~Game() {}
    void Render() override {
        Update(registry);
    }
    void InputHandle(SDL_Event* e) override {

    }
};



