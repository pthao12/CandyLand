#ifndef HANDLE_BUTTON_H
#define HANDLE_BUTTON_H

#include "LButton.h"
#include "base.h"

void loadChunk(string path);

void HandlePlayButton(SDL_Event* e, LButton& playButton, bool& play , bool& pause, bool& restart, SDL_Rect* buttonSize);

void HandleSettingButton(SDL_Event* e, LButton& settingButton, bool& pause, bool& music, bool& setting, SDL_Rect* buttonSize, SDL_Renderer *gRenderer, int &pausTime);

void HandleResumeButton(SDL_Event* e, LButton& resume, bool& pause, bool& setting, SDL_Rect* buttonSize, SDL_Renderer *gRenderer, int& pauseTime);

void HandleHomeButton(SDL_Event* e, LButton& home, bool& played, bool& restart, bool& setting, SDL_Rect* buttonSize, SDL_Renderer *gRenderer);

void HandleRestartButton(SDL_Event* e, LButton& restartGame, bool& restart, bool& pause, bool& endG, bool& setting, SDL_Rect* buttonSize, SDL_Renderer *gRenderer);

void HandleUnmuteButton(SDL_Event* e, LButton& unmute, bool& music, SDL_Rect* buttonSize, SDL_Renderer *gRenderer);

void HandleMuteButton(SDL_Event* e, LButton& mute, bool& music, SDL_Rect* buttonSize, SDL_Renderer *gRenderer);

void HandleExitButton(SDL_Event* e, LButton& exit, SDL_Rect* buttonSize, SDL_Renderer *gRenderer);

#endif
