#include <switch.h>

#include <SDL2/SDL.h>
#include <SDL2/SDL_events.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_mixer.h>
#include <SDL2/SDL_TTF.h>
#include "Board.h"
#include "GFX.h"

Board brd;

touchPosition Stylus;
u32 kDown;

typedef struct 
{
	SDL_Texture * texture;
} 
image;
image background, cursor;


SDL_Window *    window;
SDL_Renderer *  renderer;
SDL_Surface *   surface;

TTF_Font* font;

int lvl=0;

int curX=0, curY=0;

float divX=0,divY=0;

bool buttonControlled = false;

bool downTouchInBox(touchPosition touch, int x1, int y1, int x2, int y2)
{
	int tx=touch.px;
	int ty=touch.py;

	if (kDown & KEY_TOUCH && tx > x1 && tx < x2 && ty > y1 && ty < y2)
		return true;
	else
		return false;
}

void updateScreen(){
	SDL_RenderClear(renderer);
	renderTexture(renderer, background.texture, 0, 0);
	if (buttonControlled)
		renderTexture(renderer, cursor.texture, 332+124*curX, 52+124*curY);
	brd.draw(renderer);
	
	SixAxisSensorValues sasv;
	hidSixAxisSensorValuesRead(&sasv, CONTROLLER_P1_AUTO, 1);
	divX-=sasv.gyroscope.z;
	divY-=sasv.gyroscope.x;
	renderText(renderer, font, 10, 200, "X: " + to_string(sasv.gyroscope.x));
	renderText(renderer, font, 10, 240, "Y: " + to_string(sasv.gyroscope.y));
	renderText(renderer, font, 10, 280, "Z: " + to_string(sasv.gyroscope.z));
	
	renderText(renderer, font, 10, 340, "dX: " + to_string(divX));
	renderText(renderer, font, 10, 380, "dY: " + to_string(divY));
	renderTexture(renderer, cursor.texture, (150*divX+640)-62, (100*divY+360)-62);
	SDL_RenderPresent(renderer);
}

void manageInput()
{
	hidScanInput();
	kDown = hidKeysDown(CONTROLLER_P1_AUTO);
	hidTouchRead(&Stylus, 0);

	if (divX < -30)
		divX = 60+divX;
	if (divX > 30)
		divX = -60+divX;
	if (divY < -30)
		divY = 60+divY;
	if (divY > 30)
		divY = -60+divY;

	if (kDown & KEY_DOWN){
		curY++;
		buttonControlled = true;
	}
	if (kDown & KEY_UP){
		curY--;
		buttonControlled = true;
	}
	if (kDown & KEY_LEFT){
		curX--;
		buttonControlled = true;
	}
	if (kDown & KEY_RIGHT){
		curX++;
		buttonControlled = true;
	}
	if (kDown & KEY_X){
		divX=0;
		divY=0;
	}


	if (kDown & KEY_Y)
		brd.loadBoard("romfs:/levels/5x5.json", ++lvl);

	if (curX < 0)
		curX=4;
	if (curX > 4)
		curX=0;
	if (curY < 0)
		curY=4;
	if (curY > 4)
		curY=0;

	if (downTouchInBox(Stylus, 330, 50, 950, 670)){
		buttonControlled = false;
		int tx=(Stylus.px-330)/124;
		int ty=(Stylus.py-50)/124;
		brd.rotateTile(tx, ty);
		if (brd.getIsSolved()){
			brd.loadBoard("romfs:/levels/5x5.json", ++lvl);
		}
	}

	if (kDown & KEY_B){
		buttonControlled = false;
		int tx=((150*divX+640)-330)/124;
		int ty=((100*divY+360)-50)/124;
		if (tx >= 0 && tx <= 5 && ty >= 0 && ty <= 5)
			brd.rotateTile(tx, ty);
		if (brd.getIsSolved()){
			brd.loadBoard("romfs:/levels/5x5.json", ++lvl);
		}
	}

	if (kDown & KEY_A){
		buttonControlled = true;
		brd.rotateTile(curX, curY);
		if (brd.getIsSolved()){
			brd.loadBoard("romfs:/levels/5x5.json", ++lvl);
		}
	}
}


int main(int argc, char **argv)
{
	romfsInit();

	u32 handles[4];
	hidGetSixAxisSensorHandles(&handles[0], 2, CONTROLLER_PLAYER_1, TYPE_JOYCON_PAIR);
	hidGetSixAxisSensorHandles(&handles[2], 1, CONTROLLER_PLAYER_1, TYPE_PROCONTROLLER);
	hidGetSixAxisSensorHandles(&handles[3], 1, CONTROLLER_HANDHELD, TYPE_HANDHELD);
	hidStartSixAxisSensor(handles[0]);
	hidStartSixAxisSensor(handles[1]);
	hidStartSixAxisSensor(handles[2]);
	hidStartSixAxisSensor(handles[3]);
	
	SDL_Init(SDL_INIT_EVERYTHING);
	// Initialize
	IMG_Init(IMG_INIT_PNG);
	TTF_Init();
	window = SDL_CreateWindow("Main-Window", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 1920, 1080, SDL_WINDOW_SHOWN);
	renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);

	surface = IMG_Load("romfs:/images/background.png");
	background.texture = SDL_CreateTextureFromSurface(renderer, surface);
	SDL_FreeSurface(surface);

	surface = IMG_Load("romfs:/images/cursor.png");
	cursor.texture = SDL_CreateTextureFromSurface(renderer, surface);
	SDL_FreeSurface(surface);

	font = TTF_OpenFont("romfs:/fonts/OpenSans.ttf", 36);

	brd.init(renderer, font);
	brd.loadBoard("romfs:/levels/5x5.json", lvl);

	// Game loop:
	while (appletMainLoop())
	{
		manageInput();
		updateScreen();
		//printGame();

		if (kDown & KEY_PLUS)
			break;
	}

	//On detruit la fenètre
	SDL_DestroyWindow(window);
	IMG_Quit();	
    TTF_CloseFont(font);
	TTF_Quit();
	SDL_Quit();
	romfsExit();

	return EXIT_SUCCESS;
}
