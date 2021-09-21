#pragma once
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

#include "Point.hpp"
#include "Button.hpp"

class Window {
    public:
        Window(const char* title, int w, int h);
        SDL_Texture* loadTexture(const char* fileName);
        void clearRenderer();
        void drawLine(Point* a, Point* b);
        void drawLine(Point* a, Vector2f& b);
        void drawEntity(Point& p);
        void drawEntity(Button& b);
        void display();
        void end();
    private:
        SDL_Window* window;
        SDL_Renderer* renderer;
};