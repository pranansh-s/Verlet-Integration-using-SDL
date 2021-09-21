#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h> 

#include "Window.hpp"

//Initializes a window and renderer
Window::Window(const char* title, int w, int h):window(NULL), renderer(NULL){
    window = SDL_CreateWindow(title, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, w, h, SDL_WINDOW_SHOWN);
    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
}

//Loads and returns texture to program
SDL_Texture* Window::loadTexture(const char* filePath){
    SDL_Texture* texture = NULL;
    texture = IMG_LoadTexture(renderer, filePath);

    return texture;
}

void Window::drawEntity(Button& b){
    SDL_Rect src;
    src.x = b.getInit().x;
    src.y = b.getInit().y;
    src.w = b.getInit().w;
    src.h = b.getInit().h;

    SDL_Rect dst;
    dst.x = b.getPos().x;
    dst.y = b.getPos().y;
    dst.w = b.getInit().w;
    dst.h = b.getInit().h;
    SDL_RenderCopy(renderer, b.getTexture(), &src, &dst);
}

void Window::drawLine(Point* a, Point* b){
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
    SDL_RenderDrawLine(renderer, a->pos.x, a->pos.y, b->pos.x, b->pos.y);
}

void Window::drawLine(Point* a, Vector2f& b){
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
    SDL_RenderDrawLine(renderer, a->pos.x, a->pos.y, b.x, b.y);
}

void Window::drawEntity(Point& p){
    if(p.getLocked()){
        SDL_SetTextureColorMod(p.getTexture(), 255, 0, 0);
    }
    else{
        SDL_SetTextureColorMod(p.getTexture(), 255, 255, 255);
    }

    SDL_Rect src;
    src.x = p.getInit().x;
    src.y = p.getInit().y;
    src.w = p.getInit().w;
    src.h = p.getInit().h;

    SDL_Rect dst;
    dst.x = p.pos.x - p.siz.x / 2;
    dst.y = p.pos.y - p.siz.y / 2;
    dst.w = p.getInit().w;
    dst.h = p.getInit().h;
    SDL_RenderCopy(renderer, p.getTexture(), &src, &dst);
}

//Sets a background colour and clears renderer (used before each frame)
void Window::clearRenderer(){
    SDL_SetRenderDrawColor(renderer, 106, 106, 106, 255);
    SDL_RenderClear(renderer);
}

//Draws whatever has been rendered to the window
void Window::display(){
    SDL_RenderPresent(renderer);
}

//Destroys window and renderer (used before quitting to avoid memory leak)
void Window::end(){
    SDL_DestroyWindow(window);
    SDL_DestroyRenderer(renderer);
}
