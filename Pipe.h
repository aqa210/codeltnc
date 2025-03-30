#ifndef PIPE_H
#define PIPE_H

#include <SDL.h>
#include "Game.h"

struct Pipe {
    int x, height;
    SDL_Rect topRect, bottomRect;
    bool passed;
    int yOffset;
    int yVelocity;
    bool isMoving;

    static int pipeCount;

    Pipe(int startX);
    void updateRects();
    void update();
    bool checkCollision(SDL_Rect bird);
    void render(SDL_Renderer* renderer, SDL_Texture* texture);
};

#endif
