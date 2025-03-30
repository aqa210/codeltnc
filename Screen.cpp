#include "Screen.h"
#include "Render.h"
#include <iostream>

SDL_Rect musicButton = {SCREEN_WIDTH - MUSIC_BUTTON_X_OFFSET, MUSIC_BUTTON_Y, MUSIC_BUTTON_WIDTH, MUSIC_BUTTON_HEIGHT};

void toggleMusic() {
    musicOn = !musicOn;
    if (musicOn) {
        Mix_PlayMusic(bgMusic, -1);
    } else {
        Mix_HaltMusic();
    }
}

void showStartScreen() {
    bool waiting = true;
    SDL_Event event;

    while (waiting) {
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) exit(0);
            if (event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_SPACE) waiting = false;
            if (event.type == SDL_MOUSEBUTTONDOWN) {
                int mouseX = event.button.x;
                int mouseY = event.button.y;
                if (mouseX >= musicButton.x && mouseX <= musicButton.x + musicButton.w &&
                    mouseY >= musicButton.y && mouseY <= musicButton.y + musicButton.h) {
                    toggleMusic();
                }
            }
        }

        SDL_RenderClear(renderer);
        SDL_RenderCopy(renderer, backgroundTexture, NULL, NULL);

        SDL_Rect startRect = {SCREEN_WIDTH / 2 - START_SCREEN_X_OFFSET, SCREEN_HEIGHT / 2 - START_SCREEN_Y_OFFSET,
                             START_SCREEN_WIDTH, START_SCREEN_HEIGHT};
        SDL_RenderCopy(renderer, startTexture, NULL, &startRect);

        if (musicOn)
            SDL_RenderCopy(renderer, musicOnTexture, NULL, &musicButton);
        else
            SDL_RenderCopy(renderer, musicOffTexture, NULL, &musicButton);

        if (!font) font = TTF_OpenFont(FONT_FILE, FONT_SIZE);

        Uint32 time = SDL_GetTicks();
        bool showText = (time / TEXT_BLINK_INTERVAL) % 2 == 0;

        if (showText) {
            std::string startText = "Press SPACE to Start";
            SDL_Surface* surface = TTF_RenderText_Solid(font, startText.c_str(), textColor);
            SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);

            SDL_Rect textRect = {SCREEN_WIDTH / 2 - surface->w / 2, SCREEN_HEIGHT - 100, surface->w, surface->h};
            SDL_RenderCopy(renderer, texture, NULL, &textRect);

            SDL_FreeSurface(surface);
            SDL_DestroyTexture(texture);
        }

        SDL_RenderPresent(renderer);
    }
}

void showGameOverScreen(int score) {
    bool waiting = true;
    bool firstPress = false;
    SDL_Event event;

    while (waiting) {
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) exit(0);
            if (event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_SPACE) {
                if (!firstPress) {
                    firstPress = true;
                } else {
                    waiting = false;
                }
            }
        }

        SDL_RenderClear(renderer);
        SDL_Rect bgRect1 = {bgX, 0, SCREEN_WIDTH, SCREEN_HEIGHT};
        SDL_Rect bgRect2 = {bgX + SCREEN_WIDTH, 0, SCREEN_WIDTH, SCREEN_HEIGHT};
        SDL_RenderCopy(renderer, backgroundTexture, NULL, &bgRect1);
        SDL_RenderCopy(renderer, backgroundTexture, NULL, &bgRect2);

        SDL_Rect gameOverRect = {SCREEN_WIDTH / 2 - GAMEOVER_SCREEN_X_OFFSET, SCREEN_HEIGHT / 2 - GAMEOVER_SCREEN_Y_OFFSET,
                                GAMEOVER_SCREEN_WIDTH, GAMEOVER_SCREEN_HEIGHT};
        SDL_RenderCopy(renderer, gameOverTexture, NULL, &gameOverRect);

        renderScore(score);
        renderHighScore();
        if (firstPress) {
            SDL_Surface* surface = TTF_RenderText_Solid(font, "Press SPACE again to restart", textColor);
            SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);
            SDL_Rect textRect = {SCREEN_WIDTH / 2 - surface->w / 2, SCREEN_HEIGHT - 100, surface->w, surface->h};
            SDL_RenderCopy(renderer, texture, NULL, &textRect);
            SDL_FreeSurface(surface);
            SDL_DestroyTexture(texture);
        }

        SDL_RenderPresent(renderer);
    }
}

void showPauseScreen() {
    SDL_Surface* surface = TTF_RenderText_Solid(font, "Paused - Press P to Resume", textColor);
    SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);
    SDL_Rect textRect = {SCREEN_WIDTH / 2 - surface->w / 2, SCREEN_HEIGHT / 2 - surface->h / 2, surface->w, surface->h};
    SDL_RenderCopy(renderer, texture, NULL, &textRect);
    SDL_RenderPresent(renderer);
    SDL_FreeSurface(surface);
    SDL_DestroyTexture(texture);

    bool paused = true;
    SDL_Event event;
    while (paused) {
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) exit(0);
            if (event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_p) {
                paused = false;
            }
        }
    }
}
