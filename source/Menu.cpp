#include "Menu.h"
#include <json-c/json.h>
#include <fstream>
#include <streambuf>

using namespace std;

Menu::Menu(){

}

void Menu::init(TTF_Font* smallFnt, TTF_Font* bigFnt, string inFile){
	font = smallFnt;
	b_font = bigFnt;

	std::ifstream t(inFile);
	std::string jsonText((std::istreambuf_iterator<char>(t)), std::istreambuf_iterator<char>());

	json_object * jobj = json_tokener_parse(jsonText.c_str());

    levelCount = json_object_array_length(jobj);
	json_object_put(jobj);

	menuPos = 1;
}

void Menu::moveSelection(int dir){
	menuPos += dir;
	if (state == LVLS_STATE){
		if (menuPos < 1)
			menuPos = levelCount;
		if (menuPos > levelCount)
			menuPos = 1;
	}else if (state == MAIN_STATE){
		if (menuPos < 1)
			menuPos = 2;
		if (menuPos > 2)
			menuPos = 1;
	}
}

bool Menu::selectOption(){
	switch(state){
		case MAIN_STATE:
			return false;
		case LVLS_STATE:
			return true;
		default:
			return false;
	}
}

int Menu::getSelectedLevel(){
	return menuPos-1;
}

void Menu::draw(SDL_Renderer* renderer){
	switch(state){
		case MAIN_STATE:
			renderTextCentered(renderer, b_font, 960, 100, "Play Game");
			break;
		case LVLS_STATE:
			int start=1, end=2;
			if (menuPos > 5){
				start = menuPos - 5;
				end = menuPos + 5;
			}else{
				start = 1;
				end = 11;
			}
			if (end > 1000){
				start = 991;
				end = 1001;
			}

			int j=0;
			renderTextCentered(renderer, b_font, 480, 40, "Select Level:");
			for (int i=start; i<end; i++){
				if (i != menuPos)
					renderColorTextCentered(renderer, b_font, 1440, 40+j*100, "Level " + to_string(i), {200,200,200});
				else
					renderTextCentered(renderer, b_font, 1440, 40+j*100, "Level " + to_string(i));
				j++;
			}

			break;
	}

    //Draw menu options and currently selected level
	//renderText(renderer, font, 20, 20, "Current selected level: " + to_string(menuPos) + "\nLevel count: " + to_string(levelCount));
}