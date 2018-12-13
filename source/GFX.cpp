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

void renderText(SDL_Renderer* renderer, TTF_Font* fnt, int destX, int destY, string s){
    if (fnt == NULL)
        return;
    SDL_Color White = {0, 0, 0};  
    SDL_Surface* surfaceMessage = TTF_RenderText_Blended_Wrapped(fnt, s.c_str(), White, 1280); 
    SDL_Texture* Message = SDL_CreateTextureFromSurface(renderer, surfaceMessage); //now you can convert it into a texture

    int w,h;
    w = surfaceMessage->w;
    h = surfaceMessage->h;
    SDL_Rect rect;
    rect.x = 0;
    rect.y = 0;
    rect.w = w;
    rect.h = h;

	SDL_Rect dest;
	dest.x = destX;
	dest.y = destY;
	dest.w = w;
	dest.h = h;

    SDL_RenderCopy(renderer, Message, &dest, &rect); 

    //Free surface and texture
    SDL_FreeSurface(surfaceMessage);
    SDL_DestroyTexture(Message);
}