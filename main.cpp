#include "Game.h"
#include "Screen.h"
#include "Render.h"
#include "SmokeParticle.h"
#include "SparkleParticle.h"
#include "Item.h"
#include "Bird.h"
#include "Pipe.h"
#include <ctime>

int main(int argc, char* argv[]) {
    srand(time(0));
    initSDL();
    loadAssets();

    while (true) {
        showStartScreen();
        Pipe::pipeCount = 0;
        bool running = true;
        SDL_Event event;
        Bird bird;
        std::vector<Pipe> pipes;
        std::vector<Item> items;
        int pipeSpawnTimer = 0;
        int itemSpawnCounter = 0;
        int score = 0;
        pipeSpeed = PIPE_SPEED;
        pipeSpawnInterval = PIPE_SPAWN_INTERVAL;
        bool paused = false;

        while (running) {
            bgX -= BG_SPEED;
            if (bgX <= -SCREEN_WIDTH) {
                bgX = 0;
            }

            while (SDL_PollEvent(&event)) {
                if (event.type == SDL_QUIT) exit(0);
                if (event.type == SDL_KEYDOWN) {
                    if (event.key.keysym.sym == SDLK_SPACE) {
                        bird.flap();
                    }
                    if (event.key.keysym.sym == SDLK_p) {
                        paused = !paused;
                        if (paused) showPauseScreen();
                    }
                }
            }

            if (!paused) {
                bird.update();

                if (score > 0 && score % 5 == 0) {
                    pipeSpeed = PIPE_SPEED + (score / 5) * PIPE_SPEED_INCREASE;
                    pipeSpawnInterval = std::max(PIPE_SPAWN_MIN, PIPE_SPAWN_INTERVAL - (score / 5) * PIPE_SPAWN_REDUCE);
                }

                if (pipeSpawnTimer++ > pipeSpawnInterval) {
                    pipes.emplace_back(SCREEN_WIDTH);
                    pipeSpawnTimer = 0;
                    itemSpawnCounter++;

                    if (itemSpawnCounter >= ITEM_SPAWN_INTERVAL) {
                        items.emplace_back(SCREEN_WIDTH + PIPE_WIDTH / 2, pipes);
                        itemSpawnCounter = 0;
                    }
                }

                bool collisionDetected = false;
                for (auto it = pipes.begin(); it != pipes.end();) {
                    it->update();
                    if (!bird.isFalling && it->checkCollision(bird.rect)) {
                        bird.isFalling = true;
                        collisionDetected = true;
                        screenShake(SCREEN_SHAKE_INTENSITY, SCREEN_SHAKE_DURATION);
                        Mix_PlayChannel(-1, hitSound, 0);
                        Mix_PlayChannel(-1, fallSound, 0);
                        if (score > highScore) {
                            highScore = score;
                        }
                    }
                    if (!it->passed && bird.x > it->x + PIPE_WIDTH/2) {
                        score++;
                        it->passed = true;
                        Mix_PlayChannel(-1, scoreSound, 0);
                        bird.addSparkles();
                    }
                    if (it->x + PIPE_WIDTH < 0) {
                        it = pipes.erase(it);
                    } else {
                        ++it;
                    }
                }

                for (auto it = items.begin(); it != items.end();) {
                    it->update();
                    if (!it->collected && it->checkCollision(bird.rect)) {
                        it->collected = true;
                        Mix_PlayChannel(-1, itemSound, 0);
                        if (it->type == 0) {
                            score += ITEM_SCORE_BONUS;
                        } else if (it->type == 1) {
                            bird.shrink();
                        }
                    }
                    if (it->x < 0) {
                        it = items.erase(it);
                    } else {
                        ++it;
                    }
                }

                if (bird.y > SCREEN_HEIGHT) {
                    running = false;
                    if (!collisionDetected) {
                        Mix_PlayChannel(-1, hitSound, 0);
                    }
                    showGameOverScreen(score);
                }
            }

            SDL_RenderClear(renderer);
            SDL_Rect bgRect1 = {bgX, 0, SCREEN_WIDTH, SCREEN_HEIGHT};
            SDL_Rect bgRect2 = {bgX + SCREEN_WIDTH, 0, SCREEN_WIDTH, SCREEN_HEIGHT};
            SDL_RenderCopy(renderer, backgroundTexture, NULL, &bgRect1);
            SDL_RenderCopy(renderer, backgroundTexture, NULL, &bgRect2);
            bird.render(renderer, birdTexture);
            for (auto& pipe : pipes) {
                pipe.render(renderer, pipeTexture);
            }
            for (auto& item : items) {
                item.render(renderer, itemTexture);
            }
            renderScore(score);

            if (bird.isShrunk) {
                SDL_Surface* surface = TTF_RenderText_Solid(font, "Shrunk!", {0, 255, 255});
                SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);
                SDL_Rect textRect = {SHRUNK_TEXT_X, SHRUNK_TEXT_Y, surface->w, surface->h};
                SDL_RenderCopy(renderer, texture, NULL, &textRect);
                SDL_FreeSurface(surface);
                SDL_DestroyTexture(texture);
            }

            SDL_RenderPresent(renderer);
            SDL_Delay(16);
        }
    }

    cleanUp();
    return 0;
}
