#ifndef Menu_H
#define Menu_H
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_TTF.h>
#include <string>
#include "GFX.h"

using namespace std;

class Menu{
	public:
		Menu();
        void draw();
	private:
        int menuPos = 0;
        int levelCount = 0;
		TTF_Font* font;
};

#endif