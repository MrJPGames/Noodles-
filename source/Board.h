#ifndef Board_H
#define Board_H
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <string>
#include "GFX.h"

using namespace std;

class Board{
	public:
		Board();
        void rotateTile(int x, int y);
		void init(SDL_Renderer* renderer);
		void draw(SDL_Renderer* renderer);
	private:
		void updatePowerState(int x, int y);
		void resetPowerState();
};

#endif