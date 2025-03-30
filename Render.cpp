#include "Render.h"
#include<string>
void renderHighScore() {
    if (!font) font = TTF_OpenFont("font.ttf", 24); // Thay "font.ttf" bằng đường dẫn font của bạn

    std::string highScoreText = "High Score: " + std::to_string(highScore);
    SDL_Surface* surface = TTF_RenderText_Solid(font, highScoreText.c_str(), textColor);
    SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);

    SDL_Rect textRect = {SCREEN_WIDTH / 2 - surface->w / 2, 50, surface->w, surface->h}; // Vị trí hiển thị
    SDL_RenderCopy(renderer, texture, NULL, &textRect);

    SDL_FreeSurface(surface);
    SDL_DestroyTexture(texture);
}

void renderScore(int score) {
    if (!font) font = TTF_OpenFont(FONT_FILE, FONT_SIZE);

    std::string scoreText = "Score: " + std::to_string(score);
    SDL_Surface* surface = TTF_RenderText_Solid(font, scoreText.c_str(), textColor);
    SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);

    SDL_Rect textRect = {SCORE_TEXT_X, SCORE_TEXT_Y, surface->w, surface->h};
    SDL_RenderCopy(renderer, texture, NULL, &textRect);

    SDL_FreeSurface(surface);
    SDL_DestroyTexture(texture);
}

void screenShake(int intensity, int duration) {
    Uint32 startTime = SDL_GetTicks();
    while (SDL_GetTicks() - startTime < duration) {
        int offsetX = (rand() % (intensity * 2)) - intensity;
        int offsetY = (rand() % (intensity * 2)) - intensity;
        SDL_Rect viewport = {offsetX, offsetY, SCREEN_WIDTH, SCREEN_HEIGHT};
        SDL_RenderSetViewport(renderer, &viewport);
        SDL_RenderPresent(renderer);
        SDL_Delay(16);
    }
    SDL_Rect viewport = {0, 0, SCREEN_WIDTH, SCREEN_HEIGHT};
    SDL_RenderSetViewport(renderer, &viewport);
}
