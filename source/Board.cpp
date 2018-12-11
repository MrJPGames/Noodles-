#include "Board.h"
#include <string>

using namespace std;

#define TILE_TYPES 4

typedef struct 
{
	SDL_Texture * texture;
} 
images;
images tileTex[TILE_TYPES], tileTexOn[TILE_TYPES];


typedef struct{
    int tileType;
    int orientation;
	//udlr
    bool connections[4];
	bool turnedOn;
}tile;

typedef struct{
	int x;
	int y;
}position;

tile board[5][5];
position source;

Board::Board(){
	int k=0;
	for (int i=0; i<5; i++){
		for (int j=0; j<5; j++){
			board[i][j].tileType = k;
			k++;
			if (k >= TILE_TYPES)
				k=0;
			board[i][j].orientation = 0;
			if (board[i][j].tileType == 0){
				board[i][j].connections[0] = false;
				board[i][j].connections[1] = true;
				board[i][j].connections[2] = false;
				board[i][j].connections[3] = true;
			}
			if (board[i][j].tileType == 1){
				board[i][j].connections[0] = false;
				board[i][j].connections[1] = false;
				board[i][j].connections[2] = true;
				board[i][j].connections[3] = true;
			}
			if (board[i][j].tileType == 2){
				board[i][j].connections[0] = false;
				board[i][j].connections[1] = true;
				board[i][j].connections[2] = true;
				board[i][j].connections[3] = true;
			}
			if (board[i][j].tileType == 3){
				board[i][j].connections[0] = false;
				board[i][j].connections[1] = true;
				board[i][j].connections[2] = false;
				board[i][j].connections[3] = false;
			}
			board[i][j].turnedOn = false;
		}
	}
	source.x=2;
	source.y=4;
}

void Board::init(SDL_Renderer* renderer){
	IMG_Init(IMG_INIT_PNG);
	SDL_Surface *   surface;
	for (int i=0; i<TILE_TYPES; i++){
		string off = "romfs:/images/tile" + to_string(i) + ".png";
		string on = "romfs:/images/tileOn" + to_string(i) + ".png";
		surface = IMG_Load(off.c_str());
		tileTex[i].texture = SDL_CreateTextureFromSurface(renderer, surface);
		SDL_FreeSurface(surface);
		
		surface = IMG_Load(on.c_str());
		tileTexOn[i].texture = SDL_CreateTextureFromSurface(renderer, surface);
		SDL_FreeSurface(surface);
	}
}

void Board::resetPowerState(){
	for (int x=0; x<5; x++){
		for (int y=0; y<5; y++){
			board[x][y].turnedOn = false;
		}
	}
}

void Board::updatePowerState(int x, int y){
	//Already activated tiles don't need to be updated again
	if (board[x][y].turnedOn)
		return;
	board[x][y].turnedOn = true;
	//Up (other tile down)
	if (board[x][y].connections[0] && y > 0 && board[x][y-1].connections[1])
		updatePowerState(x,y-1);

	//Down (other tile up)
	if (board[x][y].connections[1] && y < 4 && board[x][y+1].connections[0])
		updatePowerState(x,y+1);
		
	//Left (other tile right)	
	if (board[x][y].connections[2] && x > 0 && board[x-1][y].connections[3])
		updatePowerState(x-1,y);

	//Right (other tile left)
	if (board[x][y].connections[3] && x < 4 && board[x+1][y].connections[2])
		updatePowerState(x+1,y);
}

void Board::rotateTile(int x, int y){
	resetPowerState();
	board[x][y].orientation++;
	if (board[x][y].orientation > 3)
		board[x][y].orientation = 0;

	//udlr
	bool q,w,e,r;
	q=board[x][y].connections[0];
	w=board[x][y].connections[1];
	e=board[x][y].connections[2];
	r=board[x][y].connections[3];
	board[x][y].connections[0]=e;
	board[x][y].connections[1]=r;
	board[x][y].connections[2]=w;
	board[x][y].connections[3]=q;

	updatePowerState(source.x, source.y);
}

void Board::draw(SDL_Renderer* renderer){
	for (int i=0; i<5; i++){
		for (int j=0; j<5; j++){
			int k=board[i][j].tileType;
			if (board[i][j].turnedOn)
				renderTextureRotated(renderer, tileTexOn[k].texture, 332+124*i, 52+124*j, board[i][j].orientation*90);
			else
				renderTextureRotated(renderer, tileTex[k].texture, 332+124*i, 52+124*j, board[i][j].orientation*90);
		}
	}
}