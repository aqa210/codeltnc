#include <SDL_image.h>
#include <iostream>
#include <vector>
#include <ctime>

const int SCREEN_WIDTH = 800;
const int SCREEN_HEIGHT = 600;
const int GRAVITY = 1;
const int FLAP_STRENGTH = -15;
const int PIPE_WIDTH = 70;
const int PIPE_GAP = 180;
const int PIPE_SPEED = 3;

SDL_Window* window = nullptr;
SDL_Renderer* renderer = nullptr;
SDL_Texture* birdTexture = nullptr;
SDL_Texture* pipeTexture = nullptr;
SDL_Texture* backgroundTexture = nullptr;
SDL_Texture* gameOverTexture = nullptr;
SDL_Texture* startTexture = nullptr; // Ảnh Start

struct Bird {
    int x, y, velocity;
    SDL_Rect rect;
    SDL_Rect spriteClips[3];
    int frame = 0;
    Uint32 lastFrameTime = 0;

    Bird() {
        x = 100;
        y = SCREEN_HEIGHT / 2;
        velocity = 0;
        rect = {x, y, 70, 70};

        spriteClips[0] = { 0, 0, 520/3, 165 };
        spriteClips[1] = { 520/3, 0, 520/3, 165 };
        spriteClips[2] = { 520*2/3, 0, 520/3, 165 };
    }

    void flap() { velocity = FLAP_STRENGTH; }

    void update() {
        velocity += GRAVITY;
        y += velocity;
        rect.y = y;

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
    int yOffset = 0;       // Độ lệch dọc để di chuyển lên xuống
    int yVelocity = 1;     // Tốc độ di chuyển lên/xuống
    bool isMoving;         // Xác định cột này có di chuyển không

    static int pipeCount;  // Đếm số cột đã xuất hiện

    Pipe(int startX) {
        x = startX;
        height = rand() % (SCREEN_HEIGHT - PIPE_GAP - 100) + 50;
        passed = false;

        // Sau một số cột đầu tiên (ví dụ 3 cột), các cột mới sẽ bắt đầu di chuyển
        isMoving = (pipeCount >= 3);
        pipeCount++;

        updateRects();
    }

    void updateRects() {
        topRect = {x, yOffset -50 , PIPE_WIDTH, height};
        bottomRect = {x, height + PIPE_GAP + yOffset +50 , PIPE_WIDTH, SCREEN_HEIGHT - (height + PIPE_GAP)};
    }

    void update() {
        x -= PIPE_SPEED;  // Di chuyển sang trái

        // Chỉ di chuyển lên xuống nếu isMoving == true
        if (isMoving) {
            yOffset += yVelocity;
            if (yOffset > 50 || yOffset < -50) {
                yVelocity = -yVelocity;
            }
        }

        updateRects();
    }

    bool checkCollision(SDL_Rect bird) {
        return SDL_HasIntersection(&bird, &topRect) || SDL_HasIntersection(&bird, &bottomRect);
    }

    void render(SDL_Renderer* renderer, SDL_Texture* texture) {
        SDL_RenderCopy(renderer, texture, NULL, &bottomRect);
        SDL_RenderCopyEx(renderer, texture, NULL, &topRect, 180, NULL, SDL_FLIP_NONE);
    }
};

// Khởi tạo biến đếm số cột
int Pipe::pipeCount = 0;
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
    birdTexture = IMG_LoadTexture(renderer, "bird_spritesheet.png");
    pipeTexture = IMG_LoadTexture(renderer, "pipe.png");
    backgroundTexture = IMG_LoadTexture(renderer, "background.png");
    gameOverTexture = IMG_LoadTexture(renderer, "gameover.png");
    startTexture = IMG_LoadTexture(renderer, "start.png");

    if (!birdTexture || !pipeTexture || !backgroundTexture || !gameOverTexture || !startTexture) {
        std::cerr << "Failed to load images!" << std::endl;
        exit(1);
    }
}

void cleanUp() {
    SDL_DestroyTexture(birdTexture);
    SDL_DestroyTexture(pipeTexture);
    SDL_DestroyTexture(backgroundTexture);
    SDL_DestroyTexture(gameOverTexture);
    SDL_DestroyTexture(startTexture);
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

        SDL_Rect startRect = {SCREEN_WIDTH / 2 - 150, SCREEN_HEIGHT / 2 - 100, 300, 200};
        SDL_RenderCopy(renderer, startTexture, NULL, &startRect);

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

        SDL_Rect gameOverRect = {SCREEN_WIDTH / 2 - 150, SCREEN_HEIGHT / 2 - 100, 300, 200};
        SDL_RenderCopy(renderer, gameOverTexture, NULL, &gameOverRect);

        SDL_RenderPresent(renderer);
    }
}

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
            for (auto& pipe : pipes) pipe.render(renderer, pipeTexture);
            SDL_RenderPresent(renderer);
            SDL_Delay(16);
        }

        showGameOverScreen(score);
    }

    cleanUp();
    return 0;
}
