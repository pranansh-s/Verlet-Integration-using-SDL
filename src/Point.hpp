#pragma once
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <math.h>

#include "Math.hpp"

class Point {
    public:
        Point(SDL_Texture *texture, Vector2f size, Vector2f position, Vector2f oldPosition, bool locked)
        :tex(texture), siz(size), pos(position), oldPos(oldPosition), lock(locked){
            init.x = 0;
            init.y = 0;
            init.w = size.x;
            init.h = size.y;
        }
        bool click(Vector2f mousePos){
            if((mousePos.x >= pos.x - siz.x / 2 && mousePos.x <= pos.x + siz.x / 2) && (mousePos.y >= pos.y - siz.y / 2 && mousePos.y <= pos.y + siz.y / 2)){
                return true;
            }
            return false;
        }
        void setLockUnlock(){
            lock = !lock;
        }
        bool getLocked(){
            return lock;
        }
        SDL_Rect getInit(){
            return init;
        }
        SDL_Texture* getTexture(){
            return tex;
        }
        Vector2f siz, pos, oldPos;
    private:        
        bool lock;
        SDL_Rect init;
        SDL_Texture *tex;
};

struct PointPair{
    PointPair(Point* x, Point* y, float distance):a(x), b(y), dst(distance){}

    float dst;
    Point *a, *b;
};