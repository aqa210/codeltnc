#ifndef SPARKLE_PARTICLE_H
#define SPARKLE_PARTICLE_H

#include <SDL.h>
#include "Game.h"

struct SparkleParticle {
    float x, y;
    float velocityX, velocityY;
    float alpha;
    float lifetime;

    SparkleParticle(float startX, float startY);
    void update();
    void render(SDL_Renderer* renderer, SDL_Texture* texture);
};

#endif
