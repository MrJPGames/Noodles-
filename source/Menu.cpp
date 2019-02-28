#include "Menu.h"
#include <json-c/json.h>
#include <fstream>
#include <streambuf>

using namespace std;

Menu::Menu(){
	std::ifstream t("test.json");
	std::string jsonText((std::istreambuf_iterator<char>(t)), std::istreambuf_iterator<char>());

	json_object * jobj = json_tokener_parse(jsonText.c_str());

    levelCount = json_object_array_length(jobj);
    
	//Free memory after use
	json_object_put(jobj);
}

void Menu::draw(){
    //Draw menu options and currently selected level
}