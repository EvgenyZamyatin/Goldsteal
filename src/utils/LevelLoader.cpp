#include "LevelLoader.h"
#include "../model/GameState.h"
#include "../model/Environment.h"
#include <tinyxml.h>
#include <iostream>
#include <stdlib.h>
#include <hge.h>

ITileSet* LevelLoader::parseTileSet(TiXmlNode* v) {
    TiXmlElement* e = v->ToElement();
	TiXmlAttribute* attr = e->FirstAttribute();
	int childCount = 0;
	TiXmlNode *child = v->FirstChild();
	while (child)child=child->NextSibling(), childCount++;
	int margin=0;
	int spacing=0;
	int tw,th;
	int w,h;
	HTEXTURE tex;
	while (attr) {
		if (std::string(attr->Name()) == "tilewidth") {
			tw=atoi(attr->Value());
		} else if (std::string(attr->Name()) == "tileheight") {
			th=atoi(attr->Value());
		 } else if (std::string(attr->Name()) == "spacing") {
		 	spacing=atoi(attr->Value());
		} else if (std::string(attr->Name()) == "margin") {
			margin=atoi(attr->Value());		
		}
		attr=attr->Next();		 	
	}	
	
	if (childCount == 1) {
		child = v->FirstChild();
		e = child->ToElement();
		attr = e->FirstAttribute();
		while (attr) {
			if (std::string(attr->Name()) == "width") {
				w=atoi(attr->Value());
			} else if (std::string(attr->Name()) == "height") {
				h=atoi(attr->Value());
			} else if (std::string(attr->Name()) == "source") {
				tex=hge->Texture_Load(attr->Value());
				if (tex == 0) {
					std::cerr << "Can't load " << attr->Value() << "\n";
					exit(1);	
				}
			}
		 	attr=attr->Next();		 			
		}
		return new TileSetFromImage(tex, w, h, tw, th, margin, spacing);
	}
	
	w=tw;
	h=th;
	std::vector<HTEXTURE> texs;
	for (child=v->FirstChild(); child != 0; child=child->NextSibling()) {
		for (attr=child->FirstChild()->ToElement()->FirstAttribute(); attr != 0; attr = attr->Next()) {
			if (std::string(attr->Name()) == "source") {
				tex=hge->Texture_Load(attr->Value());
				if (tex == 0) {
					std::cerr << "Can't load " << attr->Value() << "\n";
					exit(1);
				}
				texs.push_back(tex);
			}
		}			
	}
	return new TileSet(texs, tw, th);
}

Environment* LevelLoader::parseEnvironment(TiXmlNode* v, const Resources& tsets) {
	assert(std::string(v->Value()) == "layer");
	int width, hight;
	for (TiXmlAttribute* attr = v->ToElement()->FirstAttribute(); attr != 0; attr=attr->Next()) {
		if (std::string(attr->Name()) == "width") {
			width = atoi(attr->Value());
		} else if (std::string(attr->Name()) == "height") {
			hight = atoi(attr->Value());
		}
	}
	v=v->FirstChild();
	assert(std::string(v->Value()) == "data");
	v=v->FirstChild();
	std::vector<int> indxs;
	for (TiXmlNode *node = v; node != 0; node = node->NextSibling()) {
		indxs.push_back(atoi(node->ToElement()->FirstAttribute()->Value())-1);
	}
	int tmp=indxs[0];
	int nd;
	int sm=0;

	for (nd = 0; tmp >= tsets.tileSets[nd]->size(); nd++) {
		sm += tsets.tileSets[nd]->size();
		tmp -= tsets.tileSets[nd]->size();
	}

	for (int& i : indxs)
		i -= sm;
	Environment* env = new Environment(width, hight, 
									tsets.tileSets[nd]->tileWidth, tsets.tileSets[nd]->tileHight, indxs, tsets.tileSets[nd]);
	return env;
}

GameState* LevelLoader::load(std::string file, Resources &res) {
	TiXmlDocument doc(file);
	bool loadOkay = doc.LoadFile();
	if (!loadOkay) {
		std::cerr << "Can't open " << file << "\n";
		return NULL;
	}
	TiXmlNode *v = &doc;
	TiXmlNode* pChild;
	TiXmlText* pText;
	//assert(v->Type() == TiXmlNode::TINYXML_DOCUMENT);	
	v = v->FirstChild()->NextSibling()->FirstChild();
	while (v->Type() == TiXmlNode::TINYXML_ELEMENT && 
			std::string(v->Value()) == "tileset") {
		res.tileSets.push_back(parseTileSet(v));
		v=v->NextSibling();     
	}
	GameState* state = new GameState();
	state->env=parseEnvironment(v, res);	                           
	return state;
}
