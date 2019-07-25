#ifndef Menu_H
#define Menu_H
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_TTF.h>
#include <string>
#include "GFX.h"

using namespace std;

#define MAIN_STATE 0
#define LVLS_STATE 1 //LeVeL Select

class Menu{
	public:
		Menu();
        void init(TTF_Font* font, TTF_Font* bFnt, string inFile);
        void draw(SDL_Renderer* renderer);
        void moveSelection(int dir);
        bool selectOption();
        int getSelectedLevel();
	private:
        int menuPos = 0;
        int levelCount = -1;
	TTF_Font* font;
	TTF_Font* b_font;
	int state = LVLS_STATE;
};

#endif