#include "LevelLoader.h"
#include "../model/GameState.h"
#include <tinyxml.h>
#include <iostream>

GameState* LevelLoader::load(std::string file) {
	TiXmlDocument doc(file);
	bool loadOkay = doc.LoadFile();
	if (!loadOkay) {
		std::cerr << "Can't open " << file << "\n";
		return NULL;
	}
	TiXmlNode *v = &doc;

}
