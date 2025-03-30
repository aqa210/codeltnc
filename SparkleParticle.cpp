#include "SparkleParticle.h"
#include <ctime>

SparkleParticle::SparkleParticle(float startX, float startY) {
    x = startX;
    y = startY;
    velocityX = (rand() % SPARKLE_VELOCITY_RANGE - SPARKLE_VELOCITY_RANGE/2) * 0.5f;
    velocityY = (rand() % SPARKLE_VELOCITY_RANGE - SPARKLE_VELOCITY_RANGE/2) * 0.5f;
    alpha = 255;
    lifetime = SPARKLE_LIFETIME;
}

void SparkleParticle::update() {
    x += velocityX;
    y += velocityY;
    lifetime -= SPARKLE_FADE_RATE;
    alpha = lifetime * 255;
}

void SparkleParticle::render(SDL_Renderer* renderer, SDL_Texture* texture) {
    SDL_SetTextureAlphaMod(texture, static_cast<Uint8>(alpha));
    SDL_Rect rect = {static_cast<int>(x), static_cast<int>(y), SPARKLE_SIZE, SPARKLE_SIZE};
    SDL_RenderCopy(renderer, texture, NULL, &rect);
}
