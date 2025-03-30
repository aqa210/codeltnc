#ifndef BIRD_H
#define BIRD_H

#include <SDL.h>
#include <vector>
#include "Game.h"
#include "SmokeParticle.h"
#include "SparkleParticle.h"

struct Bird {
    int x, y, velocity;
    SDL_Rect rect;
    SDL_Rect spriteClips[3];
    int frame;
    Uint32 lastFrameTime;
    bool isFalling;
    float rotation;
    std::vector<SmokeParticle> smokeTrail;
    std::vector<SparkleParticle> sparkles;
    bool isShrunk;
    Uint32 shrinkTime;

    Bird();
    void flap();
    void update();
    void addSparkles();
    void shrink();
    void render(SDL_Renderer* renderer, SDL_Texture* texture);
};

#endif
