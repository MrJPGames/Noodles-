#include <switch.h>

#include <SDL2/SDL.h>
#include <SDL2/SDL_events.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_mixer.h>
#include <SDL2/SDL_TTF.h>
#include "Board.h"
#include "Menu.h"
#include "GFX.h"

#define TITL_STATE 0
#define MENU_STATE 1
#define GAME_STATE 2
#define SOLV_STATE 3

Board brd;
Menu menu;

//Sets the current mode (state) of the game
int mode = TITL_STATE;

touchPosition Stylus;
u32 kDown;

typedef struct 
{
	SDL_Texture * texture;
} 
image;
image background, cursor, fade, title;


SDL_Window *    window;
SDL_Renderer *  renderer;
SDL_Surface *   surface;

TTF_Font* font;
TTF_Font* bigFont;

int curX=0, curY=0;

bool buttonControlled = true;

bool downTouchInBox(touchPosition touch, int x1, int y1, int x2, int y2)
{
	int tx=touch.px;
	int ty=touch.py;

	if (kDown & KEY_TOUCH && tx > x1 && tx < x2 && ty > y1 && ty < y2)
		return true;
	else
		return false;
}

void renderGame(){
	if (buttonControlled)
		renderTexture(renderer, cursor.texture, 498+186*curX, 78+186*curY);
	brd.draw(renderer);
}

void renderMenu(){
	menu.draw(renderer);
}

void renderTitle(){
	renderTexture(renderer, title.texture, 0, 0);
	renderTextCentered(renderer, font, 960, 900, "Press A to start!");
}

void renderSolve(){
	brd.draw(renderer);
	renderFadeOverlay(renderer, 0, 0, 0, 127);
	renderColorTextCentered(renderer, bigFont, 960, 80, "Stage Cleared!", {255,255,255});
	renderColorTextCentered(renderer, font, 960, 920, "Press A to continue", {255,255,255});
	renderColorTextCentered(renderer, font, 960, 1000, "Press B to exit", {255,255,255});
}


void updateScreen(){
	SDL_RenderClear(renderer);
	renderTexture(renderer, background.texture, 0, 0);
	switch(mode){
		case GAME_STATE:
			renderGame();
			break;
		case MENU_STATE:
			renderMenu();
			break;
		case SOLV_STATE:
			renderSolve();
			break;
		case TITL_STATE:
			renderTitle();
			break;
		default:
			renderText(renderer, font, 0, 0, "Invalid mode encountered! Mode id: " + to_string(mode));
			break;
	}
	SDL_RenderPresent(renderer);
}

void manageInput(){
	hidScanInput();
	kDown = hidKeysDown(CONTROLLER_P1_AUTO);
	hidTouchRead(&Stylus, 0);

	switch(mode){
		case GAME_STATE:
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
					mode = SOLV_STATE;
				}
			}
			if (kDown & KEY_A){
				buttonControlled = true;
				brd.rotateTile(curX, curY);
				if (brd.getIsSolved()){
					mode = SOLV_STATE;
					for (int i=0; i<128; i+=4){
						SDL_RenderClear(renderer);
						renderTexture(renderer, background.texture, 0, 0);
						renderGame();
						renderFadeOverlay(renderer, 0, 0, 0, i);
						SDL_RenderPresent(renderer);
					}
				}
			}
			break;
		case MENU_STATE:
			if (kDown & KEY_DOWN){
				menu.moveSelection(1);
			}
			if (kDown & KEY_UP){
				menu.moveSelection(-1);
			}
			if (kDown & KEY_A){
				if (menu.selectOption()){
					int lvl = menu.getSelectedLevel();
					brd.loadBoard("romfs:/levels/5x5.json", lvl);
					mode = GAME_STATE;

					for (int i=0; i<255; i+=17){
						SDL_RenderClear(renderer);
						renderTexture(renderer, background.texture, 0, 0);
						renderMenu();
						renderFadeOverlay(renderer, 0, 0, 0, i);
						SDL_RenderPresent(renderer);
					}
					brd.loadNextBoard("romfs:/levels/5x5.json");
					for (int i=255; i>0; i-=17){
						SDL_RenderClear(renderer);
						renderTexture(renderer, background.texture, 0, 0);
						renderGame();
						renderFadeOverlay(renderer, 0, 0, 0, i);
						SDL_RenderPresent(renderer);
					}
				}
			}
			break;
		case SOLV_STATE:
			if (kDown & KEY_A){
				mode = GAME_STATE;
				for (int i=0; i<255; i+=17){
					SDL_RenderClear(renderer);
					renderTexture(renderer, background.texture, 0, 0);
					renderSolve();
					renderFadeOverlay(renderer, 0, 0, 0, i);
					SDL_RenderPresent(renderer);
				}
				brd.loadNextBoard("romfs:/levels/5x5.json");
				for (int i=255; i>0; i-=17){
					SDL_RenderClear(renderer);
					renderTexture(renderer, background.texture, 0, 0);
					renderGame();
					renderFadeOverlay(renderer, 0, 0, 0, i);
					SDL_RenderPresent(renderer);
				}
			}
			if (kDown & KEY_B){
				mode = MENU_STATE;
				for (int i=0; i<255; i+=17){
					SDL_RenderClear(renderer);
					renderTexture(renderer, background.texture, 0, 0);
					renderSolve();
					renderFadeOverlay(renderer, 0, 0, 0, i);
					SDL_RenderPresent(renderer);
				}
				brd.loadNextBoard("romfs:/levels/5x5.json");
				for (int i=255; i>0; i-=17){
					SDL_RenderClear(renderer);
					renderTexture(renderer, background.texture, 0, 0);
					renderMenu();
					renderFadeOverlay(renderer, 0, 0, 0, i);
					SDL_RenderPresent(renderer);
				}
			}
			break;
		case TITL_STATE:
			if (kDown & KEY_A){
				mode = MENU_STATE;
				for (int i=0; i<255; i+=17){
					SDL_RenderClear(renderer);
					renderTitle();
					renderFadeOverlay(renderer, 0, 0, 0, i);
					SDL_RenderPresent(renderer);
				}
				for (int i=255; i>0; i-=17){
					SDL_RenderClear(renderer);
					renderTexture(renderer, background.texture, 0, 0);
					renderMenu();
					renderFadeOverlay(renderer, 0, 0, 0, i);
					SDL_RenderPresent(renderer);
				}
			}
			break;
	}
}


int main(int argc, char **argv)
{
	romfsInit();
	
	SDL_Init(SDL_INIT_EVERYTHING);
	// Initialize
	IMG_Init(IMG_INIT_PNG);
	TTF_Init();
	window = SDL_CreateWindow("Main-Window", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 1920, 1080, SDL_WINDOW_SHOWN);
	renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);

	//Set blend mode
	SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);

	surface = IMG_Load("romfs:/images/background.png");
	background.texture = SDL_CreateTextureFromSurface(renderer, surface);
	SDL_FreeSurface(surface);

	surface = IMG_Load("romfs:/images/cursor.png");
	cursor.texture = SDL_CreateTextureFromSurface(renderer, surface);
	SDL_FreeSurface(surface);

	surface = IMG_Load("romfs:/images/fade.png");
	fade.texture = SDL_CreateTextureFromSurface(renderer, surface);
	SDL_FreeSurface(surface);

	surface = IMG_Load("romfs:/images/titlescreen.png");
	title.texture = SDL_CreateTextureFromSurface(renderer, surface);
	SDL_FreeSurface(surface);

	font = TTF_OpenFont("romfs:/fonts/OpenSans.ttf", 48);
	bigFont = TTF_OpenFont("romfs:/fonts/OpenSans.ttf", 72);

	brd.init(renderer, font, "romfs:/levels/5x5.json");
	brd.loadBoard("romfs:/levels/5x5.json", 0);

	menu.init(font, bigFont, "romfs:/levels/5x5.json");

	// Game loop:
	while (true)
	{
		manageInput();
		updateScreen();

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
