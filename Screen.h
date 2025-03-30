#ifndef SCREEN_H
#define SCREEN_H

#include "Game.h"

extern SDL_Rect musicButton;

void toggleMusic();
void showStartScreen();
void showGameOverScreen(int score);
void showPauseScreen();

#endif
