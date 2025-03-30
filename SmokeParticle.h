#ifndef SMOKE_PARTICLE_H
#define SMOKE_PARTICLE_H

#include <SDL.h>
#include "Game.h"

struct SmokeParticle {
    float x, y;
    float velocityX, velocityY;
    float alpha;
    float lifetime;
    float size;

    SmokeParticle(float startX, float startY);
    void update();
    void render(SDL_Renderer* renderer, SDL_Texture* texture);
};

#endif
