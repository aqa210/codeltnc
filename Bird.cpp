#include "Bird.h"
#include <SDL_mixer.h>
#include <ctime>

Bird::Bird() {
    x = BIRD_START_X;
    y = BIRD_START_Y;
    velocity = 0;
    rect = {x, y, BIRD_WIDTH, BIRD_HEIGHT};
    spriteClips[0] = {0, 0, SPRITESHEET_WIDTH/3, SPRITESHEET_HEIGHT};
    spriteClips[1] = {SPRITESHEET_WIDTH/3, 0, SPRITESHEET_WIDTH/3, SPRITESHEET_HEIGHT};
    spriteClips[2] = {SPRITESHEET_WIDTH*2/3, 0, SPRITESHEET_WIDTH/3, SPRITESHEET_HEIGHT};
    frame = 0;
    lastFrameTime = 0;
    isFalling = false;
    rotation = 0.0f;
    isShrunk = false;
    shrinkTime = 0;
}

void Bird::flap() {
    if (!isFalling) {
        velocity = FLAP_STRENGTH;
        Mix_PlayChannel(-1, flapSound, 0);
    }
}

void Bird::update() {
    if (isFalling) {
        velocity += GRAVITY * GRAVITY_FALL_MULTIPLIER;
        rotation += BIRD_ROTATION_SPEED;
        if (rotation > BIRD_MAX_ROTATION) rotation = BIRD_MAX_ROTATION;
        if (rand() % SMOKE_SPAWN_CHANCE == 0) {
            smokeTrail.emplace_back(x + rect.w/2, y + rect.h/2);
        }
    } else {
        velocity += GRAVITY;
    }
    y += velocity;
    rect.x = x;
    rect.y = y;

    if (isShrunk && SDL_GetTicks() - shrinkTime > BIRD_SHRINK_DURATION) {
        isShrunk = false;
        rect.w = BIRD_WIDTH;
        rect.h = BIRD_HEIGHT;
    } else if (isShrunk) {
        rect.w = BIRD_SHRUNK_WIDTH;
        rect.h = BIRD_SHRUNK_HEIGHT;
    }

    if (!isFalling && SDL_GetTicks() - lastFrameTime > BIRD_FRAME_DELAY) {
        frame = (frame + 1) % 3;
        lastFrameTime = SDL_GetTicks();
    }

    for (auto it = smokeTrail.begin(); it != smokeTrail.end();) {
        it->update();
        if (it->lifetime <= 0) {
            it = smokeTrail.erase(it);
        } else {
            ++it;
        }
    }

    for (auto it = sparkles.begin(); it != sparkles.end();) {
        it->update();
        if (it->lifetime <= 0) {
            it = sparkles.erase(it);
        } else {
            ++it;
        }
    }
}

void Bird::addSparkles() {
    for (int i = 0; i < SPARKLE_COUNT; i++) {
        sparkles.emplace_back(x + rect.w/2, y + rect.h/2);
    }
}

void Bird::shrink() {
    isShrunk = true;
    shrinkTime = SDL_GetTicks();
    rect.w = BIRD_SHRUNK_WIDTH;
    rect.h = BIRD_SHRUNK_HEIGHT;
}

void Bird::render(SDL_Renderer* renderer, SDL_Texture* texture) {
    for (auto& smoke : smokeTrail) {
        smoke.render(renderer, smokeTexture);
    }
    for (auto& sparkle : sparkles) {
        sparkle.render(renderer, sparkleTexture);
    }
    SDL_RenderCopyEx(renderer, texture, &spriteClips[frame],
                    &rect, rotation, NULL, SDL_FLIP_NONE);
}
