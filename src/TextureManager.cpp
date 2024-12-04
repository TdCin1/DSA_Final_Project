#include "TextureManager.h"

using namespace std;

SDL_Texture *TextureManager::LoadTexture(SDL_Renderer* ren, const char *fileName) {

    SDL_Surface* tempSurface = IMG_Load(fileName);

    if(!tempSurface){
        throw runtime_error("File Not Found " + string(SDL_GetError()) );
    }

    SDL_Texture* tex = SDL_CreateTextureFromSurface(ren,tempSurface);

    SDL_FreeSurface(tempSurface);

    return tex;
}

void TextureManager::Draw(SDL_Renderer* ren, SDL_Texture *tex, SDL_Rect src, SDL_Rect dest) {
    SDL_RenderCopy(ren, tex, &src, &dest);

}
