#include "Game.h"
#include <iostream>
#include<fstream>
int highScore = 0;
bool musicOn = true;
float pipeSpeed = PIPE_SPEED;
int pipeSpawnInterval = PIPE_SPAWN_INTERVAL;
SDL_Color textColor = {255, 255, 255};


SDL_Window* window = nullptr;
SDL_Renderer* renderer = nullptr;
SDL_Texture* birdTexture = nullptr;
SDL_Texture* pipeTexture = nullptr;
SDL_Texture* backgroundTexture = nullptr;
SDL_Texture* gameOverTexture = nullptr;
SDL_Texture* startTexture = nullptr;
SDL_Texture* musicOnTexture = nullptr;
SDL_Texture* musicOffTexture = nullptr;
SDL_Texture* smokeTexture = nullptr;
SDL_Texture* itemTexture = nullptr;
SDL_Texture* sparkleTexture = nullptr;
TTF_Font* font = nullptr;
Mix_Chunk* flapSound = nullptr;
Mix_Chunk* hitSound = nullptr;
Mix_Music* bgMusic = nullptr;
Mix_Chunk* scoreSound = nullptr;
Mix_Chunk* fallSound = nullptr;
Mix_Chunk* itemSound = nullptr;
int bgX = 0;

void loadHighScore() {
    std::ifstream file("highscore.txt");
    if (file.is_open()) {
        file >> highScore;
        file.close();
        std::cout << "Đã đọc highScore: " << highScore << std::endl;
    } else {
        highScore = 0;
        std::cout << "Không tìm thấy tệp, đặt highScore = 0" << std::endl;
    }
}

void saveHighScore(int currentScore) {
    if (currentScore > highScore) {
        highScore = currentScore;
        std::ofstream file("highscore.txt");
        if (file.is_open()) {
            file << highScore;
            file.close();
            std::cout << "Đã lưu highScore: " << highScore << std::endl;
        } else {
            std::cerr << "Không thể mở tệp để lưu" << std::endl;
        }
    }
}
void initSDL() {
    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO) < 0) {
        std::cerr << "SDL Initialization Failed: " << SDL_GetError() << std::endl;
        exit(1);
    }
    if (!(IMG_Init(IMG_INIT_PNG) & IMG_INIT_PNG)) {
        std::cerr << "SDL_image Initialization Failed: " << IMG_GetError() << std::endl;
        exit(1);
    }
    if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) < 0) {
        std::cerr << "SDL_mixer Initialization Failed: " << Mix_GetError() << std::endl;
        exit(1);
    }
    if (TTF_Init() == -1) {
        std::cerr << "SDL_ttf Initialization Failed: " << TTF_GetError() << std::endl;
        exit(1);
    }

    window = SDL_CreateWindow("Flappy Bird", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);

    if (!window || !renderer) {
        std::cerr << "SDL Error: " << SDL_GetError() << std::endl;
        exit(1);
    }
}

void loadAssets() {
    birdTexture = IMG_LoadTexture(renderer, BIRD_SPRITESHEET);
    pipeTexture = IMG_LoadTexture(renderer, PIPE_IMAGE);
    backgroundTexture = IMG_LoadTexture(renderer, BACKGROUND_IMAGE);
    gameOverTexture = IMG_LoadTexture(renderer, GAMEOVER_IMAGE);
    startTexture = IMG_LoadTexture(renderer, START_IMAGE);
    musicOnTexture = IMG_LoadTexture(renderer, MUSIC_ON_IMAGE);
    musicOffTexture = IMG_LoadTexture(renderer, MUSIC_OFF_IMAGE);
    smokeTexture = IMG_LoadTexture(renderer, SMOKE_IMAGE);
    itemTexture = IMG_LoadTexture(renderer, ITEM_IMAGE);
    sparkleTexture = IMG_LoadTexture(renderer, SPARKLE_IMAGE);

    flapSound = Mix_LoadWAV(FLAP_SOUND);
    hitSound = Mix_LoadWAV(HIT_SOUND);
    bgMusic = Mix_LoadMUS(BG_MUSIC);
    scoreSound = Mix_LoadWAV(SCORE_SOUND);
    fallSound = Mix_LoadWAV(FALL_SOUND);
    itemSound = Mix_LoadWAV(ITEM_SOUND);

    if (!birdTexture || !pipeTexture || !backgroundTexture || !gameOverTexture ||
        !startTexture || !smokeTexture || !itemTexture || !sparkleTexture ||
        !flapSound || !hitSound || !bgMusic || !scoreSound || !fallSound || !itemSound) {
        std::cerr << "Failed to load assets!" << std::endl;
        exit(1);
    }

    if (musicOn) Mix_PlayMusic(bgMusic, -1);
}

void cleanUp() {
    Mix_FreeChunk(flapSound);
    Mix_FreeChunk(hitSound);
    Mix_FreeMusic(bgMusic);
    Mix_FreeChunk(scoreSound);
    Mix_FreeChunk(fallSound);
    Mix_FreeChunk(itemSound);

    SDL_DestroyTexture(birdTexture);
    SDL_DestroyTexture(pipeTexture);
    SDL_DestroyTexture(backgroundTexture);
    SDL_DestroyTexture(gameOverTexture);
    SDL_DestroyTexture(startTexture);
    SDL_DestroyTexture(musicOnTexture);
    SDL_DestroyTexture(musicOffTexture);
    SDL_DestroyTexture(smokeTexture);
    SDL_DestroyTexture(itemTexture);
    SDL_DestroyTexture(sparkleTexture);
    if (font) TTF_CloseFont(font);

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);

    Mix_CloseAudio();
    IMG_Quit();
    TTF_Quit();
    SDL_Quit();
}
