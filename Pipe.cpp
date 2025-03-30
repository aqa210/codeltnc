#include "Pipe.h"
#include <ctime>

int Pipe::pipeCount = 0;

Pipe::Pipe(int startX) {
    x = startX;
    height = rand() % PIPE_HEIGHT_RANGE + PIPE_HEIGHT_MIN;
    passed = false;
    isMoving = (pipeCount >= PIPE_MOVING_THRESHOLD);
    pipeCount++;
    yOffset = 0;
    yVelocity = PIPE_OFFSET_VELOCITY;
    updateRects();
}

void Pipe::updateRects() {
    topRect = {x, yOffset - PIPE_OFFSET_AMPLITUDE, PIPE_WIDTH, height};
    bottomRect = {x, height + PIPE_GAP + yOffset + PIPE_OFFSET_AMPLITUDE, PIPE_WIDTH, SCREEN_HEIGHT - (height + PIPE_GAP)};
}

void Pipe::update() {
    x -= static_cast<int>(pipeSpeed);
    if (isMoving) {
        yOffset += yVelocity;
        if (yOffset > PIPE_OFFSET_AMPLITUDE || yOffset < -PIPE_OFFSET_AMPLITUDE) {
            yVelocity = -yVelocity;
        }
    }
    updateRects();
}

bool Pipe::checkCollision(SDL_Rect bird) {
    SDL_Rect smallerBird = {bird.x + BIRD_COLLISION_OFFSET, bird.y + BIRD_COLLISION_OFFSET,
                           bird.w - BIRD_COLLISION_SHRINK, bird.h - BIRD_COLLISION_SHRINK};
    return SDL_HasIntersection(&smallerBird, &topRect) || SDL_HasIntersection(&smallerBird, &bottomRect);
}

void Pipe::render(SDL_Renderer* renderer, SDL_Texture* texture) {
    SDL_RenderCopy(renderer, texture, NULL, &bottomRect);
    SDL_RenderCopyEx(renderer, texture, NULL, &topRect, 180, NULL, SDL_FLIP_NONE);
}
