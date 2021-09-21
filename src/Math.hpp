#pragma once

struct Vector2f{
    Vector2f():x(0.0f), y(0.0f){}
    Vector2f(float posX, float posY):x(posX), y(posY){}

    float x, y;
};