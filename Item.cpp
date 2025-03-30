#include "Item.h"
#include <ctime>

Item::Item(int startX, const std::vector<Pipe>& pipes) {
    x = startX;
    int closestPipeHeight = SCREEN_HEIGHT / 2;
    for (const auto& pipe : pipes) {
        if (pipe.x <= startX && pipe.x + PIPE_WIDTH >= startX) {
            closestPipeHeight = pipe.height;
            break;
        } else if (pipe.x < startX) {
            closestPipeHeight = pipe.height;
        }
    }
    y = closestPipeHeight + PIPE_GAP / 2;
    rect = {x, y, ITEM_WIDTH, ITEM_HEIGHT};
    collected = false;
    type = rand() % ITEM_TYPE_COUNT;
}

void Item::update() {
    x -= static_cast<int>(pipeSpeed);
    rect.x = x;
}

bool Item::checkCollision(SDL_Rect bird) {
    return SDL_HasIntersection(&bird, &rect);
}

void Item::render(SDL_Renderer* renderer, SDL_Texture* texture) {
    if (!collected) {
        SDL_RenderCopy(renderer, texture, NULL, &rect);
    }
}
