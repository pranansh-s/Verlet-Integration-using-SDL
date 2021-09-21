#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <vector>

#include "Math.hpp"
#include "Point.hpp"
#include "Button.hpp"
#include "Window.hpp"

const float friction = 0.99f, gravity = 0.098f, cor = 0.85f;
std::vector<Point> MainComponents;
std::vector<PointPair> lines;

//Verlet Integration
void animatePoints(){
    for(Point &p: MainComponents){
        if(!p.getLocked()){
            float vx = (p.pos.x - p.oldPos.x) * friction;
            float vy = (p.pos.y - p.oldPos.y) * friction;

            p.oldPos = Vector2f(p.pos.x, p.pos.y);
            p.pos = Vector2f(p.pos.x + vx, p.pos.y + vy + gravity);

            //Reversing at edges
            if(p.pos.x > 700) {
                p.pos.x = 700;
                p.oldPos.x = p.pos.x + vx * cor;
            }
            else if(p.pos.x < 0) {
                p.pos.x = 0;
                p.oldPos.x = p.pos.x + vx * cor;
            }
            if(p.pos.y > 700) {
                p.pos.y = 700;
                p.oldPos.y = p.pos.y + vy * cor;
            }
            else if(p.pos.y < 0) {
                p.pos.y = 0;
                p.oldPos.y = p.pos.y + vy * cor;
            }
        }
    }
}

void animateSticks(){
    for(PointPair& e : lines){
        float cx = e.b->pos.x - e.a->pos.x;
        float cy = e.b->pos.y - e.a->pos.y;

        float cDis = e.dst - sqrt(cx * cx + cy * cy);
        float percent = cDis / sqrt(cx * cx + cy * cy) / 2;

        float offX = cx * percent;
        float offY = cy * percent;

        if(!e.a->getLocked()){
            e.a->pos = Vector2f(e.a->pos.x - offX, e.a->pos.y - offY);
        }
        if(!e.b->getLocked()){
            e.b->pos = Vector2f(e.b->pos.x + offX, e.b->pos.y + offY);
        }
    }
}

int main(int argv, char** args){
    SDL_Init(SDL_INIT_EVERYTHING);    

    Window window("verlet", 700, 700);

    SDL_Texture *play = window.loadTexture("gfx/play.png");
    SDL_Texture *pause = window.loadTexture("gfx/pause.png");
    SDL_Texture *point = window.loadTexture("gfx/point.png");

    bool isRunning = true, isLeftDown = false, makeLine = false;
    Vector2f mousePos;
    Point* savedPos;
    SDL_Event event;

    std::vector<Button> UiComponents = {
        Button(pause, play, Vector2f(40, 40), Vector2f(10, 10), false)
    };
    //timeStep -> Allows rendering at 100 frames per second: 1/100
    float timeStep = 0.01f;
	float currentTime = SDL_GetTicks() * 0.001f;
	float accumulator = 0.0f;
	while (isRunning)
	{
		float newTime = SDL_GetTicks() * 0.001f;
		float f = newTime - currentTime;
		currentTime = newTime;
		
		accumulator += f;
        while(accumulator >= timeStep){
            while (SDL_PollEvent(&event)){
                if(event.type == SDL_QUIT){
					isRunning = false;
					break;
				}
                //Register key events here ->
                else if(event.type == SDL_MOUSEMOTION){
                    mousePos = Vector2f(event.motion.x, event.motion.y);
                }
                else if(event.type == SDL_MOUSEBUTTONDOWN){
                    if(event.button.button == SDL_BUTTON_LEFT && !isLeftDown){
                        isLeftDown = true;
                        bool ui = false, isLocking = false;
                        for(Button& e : UiComponents){
                           ui |= e.click(mousePos);
                        }
                        if(!UiComponents[0].checkOn()){
                            for(Point& p : MainComponents){
                                if(p.click(mousePos)){
                                    p.setLockUnlock();
                                    isLocking = true;
                                    break;
                                }
                            }
                            if(!ui && !isLocking) MainComponents.push_back(Point(point, Vector2f(10, 10), Vector2f(mousePos.x, mousePos.y), Vector2f(mousePos.x, mousePos.y), false));
                        }
                    }
                    else if(event.button.button == SDL_BUTTON_RIGHT){
                        if(!UiComponents[0].checkOn()){
                            savedPos = NULL;
                            for(Point& p : MainComponents){
                                if(p.click(mousePos)){
                                    makeLine = true;
                                    savedPos = &p;
                                    break;
                                }
                            }
                        }
                    }
                }
                else if(event.type == SDL_MOUSEBUTTONUP){
                    if(event.button.button == SDL_BUTTON_LEFT && isLeftDown){
                        isLeftDown = false;
                    }
                    else if(event.button.button == SDL_BUTTON_RIGHT && makeLine){
                        if(!UiComponents[0].checkOn()){
                            for(Point& p : MainComponents){
                                if(p.click(mousePos)){
                                    Point *ss = &p;
                                    float dst = sqrt(pow(savedPos->pos.x - p.pos.x, 2) + pow(savedPos->pos.y - p.pos.y, 2));
                                    lines.push_back(PointPair(savedPos, ss, dst));
                                    break;
                                }
                            }
                        }
                        makeLine = false;
                    }
                }
            }
            //Perform once per frame ->
            if(UiComponents[0].checkOn()){
                animatePoints();
                animateSticks();
            }
            accumulator -= timeStep;
        }

        window.clearRenderer();
        //Render content here ->
        for(Button& e : UiComponents){
            window.drawEntity(e);
        }
        for(Point& e : MainComponents){
            window.drawEntity(e);
        }
        for(PointPair& e: lines){
            window.drawLine(e.a, e.b);            
        }
        if(makeLine){
            window.drawLine(savedPos, mousePos);
        }
        window.display();
    }
    window.end();
    SDL_Quit();

    return 0;
}
