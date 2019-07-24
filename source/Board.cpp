#include "Board.h"
#include <json-c/json.h>
#include <fstream>
#include <streambuf>

using namespace std;

#define TILE_TYPES 4

typedef struct 
{
	SDL_Texture * texture;
} 
images;
images tileTex[TILE_TYPES], tileTexOn[TILE_TYPES], tileTexPS[TILE_TYPES];


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
	reset();
}

void Board::reset(){
	isSolved=false;
	moveCount = 0;
	for (int i=0; i<5; i++){
		for (int j=0; j<5; j++){
			board[i][j].tileType = 0;
			board[i][j].orientation = 0;
			setTileConnections(i,j);
			board[i][j].turnedOn = false;
		}
	}
	source.x=0;
	source.y=0;
	updatePowerState(source.x, source.y);
}

void Board::setTileConnections(int x, int y){
	board[x][y].connections[0] = false;
	board[x][y].connections[1] = false;
	board[x][y].connections[2] = false;
	board[x][y].connections[3] = false;
	switch (board[x][y].tileType){
		case 0:
			board[x][y].connections[1] = true;
			board[x][y].connections[3] = true;
			break;
		case 1:
			board[x][y].connections[2] = true;
			board[x][y].connections[3] = true;
			break;
		case 2:
			board[x][y].connections[1] = true;
			board[x][y].connections[2] = true;
			board[x][y].connections[3] = true;
			break;
		case 3:	
			board[x][y].connections[1] = true;
			break;
	}
}

void Board::init(SDL_Renderer* renderer, TTF_Font* fnt, string inFile){
	font = fnt;
	IMG_Init(IMG_INIT_PNG);
	SDL_Surface *   surface;
	for (int i=0; i<TILE_TYPES; i++){
		string off = "romfs:/images/tile" + to_string(i) + ".png";
		string on = "romfs:/images/tileOn" + to_string(i) + ".png";
		string ps = "romfs:/images/tilePS" + to_string(i) + ".png";
		surface = IMG_Load(off.c_str());
		tileTex[i].texture = SDL_CreateTextureFromSurface(renderer, surface);
		SDL_FreeSurface(surface);
		
		surface = IMG_Load(on.c_str());
		tileTexOn[i].texture = SDL_CreateTextureFromSurface(renderer, surface);
		SDL_FreeSurface(surface);

		surface = IMG_Load(ps.c_str());
		tileTexPS[i].texture = SDL_CreateTextureFromSurface(renderer, surface);
		SDL_FreeSurface(surface);
	}

	//Init total level count
	std::ifstream t(inFile);
	std::string jsonText((std::istreambuf_iterator<char>(t)), std::istreambuf_iterator<char>());

	json_object * jobj = json_tokener_parse(jsonText.c_str());

    levelCount = json_object_array_length(jobj);
	json_object_put(jobj);
}

void Board::loadBoard(string inFile, int puzzleNum){
	reset();
	currentLevel = puzzleNum;
	//Read file
	std::ifstream t(inFile);
	std::string jsonText((std::istreambuf_iterator<char>(t)), std::istreambuf_iterator<char>());

	json_object * jobj = json_tokener_parse(jsonText.c_str());

	json_object * puzzleJson;
	if (json_object_is_type(jobj, json_type_array)){
		board[0][0].tileType = 1; 
		puzzleJson = json_object_array_get_idx(jobj, puzzleNum);

		json_object_object_foreach(puzzleJson, key, val) {
			if (json_object_is_type(val, json_type_int)) {
				minMoves = json_object_get_int(val);
			}
			if (json_object_is_type(val, json_type_string)) {
				string stringValue(json_object_get_string(val));
				if (strcmp(key, "pieces") == 0){
					setBoardPieces(stringValue);
				}else if (strcmp(key, "orientations") == 0){
					setBoardOrientations(stringValue);
				}else if (strcmp(key, "source") == 0){
					setBoardSource(stringValue);
				}
			}
		}
	}

	for (int x=0; x<5; x++){
		for (int y=0; y<5; y++){
			for (int i=board[x][y].orientation; i>0; i--){
				nextOrientation(x,y);
			}
		}
	}

	resetPowerState();
	updatePowerState(source.x, source.y);

	//Free memory after use
	json_object_put(jobj);
}

void Board::loadNextBoard(string s){
	if (currentLevel < levelCount-1){
		loadBoard(s, currentLevel+1);
	}else{
		loadBoard(s, 0);
	}
}

void Board::setBoardPieces(string str){
	for (int x=0; x<5; x++){
		for (int y=0; y<5; y++){
			board[x][y].tileType = (str.at(y*5+x) - '0');
			setTileConnections(x,y);
		}
	}
}

void Board::setBoardOrientations(string str){
	for (int x=0; x<5; x++){
		for (int y=0; y<5; y++){
			int ori=(str.at(y*5+x) - '0');
			board[x][y].orientation = ori;
		}
	}
}

void Board::setBoardSource(string str){
	source.x = (str.at(1) - '0');
	source.y = (str.at(0) - '0');
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
	moveCount++;
	board[x][y].orientation++;
	nextOrientation(x,y);

	updatePowerState(source.x, source.y);
	updateWinState();
}

void Board::updateWinState(){
	for (int x=0; x<5; x++){
		for (int y=0; y<5; y++){
			if (!board[x][y].turnedOn)
				return;
		}
	}
	isSolved = true;
}

bool Board::getIsSolved(){
	return isSolved;
}

void Board::nextOrientation(int x, int y){
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
}

void Board::draw(SDL_Renderer* renderer){
	renderText(renderer, font, 20, 20, "Level: " + to_string(currentLevel+1) + "\nMoves: " + to_string(moveCount));
	for (int i=0; i<5; i++){
		for (int j=0; j<5; j++){
			int k=board[i][j].tileType;

			if (i == source.x && j == source.y)
				renderTextureRotated(renderer, tileTexPS[k].texture, 498+186*i, 78+186*j, board[i][j].orientation*90);
			else if (board[i][j].turnedOn)
				renderTextureRotated(renderer, tileTexOn[k].texture, 498+186*i, 78+186*j, board[i][j].orientation*90);
			else
				renderTextureRotated(renderer, tileTex[k].texture, 498+186*i, 78+186*j, board[i][j].orientation*90);
		}
	}
}

int Board::getMinMoves(){
    return minMoves;
}