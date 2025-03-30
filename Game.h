#ifndef GAME_H
#define GAME_H

#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include <SDL_mixer.h>

// Hằng số màn hình
const int SCREEN_WIDTH = 800;
const int SCREEN_HEIGHT = 600;

// Hằng số vật lý
const int GRAVITY = 1;
const int GRAVITY_FALL_MULTIPLIER = 2;
const int FLAP_STRENGTH = -15;

// Hằng số ống
const int PIPE_WIDTH = 70;
const int PIPE_GAP = 150;
const int PIPE_SPEED = 3;
const int PIPE_SPAWN_INTERVAL = 100;
const int PIPE_HEIGHT_MIN = 50;
const int PIPE_HEIGHT_RANGE = SCREEN_HEIGHT - PIPE_GAP - 100;
const int PIPE_OFFSET_AMPLITUDE = 50;
const int PIPE_OFFSET_VELOCITY = 1;
const int PIPE_MOVING_THRESHOLD = 3;

// Hằng số chim
const int BIRD_START_X = 100;
const int BIRD_START_Y = SCREEN_HEIGHT / 2;
const int BIRD_WIDTH = 50;
const int BIRD_HEIGHT = 50;
const int BIRD_SHRUNK_WIDTH = 30;
const int BIRD_SHRUNK_HEIGHT = 30;
const int BIRD_SHRINK_DURATION = 5000; // ms
const int BIRD_FRAME_DELAY = 100; // ms
const int SPRITESHEET_WIDTH = 519;
const int SPRITESHEET_HEIGHT = 130;
const float BIRD_ROTATION_SPEED = 5.0f;
const float BIRD_MAX_ROTATION = 90.0f;
const int BIRD_COLLISION_OFFSET = 5;
const int BIRD_COLLISION_SHRINK = 10;

// Hằng số vật phẩm
const int ITEM_WIDTH = 50;
const int ITEM_HEIGHT = 50;
const int ITEM_SPAWN_INTERVAL = 4;
const int ITEM_TYPE_COUNT = 2;
const int ITEM_SCORE_BONUS = 5;

// Hằng số nền
const int BG_SPEED = 1;

// Hằng số hiệu ứng
const int SMOKE_MAX_SIZE = 80;
const float SMOKE_LIFETIME = 2.0f;
const int SMOKE_VELOCITY_X_RANGE = 7;
const int SMOKE_VELOCITY_Y_RANGE = 5;
const int SMOKE_SIZE_MIN = 30;
const float SMOKE_FADE_RATE = 0.02f;
const int SMOKE_SPAWN_CHANCE = 2;

const int SPARKLE_SIZE = 50;
const float SPARKLE_LIFETIME = 1.0f;
const int SPARKLE_VELOCITY_RANGE = 5;
const float SPARKLE_FADE_RATE = 0.05f;
const int SPARKLE_COUNT = 5;

const int SCREEN_SHAKE_INTENSITY = 100;
const int SCREEN_SHAKE_DURATION = 300; // ms

// Hằng số giao diện
const int MUSIC_BUTTON_WIDTH = 50;
const int MUSIC_BUTTON_HEIGHT = 50;
const int MUSIC_BUTTON_X_OFFSET = 70;
const int MUSIC_BUTTON_Y = 20;

const int SCORE_TEXT_X = 20;
const int SCORE_TEXT_Y = 20;
const int HIGH_SCORE_TEXT_Y = 100;
const int SHRUNK_TEXT_X = 20;
const int SHRUNK_TEXT_Y = 60;

const int START_SCREEN_WIDTH = 300;
const int START_SCREEN_HEIGHT = 200;
const int START_SCREEN_X_OFFSET = 150;
const int START_SCREEN_Y_OFFSET = 100;

const int GAMEOVER_SCREEN_WIDTH = 300;
const int GAMEOVER_SCREEN_HEIGHT = 200;
const int GAMEOVER_SCREEN_X_OFFSET = 150;
const int GAMEOVER_SCREEN_Y_OFFSET = 100;

const int TEXT_BLINK_INTERVAL = 500; // ms
const int FONT_SIZE = 40;

// Hằng số điều chỉnh tốc độ
const float PIPE_SPEED_INCREASE = 0.5f;
const int PIPE_SPAWN_REDUCE = 5;
const int PIPE_SPAWN_MIN = 50;

// Tên file tài nguyên
#define BIRD_SPRITESHEET "bird_spritesheet.png"
#define PIPE_IMAGE "pipe.png"
#define BACKGROUND_IMAGE "background.png"
#define GAMEOVER_IMAGE "gameover.png"
#define START_IMAGE "start.png"
#define MUSIC_ON_IMAGE "music_on.png"
#define MUSIC_OFF_IMAGE "music_off.png"
#define SMOKE_IMAGE "smoke.png"
#define ITEM_IMAGE "item.png"
#define SPARKLE_IMAGE "sparkle.png"
#define FONT_FILE "font.ttf"
#define FLAP_SOUND "flap.mp3"
#define HIT_SOUND "hit.mp3"
#define BG_MUSIC "background.mp3"
#define SCORE_SOUND "score.mp3"
#define FALL_SOUND "fall.mp3"
#define ITEM_SOUND "item.mp3"

// Biến toàn cục
extern int highScore;
extern bool musicOn;
extern float pipeSpeed;
extern int pipeSpawnInterval;
extern SDL_Color textColor;
extern SDL_Window* window;
extern SDL_Renderer* renderer;
extern SDL_Texture* birdTexture;
extern SDL_Texture* pipeTexture;
extern SDL_Texture* backgroundTexture;
extern SDL_Texture* gameOverTexture;
extern SDL_Texture* startTexture;
extern SDL_Texture* musicOnTexture;
extern SDL_Texture* musicOffTexture;
extern SDL_Texture* smokeTexture;
extern SDL_Texture* itemTexture;
extern SDL_Texture* sparkleTexture;
extern TTF_Font* font;
extern Mix_Chunk* flapSound;
extern Mix_Chunk* hitSound;
extern Mix_Music* bgMusic;
extern Mix_Chunk* scoreSound;
extern Mix_Chunk* fallSound;
extern Mix_Chunk* itemSound;
extern int bgX;
void loadHighScore();
void saveHighScore(int currentScore);
void initSDL();
void loadAssets();
void cleanUp();

#endif
