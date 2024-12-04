#pragma once
#include "Game.h"
using namespace std;

class TextureManager{
public:
    static SDL_Texture* LoadTexture(SDL_Renderer* ren, const char* fileName);
    static void Draw(SDL_Renderer* ren,SDL_Texture* tex, SDL_Rect src, SDL_Rect dest);

};
