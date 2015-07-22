#include "LevelLoader.h"
#include "../model/GameState.h"
#include "../model/Environment.h"
#include "../geometry/Geometry.h"
#include "ITileSet.h"
#include <iostream>
#include <hgeresource.h>
#include <stdlib.h>
#include <hge.h>
#include <tmx/Map.h>         
#include <cassert>

/*
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

Environment* LevelLoader::parseEnvironment(TiXmlNode* v, const Resources& res) {
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
	ITileSet* st;
	for (int& i : indxs)
	res.find(i, i, st);
	Environment* env = new Environment(width, hight, 
									st->tileWidth, st->tileHight, indxs, st);
	return env;
}

IObject* LevelLoader::parseObject(TiXmlNode* v, const Resources& res) {
	int gid;
	double x, y, rotation=0;
	double width, hight;
	bool sstatic;
	std::string type;
   	for (TiXmlAttribute* attr = v->ToElement()->FirstAttribute(); attr != 0; attr=attr->Next()) {
   		if (std::string(attr->Name()) == "gid") {
   			gid = atoi(attr->Value())-1;
   		} else if (std::string(attr->Name()) == "x") {
   			x = attr->DoubleValue();
   		} else if (std::string(attr->Name()) == "y") {
   			y = attr->DoubleValue();
   		} else if (std::string(attr->Name()) == "rotation") {
   			rotation = attr->DoubleValue();
   		} else if (std::string(attr->Name()) == "width") {
   			width = attr->DoubleValue();
   		} else if (std::string(attr->Name()) == "height") {
   			hight = attr->DoubleValue();
   		} else if (std::string(attr->Name()) == "type") {
   			type = attr->Value();
   		}
   	}
	ITileSet* ts;
	res.find(gid, gid, ts);
	Geo::Polygon poly({{x,y},{x+width,y},{x+width,y-hight},{x, y-hight}});
	poly.rotate(rotation*M_PI/180.0);
	return new Stuff(poly, ts, gid, type);
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
	v = v->FirstChild()->NextSibling()->FirstChild();
	while (v->Type() == TiXmlNode::TINYXML_ELEMENT && 
			std::string(v->Value()) == "tileset") {
		res.tileSets.push_back(parseTileSet(v));
		v=v->NextSibling();     
	}
	GameState* state = new GameState();
	state->env=parseEnvironment(v, res);	                           
	v=v->NextSibling();                                
	assert(std::string(v->Value()) == "objectgroup");
	TiXmlNode *node = v->FirstChild();
	while (node) {
		state->env->addObject(parseObject(node, res));
		node=node->NextSibling();
	}
	return state;
}
*/

void LevelLoader::MyVisitor::visitTileLayer(const tmx::Map& map, const tmx::TileLayer& layer) {
	assert(layer.hasProperty("tileSet"));
	std::string tileSetName = layer.getProperty("tileSet", "");
	HTEXTURE tileSetTex = res->GetTexture(tileSetName.c_str());
	const tmx::TileSet *ts = NULL;
	for (auto tsit : map.getTileSets()) {
		if (tsit->getName() == tileSetName) {
			ts = tsit;
			break;
		}
	}
	assert(ts != NULL);
	std::vector<std::pair<int, int>> tasks;
	for (auto cell = layer.begin(); cell != layer.end(); ++cell) {
		int id = (int)cell->getGID() - (int)ts->getFirstGID();
		if (id < 0) {
			tasks.push_back(std::make_pair(-1, -1));
			continue;
		}
		tmx::Rect r = ts->getCoords(id, {ts->getImage()->getWidth(), ts->getImage()->getHeight()});
		tasks.push_back(std::make_pair(r.x, r.y));
	}
	Renderer::EnvironmentRenderData::Layer l;
	l.tex=tileSetTex;                         
	l.texWidth = hge->Texture_GetWidth(l.tex);
	l.texHight = hge->Texture_GetHeight(l.tex);
	l.alpha = layer.getOpacity();
	l.tcorners = tasks;
	envData.layers.push_back(l);
}
void LevelLoader::MyVisitor::visitObjectLayer(const tmx::Map& map, const tmx::ObjectLayer& layer) {
    std::cerr << "ObjectLayer" << "\n";
}
void LevelLoader::MyVisitor::visitImageLayer(const tmx::Map& map, const tmx::ImageLayer& layer) {
	std::cerr << "ImageLayer" << "\n";
}

bool LevelLoader::load(HGE* hge, const char* file, const char* resources, GameState* &state, hgeResourceManager* &res, Renderer* &renderer) {
	res = new hgeResourceManager(resources);
	std::unique_ptr<tmx::Map> map = tmx::Map::parseFile(file);
	if (map == NULL)
		return false;
	
	state = new GameState(new Environment());
	MyVisitor vis(hge, res, state);
	
	vis.envData.tilesInRow=map->getWidth();
	vis.envData.tilesInColumn=map->getHeight();   
	vis.envData.texTileWidth=map->getTileWidth();
	vis.envData.texTileHight=map->getTileHeight();        
	
	vis.state->env->width=map->getTileWidth() * map->getWidth();
	vis.state->env->hight=map->getTileHeight() * map->getHeight();
	       
	vis.envData.envTileWidth=vis.state->env->width/map->getWidth();
	vis.envData.envTileHight=vis.state->env->hight/map->getHeight();
	
	map->visitLayers(vis);

	renderer = new Renderer();
	renderer->envData=vis.envData;
	return true;
}




