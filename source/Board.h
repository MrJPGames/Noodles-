#ifndef Board_H
#define Board_H
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_TTF.h>
#include <string>
#include "GFX.h"

using namespace std;

class Board{
	public:
		Board();
        void rotateTile(int x, int y);
		void init(SDL_Renderer* renderer, TTF_Font* fnt);
		void loadBoard(string s, int i);
		void draw(SDL_Renderer* renderer);
		int getMinMoves();
		bool getIsSolved();
	private:
		void updatePowerState(int x, int y);
		void resetPowerState();
		void setBoardPieces(string s);
		void setBoardOrientations(string s);
		void setBoardSource(string str);
		void setTileConnections(int x, int y);
		void reset();
		void nextOrientation(int x, int y);
		void updateWinState();
		int minMoves, moveCount;
		bool isSolved=false;
		TTF_Font* font;
};

#endif