#include <SDL.h>
#include <SDL_image.h>
#include <iostream>
#include <vector>
#include <ctime>

const int SCREEN_WIDTH = 800;
const int SCREEN_HEIGHT = 600;
const int GRAVITY = 1;
const int FLAP_STRENGTH = -15;
const int PIPE_WIDTH = 80;
const int PIPE_GAP = 200;
const int PIPE_SPEED = 3;

SDL_Window* window = nullptr;
SDL_Renderer* renderer = nullptr;
SDL_Texture* birdTexture = nullptr;
SDL_Texture* pipeTexture = nullptr;
SDL_Texture* backgroundTexture = nullptr;

struct Bird {
    int x, y, velocity;
    SDL_Rect rect;
    SDL_Rect spriteClips[3]; // 3 frame animation
    int frame = 0; // Frame hiện tại
    Uint32 lastFrameTime = 0; // Lưu thời gian đổi frame

    Bird() {
        x = 100;
        y = SCREEN_HEIGHT / 2;
        velocity = 0;
        rect = {x, y, 50, 50};

        // Xác định từng frame từ spritesheet
        spriteClips[0] = { 0, 0, 529/3, 143 };   // Frame 1 (cánh xuống)
        spriteClips[1] = { 529/3, 0, 529/3, 143 };  // Frame 2 (cánh ngang)
        spriteClips[2] = { 529*2/3,0, 529/3 , 143};  // Frame 3 (cánh lên)
    }

    void flap() { velocity = FLAP_STRENGTH; }

    void update() {
        velocity += GRAVITY;
        y += velocity;
        rect.y = y;

        // Cập nhật frame animation sau mỗi 100ms
        if (SDL_GetTicks() - lastFrameTime > 100) {
            frame = (frame + 1) % 3;
            lastFrameTime = SDL_GetTicks();
        }
    }

    void render(SDL_Renderer* renderer, SDL_Texture* texture) {
        SDL_RenderCopy(renderer, texture, &spriteClips[frame], &rect);
    }
};

struct Pipe {
    int x, height;
    SDL_Rect topRect, bottomRect;
    bool passed;

    Pipe(int startX) {
        x = startX;
        height = rand() % (SCREEN_HEIGHT - PIPE_GAP - 100) + 50;

        int offset = 10; // Điều chỉnh vị trí ống lên trên
        topRect = {x, 0, PIPE_WIDTH, height};
        bottomRect = {x, height + PIPE_GAP - offset, PIPE_WIDTH, SCREEN_HEIGHT - (height + PIPE_GAP) + offset};

        passed = false;
    }

    void update() {
        x -= PIPE_SPEED;
        topRect.x = x;
        bottomRect.x = x;
    }

    bool checkCollision(SDL_Rect bird) {
        return SDL_HasIntersection(&bird, &topRect) || SDL_HasIntersection(&bird, &bottomRect);
    }

    void render(SDL_Renderer* renderer, SDL_Texture* texture) {
        SDL_RenderCopy(renderer, texture, NULL, &bottomRect);
        SDL_RenderCopyEx(renderer, texture, NULL, &topRect, 180, NULL, SDL_FLIP_NONE);
    }
};
void initSDL() {
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        std::cerr << "SDL Initialization Failed: " << SDL_GetError() << std::endl;
        exit(1);
    }
    if (!(IMG_Init(IMG_INIT_PNG) & IMG_INIT_PNG)) {
        std::cerr << "SDL_image Initialization Failed: " << IMG_GetError() << std::endl;
        exit(1);
    }
    window = SDL_CreateWindow("Flappy Bird", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
    if (!window) {
        std::cerr << "SDL Window Error: " << SDL_GetError() << std::endl;
        exit(1);
    }
    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    if (!renderer) {
        std::cerr << "SDL Renderer Error: " << SDL_GetError() << std::endl;
        exit(1);
    }
}

void loadAssets() {
    birdTexture = IMG_LoadTexture(renderer, "bird_spritesheet.png"); // Sử dụng spritesheet
    pipeTexture = IMG_LoadTexture(renderer, "pipe.png");
    backgroundTexture = IMG_LoadTexture(renderer, "background.jpg");

    if (!birdTexture || !pipeTexture || !backgroundTexture) {
        std::cerr << "Failed to load images!" << std::endl;
        exit(1);
    }
}

void cleanUp() {
    SDL_DestroyTexture(birdTexture);
    SDL_DestroyTexture(pipeTexture);
    SDL_DestroyTexture(backgroundTexture);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    IMG_Quit();
    SDL_Quit();
}

void showStartScreen() {
    bool waiting = true;
    SDL_Event event;

    while (waiting) {
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) exit(0);
            if (event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_SPACE) waiting = false;
        }

        SDL_RenderClear(renderer);
        SDL_RenderCopy(renderer, backgroundTexture, NULL, NULL);
        SDL_RenderPresent(renderer);
    }
}

void showGameOverScreen(int score) {
    bool waiting = true;
    SDL_Event event;

    while (waiting) {
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) exit(0);
            if (event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_SPACE) waiting = false;
        }

        SDL_RenderClear(renderer);
        SDL_RenderCopy(renderer, backgroundTexture, NULL, NULL);
        SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
        SDL_Rect textRect = {SCREEN_WIDTH / 2 - 150, SCREEN_HEIGHT / 2 - 50, 300, 100};
        SDL_RenderFillRect(renderer, &textRect);
        SDL_RenderPresent(renderer);
    }
}

int main(int argc, char* argv[]) {
    srand(time(0));
    initSDL();
    loadAssets();

    while (true) {
        showStartScreen();

        bool running = true;
        SDL_Event event;
        Bird bird;
        std::vector<Pipe> pipes;
        int pipeSpawnTimer = 0;
        int score = 0;

        while (running) {
            while (SDL_PollEvent(&event)) {
                if (event.type == SDL_QUIT) exit(0);
                if (event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_SPACE) bird.flap();
            }

            bird.update();

            if (pipeSpawnTimer++ > 100) {
                pipes.emplace_back(SCREEN_WIDTH);
                pipeSpawnTimer = 0;
            }

            for (auto& pipe : pipes) {
                pipe.update();
                if (pipe.checkCollision(bird.rect)) {
                    running = false;
                }
                if (!pipe.passed && bird.x > pipe.x + PIPE_WIDTH) {
                    score++;
                    pipe.passed = true;
                }
            }

            if (bird.y > SCREEN_HEIGHT) {
                running = false;
            }

            SDL_RenderClear(renderer);
            SDL_RenderCopy(renderer, backgroundTexture, NULL, NULL);

            bird.render(renderer, birdTexture);

            for (auto& pipe : pipes) {
                pipe.render(renderer, pipeTexture);
            }

            SDL_RenderPresent(renderer);
            SDL_Delay(16);
        }

        showGameOverScreen(score);
    }

    cleanUp();
    return 0;
}
