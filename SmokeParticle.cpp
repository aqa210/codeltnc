#include "SmokeParticle.h"
#include <ctime>

SmokeParticle::SmokeParticle(float startX, float startY) {
    x = startX;
    y = startY;
    velocityX = (rand() % SMOKE_VELOCITY_X_RANGE - SMOKE_VELOCITY_X_RANGE/2) * 1.0f;
    velocityY = (rand() % SMOKE_VELOCITY_Y_RANGE - SMOKE_VELOCITY_Y_RANGE/2) * 1.0f;
    alpha = 255;
    lifetime = SMOKE_LIFETIME;
    size = (rand() % SMOKE_SIZE_MIN + SMOKE_SIZE_MIN);
}

void SmokeParticle::update() {
    x += velocityX;
    y += velocityY;
    lifetime -= SMOKE_FADE_RATE;
    alpha = lifetime * 255;
    size += 1.0f;
    if (size > SMOKE_MAX_SIZE) size = SMOKE_MAX_SIZE;
}

void SmokeParticle::render(SDL_Renderer* renderer, SDL_Texture* texture) {
    SDL_SetTextureAlphaMod(texture, static_cast<Uint8>(alpha));
    SDL_Rect rect = {static_cast<int>(x - size/2), static_cast<int>(y - size/2),
                    static_cast<int>(size), static_cast<int>(size)};
    SDL_RenderCopy(renderer, texture, NULL, &rect);
}
