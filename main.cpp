#include <SDL_image.h>
#include <SDL_mixer.h>
#include <SDL_ttf.h>
#include <iostream>
#include <vector>
#include <ctime>


const int SCREEN_WIDTH = 800;
const int SCREEN_HEIGHT = 600;
const int GRAVITY = 1;
const int FLAP_STRENGTH = -15;
const int PIPE_WIDTH = 70;
const int PIPE_GAP = 150;
const int PIPE_SPEED = 3;
int highScore = 0;
bool musicOn = true;
SDL_Color textColor = {255, 255, 255};
SDL_Window* window = nullptr;
SDL_Renderer* renderer = nullptr;
SDL_Texture* birdTexture = nullptr;
SDL_Texture* pipeTexture = nullptr;
SDL_Texture* backgroundTexture = nullptr;
SDL_Texture* gameOverTexture = nullptr;
SDL_Texture* startTexture = nullptr; // Ảnh Start
SDL_Texture* musicOnTexture = nullptr;
SDL_Texture* musicOffTexture = nullptr;
TTF_Font* font = nullptr;  // Khai báo font toàn cục

// Âm thanh
Mix_Chunk* flapSound = nullptr;
Mix_Chunk* hitSound = nullptr;
Mix_Music* bgMusic = nullptr;
Mix_Chunk* scoreSound = nullptr;  // Thêm biến toàn cục
int bgX = 0;
const int BG_SPEED = 1; // Tốc độ cuộn background
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

    void flap() {
        velocity = FLAP_STRENGTH;
        Mix_PlayChannel(-1, flapSound, 0);  // Phát âm thanh khi nhảy
    }

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
// Thêm nút nhạc
SDL_Rect musicButton = {SCREEN_WIDTH - 70, 20, 50, 50};

// Xử lý bật/tắt nhạc
void toggleMusic() {
    musicOn = !musicOn;
    if (musicOn) {
        Mix_PlayMusic(bgMusic, -1);
    } else {
        Mix_HaltMusic();
    }
}

struct Pipe {
    int x, height;
    SDL_Rect topRect, bottomRect;
    bool passed;
    int yOffset = 0;
    int yVelocity = 1;
    bool isMoving;

    static int pipeCount;

    Pipe(int startX) {
        x = startX;
        height = rand() % (SCREEN_HEIGHT - PIPE_GAP - 100) + 50;
        passed = false;
        isMoving = (pipeCount >= 3);
        pipeCount++;

        updateRects();
    }

    void updateRects() {
        topRect = {x, yOffset - 50, PIPE_WIDTH, height};
        bottomRect = {x, height + PIPE_GAP + yOffset + 50, PIPE_WIDTH, SCREEN_HEIGHT - (height + PIPE_GAP)};
    }

    void update() {
        x -= PIPE_SPEED;

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

int Pipe::pipeCount = 0;
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

        SDL_Rect startRect = {SCREEN_WIDTH / 2 - 150, SCREEN_HEIGHT / 2 - 100, 300, 200};
        SDL_RenderCopy(renderer, startTexture, NULL, &startRect);

        // Hiển thị nút nhạc
        if (musicOn)
            SDL_RenderCopy(renderer, musicOnTexture, NULL, &musicButton);
        else
            SDL_RenderCopy(renderer, musicOffTexture, NULL, &musicButton);

        SDL_RenderPresent(renderer);
    }
}

void renderHighScore() {
    if (!font) font = TTF_OpenFont("font.ttf", 40);

    std::string highScoreText = "High Score: " + std::to_string(highScore);
    SDL_Surface* surface = TTF_RenderText_Solid(font, highScoreText.c_str(), textColor);
    SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);

    SDL_Rect textRect = {SCREEN_WIDTH / 2 - surface->w / 2, 100, surface->w, surface->h};
    SDL_RenderCopy(renderer, texture, NULL, &textRect);

    SDL_FreeSurface(surface);
    SDL_DestroyTexture(texture);
}
void renderScore(int score) {
    if (!font) font = TTF_OpenFont("font.ttf", 40);

    std::string scoreText = "Score: " + std::to_string(score);
    SDL_Surface* surface = TTF_RenderText_Solid(font, scoreText.c_str(), textColor);
    SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);

    SDL_Rect textRect = {20, 20, surface->w, surface->h};
    SDL_RenderCopy(renderer, texture, NULL, &textRect);

    SDL_FreeSurface(surface);
    SDL_DestroyTexture(texture);
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
                    firstPress = true;  // Ấn lần đầu tiên
                } else {
                    waiting = false;  // Ấn lần thứ hai để thoát khỏi vòng lặp
                }
            }
        }

        SDL_RenderClear(renderer);
        SDL_Rect bgRect1 = {bgX, 0, SCREEN_WIDTH, SCREEN_HEIGHT};
SDL_Rect bgRect2 = {bgX + SCREEN_WIDTH, 0, SCREEN_WIDTH, SCREEN_HEIGHT};

// Vẽ hai background liên tiếp để tạo hiệu ứng cuộn vô tận
SDL_RenderCopy(renderer, backgroundTexture, NULL, &bgRect1);
SDL_RenderCopy(renderer, backgroundTexture, NULL, &bgRect2);

        SDL_Rect gameOverRect = {SCREEN_WIDTH / 2 - 150, SCREEN_HEIGHT / 2 - 100, 300, 200};
        SDL_RenderCopy(renderer, gameOverTexture, NULL, &gameOverRect);
         // Hiển thị điểm số
        renderScore(score);

        // Hiển thị điểm cao nhất
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
    if (TTF_Init() == -1) {  // Thêm dòng này
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
    birdTexture = IMG_LoadTexture(renderer, "bird_spritesheet.png");
    pipeTexture = IMG_LoadTexture(renderer, "pipe.png");
    backgroundTexture = IMG_LoadTexture(renderer, "background.png");
    gameOverTexture = IMG_LoadTexture(renderer, "gameover.png");
    startTexture = IMG_LoadTexture(renderer, "start.png");
    musicOnTexture = IMG_LoadTexture(renderer, "music_on.png");
    musicOffTexture = IMG_LoadTexture(renderer, "music_off.png");

    flapSound = Mix_LoadWAV("flap.mp3");
    hitSound = Mix_LoadWAV("hit.mp3");
    bgMusic = Mix_LoadMUS("background.mp3");
    scoreSound = Mix_LoadWAV("score.mp3");
if (!scoreSound) {
    std::cerr << "Failed to load score sound: " << Mix_GetError() << std::endl;
    exit(1);
}
    if (!birdTexture || !pipeTexture || !backgroundTexture || !gameOverTexture || !startTexture || !flapSound || !hitSound || !bgMusic) {
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

    SDL_DestroyTexture(birdTexture);
    SDL_DestroyTexture(pipeTexture);
    SDL_DestroyTexture(backgroundTexture);
    SDL_DestroyTexture(gameOverTexture);
    SDL_DestroyTexture(startTexture);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);

    Mix_CloseAudio();
    IMG_Quit();
    SDL_Quit();
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
                // Cập nhật vị trí background
bgX -= BG_SPEED;
if (bgX <= -SCREEN_WIDTH) {
    bgX = 0; // Reset background khi cuộn hết
}
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
                    Mix_PlayChannel(-1, hitSound, 0);  // Phát âm thanh khi thua
                    if (score > highScore) {
    highScore = score;  // Cập nhật high score mới
}
                    showGameOverScreen(score);
                }
                if (!pipe.passed && bird.x > pipe.x + PIPE_WIDTH/2) {
                    score++;
                    pipe.passed = true;
                    Mix_PlayChannel(-1, scoreSound, 0);  // Phát âm thanh khi ghi điểm
                }
            }

            if (bird.y > SCREEN_HEIGHT) {
                running = false;
                Mix_PlayChannel(-1, hitSound, 0);
                if (score > highScore) {
    highScore = score;  // Cập nhật high score mới
}
                showGameOverScreen(score);
            }

            SDL_RenderClear(renderer);
            SDL_Rect bgRect1 = { bgX, 0, SCREEN_WIDTH, SCREEN_HEIGHT };
    SDL_Rect bgRect2 = { bgX + SCREEN_WIDTH, 0, SCREEN_WIDTH, SCREEN_HEIGHT };
    SDL_RenderCopy(renderer, backgroundTexture, NULL, &bgRect1);
    SDL_RenderCopy(renderer, backgroundTexture, NULL, &bgRect2);
            bird.render(renderer, birdTexture);
            for (auto& pipe : pipes) pipe.render(renderer, pipeTexture);
            renderScore(score);  // Hiển thị điểm số
            SDL_RenderPresent(renderer);
            SDL_Delay(16);
        }
    }

    cleanUp();
    return 0;
}
