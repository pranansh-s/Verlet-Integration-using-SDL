#pragma once
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

#include "Math.hpp"

class Button {
    public:
        Button(SDL_Texture *textureOn, SDL_Texture *textureOff, Vector2f size, Vector2f position, bool onn)
        :texOn(textureOn), texOff(textureOff), siz(size), pos(position), on(onn){
            init.x = 0;
            init.y = 0;
            init.w = size.x;
            init.h = size.y;
        }
        bool click(Vector2f mousePos){
            if((mousePos.x >= pos.x && mousePos.x <= pos.x + siz.x) && (mousePos.y >= pos.y && mousePos.y <= pos.y + siz.y)){
                on = !on;
                return true;
            }
            return false;
        }
        bool checkOn(){
            return on;
        }
        SDL_Rect getInit(){
            return init;
        }
        Vector2f getPos(){
            return pos;
        }
        SDL_Texture* getTexture(){
            if(on) return texOn;
            return texOff;
        }
    private:
        bool on;
        SDL_Rect init;
        SDL_Texture *texOn, *texOff;
        Vector2f siz, pos;
};