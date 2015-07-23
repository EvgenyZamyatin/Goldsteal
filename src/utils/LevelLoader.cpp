#include "LevelLoader.h"         
#include "../model/Environment.h"
#include "../model/SimpleObstacle.h"
#include "../geometry/Geometry.h"

SimpleObstacle* LevelLoader::constructSimpleObstacle(Tmx::Object* o, hgeResourceManager* res) {
	SimpleObstacle* ans = new SimpleObstacle();
	double x = o->GetX();
	double y = o->GetY();
	double width = o->GetWidth();
	double hight = o->GetHeight();
	Geo::Polygon bounds({{x, y-hight}, {x+width, y-hight}, {x+width, y}, {x, y}});
	double angle = o->GetRot()*M_PI/180;
	bounds.rotate(3, angle);
	ans->setBounds(bounds);
	ans->setPos(bounds.center());
	ans->setDir(Geo::Vector(0,0));
	ans->rData.spr = *(res->GetSprite(o->GetName().c_str()));//o->GetProperties().GetStringProperty("Sprite").c_str()));
	if (o->GetProperties().HasProperty("SpritePoints")) 
		sscanf(o->GetProperties().GetStringProperty("SpritePoints").c_str(), "%d,%d,%d,%d", &ans->rData.point[0], &ans->rData.point[1], &ans->rData.point[2], &ans->rData.point[3]); 
	else { 
		for (int i = 0; i < 4; ++i)ans->rData.point[i] = i;	
	}
	return ans;
}

bool LevelLoader::load(HGE* hge, const char* file, const char* resources, GameState* &state, hgeResourceManager* &res) {
	res = new hgeResourceManager(resources);
	Tmx::Map map;
	map.ParseFile(file);
		
	state = new GameState(new Environment());
	
	state->env->rData.tilesInRow=map.GetWidth();
	state->env->rData.tilesInColumn=map.GetHeight();   
	state->env->rData.texTileWidth=map.GetTileWidth();
	state->env->rData.texTileHight=map.GetTileHeight();        
	
	state->env->width=map.GetTileWidth() * map.GetWidth();
	state->env->hight=map.GetTileHeight() * map.GetHeight();
	       
	state->env->rData.envTileWidth=state->env->width/map.GetWidth();
	state->env->rData.envTileHight=state->env->hight/map.GetHeight();
	
	
	for (Tmx::TileLayer* tl : map.GetTileLayers()) {
		Render::EnvironmentData::Layer l;
		l.tex = res->GetTexture(tl->GetProperties().GetStringProperty("TileSet").c_str());
		l.texWidth = hge->Texture_GetWidth(l.tex);
		l.texHight = hge->Texture_GetHeight(l.tex);
		l.alpha = (int)ceil(tl->GetOpacity() * 255);
		Tmx::Tileset const* ts = NULL;
		for (Tmx::Tileset* tmp : map.GetTilesets()) {
			if (tmp->GetName() == tl->GetProperties().GetStringProperty("TileSet")) {
				ts = tmp;
				break;
			}
		}
		int mar = ts->GetMargin();
		int spa = ts->GetSpacing();
		int tw = ts->GetTileWidth();
		int th = ts->GetTileHeight();
		int inRow = (ts->GetImage()->GetWidth() - 2*mar + spa) / (tw + spa);
		int inCol = (ts->GetImage()->GetHeight() - 2*mar + spa) / (th + spa);
		for (int i = 0; i < map.GetWidth(); ++i) {
			for (int j = 0; j < map.GetHeight(); ++j) {
				Tmx::MapTile const& t = tl->GetTile(j,i);
				int nmb = t.gid-ts->GetFirstGid();
				if (nmb < 0) {
					l.tcorners.push_back(std::make_pair(-1,-1));
					continue;
				}
				int ti = nmb % inRow;
				int tj = nmb / inRow;
				l.tcorners.push_back(std::make_pair(mar+ti*(tw+spa), mar+tj*(th+spa)));
			}
		}
		state->env->rData.layers.push_back(l);
   	}

   	for (Tmx::ObjectGroup* ob : map.GetObjectGroups()) {
   		for (Tmx::Object* o : ob->GetObjects()) {
   			if (o->GetType() == "SimpleObstacle") {
   				state->env->addObject(constructSimpleObstacle(o, res));
   			}
   		}
   	}

   	return true;
}






