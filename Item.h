#ifndef ITEM_H
#define ITEM_H

#include <SDL.h>
#include <vector>
#include "Game.h"
#include "Pipe.h"

struct Item {
    int x, y, type;
    SDL_Rect rect;
    bool collected;

    Item(int startX, const std::vector<Pipe>& pipes);
    void update();
    bool checkCollision(SDL_Rect bird);
    void render(SDL_Renderer* renderer, SDL_Texture* texture);
};

#endif
