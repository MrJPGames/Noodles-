#ifndef GFX_H
#define GFX_H
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>


void renderTexture(SDL_Renderer* renderer, SDL_Texture *tex, int destX, int destY);
void renderTextureRotated(SDL_Renderer* renderer, SDL_Texture* tex, int destX, int destY, int rotation);
void renderTextureScaled(SDL_Renderer* renderer, SDL_Texture *tex, int destX, int destY, double scale);
void renderTextureScaledRotated(SDL_Renderer* renderer, SDL_Texture *tex, int destX, int destY, double scale, int rotation);

#endif