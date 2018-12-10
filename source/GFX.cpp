#include "GFX.h"

void renderTexture(SDL_Renderer* renderer, SDL_Texture *tex, int destX, int destY){
    int w, h;
    SDL_QueryTexture(tex, NULL, NULL, &w, &h);
	SDL_Rect dest;
	dest.x = destX;
	dest.y = destY;
    dest.w = w;
    dest.h = h;

	SDL_RenderCopy(renderer, tex, NULL, &dest);
}

void renderTextureRotated(SDL_Renderer* renderer, SDL_Texture* tex, int destX, int destY, int rotation){
    int w, h;
    SDL_QueryTexture(tex, NULL, NULL, &w, &h);
	SDL_Rect dest;
	dest.x = destX;
	dest.y = destY;
	dest.w = w;
	dest.h = h;

	SDL_RenderCopyEx(renderer, tex, NULL, &dest, rotation, NULL, SDL_FLIP_NONE);
}

void renderTextureScaled(SDL_Renderer* renderer, SDL_Texture *tex, int destX, int destY, double scale){
    int w, h;
    SDL_QueryTexture(tex, NULL, NULL, &w, &h);
	SDL_Rect dest;
	dest.x = destX;
	dest.y = destY;
	dest.w = round(w*scale);
	dest.h = round(h*scale);

	SDL_RenderCopy(renderer, tex, NULL, &dest);
}

void renderTextureScaledRotated(SDL_Renderer* renderer, SDL_Texture *tex, int destX, int destY, double scale, int rotation){	
    int w, h;
    SDL_QueryTexture(tex, NULL, NULL, &w, &h);
	SDL_Rect dest;
	dest.x = destX;
	dest.y = destY;
	dest.w = round(w*scale);
	dest.h = round(h*scale);

	SDL_RenderCopyEx(renderer, tex, NULL, &dest, rotation, NULL, SDL_FLIP_NONE);
}