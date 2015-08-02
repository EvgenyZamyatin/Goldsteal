#include "Environment.h"
#include "SimpleObject.h"

inline int norm(int a) {
	int i = 0;
	while ((1 << i) < a)
		++i;
	return (1 << i);
}

Environment::Environment(Tmx::Map const* map, hgeResourceManager* res) {
	rData.tilesInRow=map->GetWidth();
	rData.tilesInColumn=map->GetHeight();   
	rData.texTileWidth=map->GetTileWidth();
	rData.texTileHight=map->GetTileHeight();        
	
	width=map->GetTileWidth() * map->GetWidth();
	hight=map->GetTileHeight() * map->GetHeight();
	       
	rData.envTileWidth=width/map->GetWidth();
	rData.envTileHight=hight/map->GetHeight();
			
	for (Tmx::TileLayer* tl : map->GetTileLayers()) {
		Render::EnvironmentData::Layer l;
		Tmx::Tileset const* ts = NULL;
		for (Tmx::Tileset* tmp : map->GetTilesets()) {
			if (tmp->GetName() == tl->GetProperties().GetStringProperty("TileSet")) {
				ts = tmp;
				break;
			}
		}
		l.tex = res->GetTexture(tl->GetProperties().GetStringProperty("TileSet").c_str());
		l.texWidth = norm(ts->GetImage()->GetWidth());
		l.texHight = norm(ts->GetImage()->GetHeight());
		l.alpha = (int)ceil(tl->GetOpacity() * 255);
		
		int mar = ts->GetMargin();
		int spa = ts->GetSpacing();
		int tw = ts->GetTileWidth();
		int th = ts->GetTileHeight();
		int inRow = (ts->GetImage()->GetWidth() - 2*mar + spa) / (tw + spa);
		int inCol = (ts->GetImage()->GetHeight() - 2*mar + spa) / (th + spa);
		for (int i = 0; i < map->GetWidth(); ++i) {
			for (int j = 0; j < map->GetHeight(); ++j) {
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
		rData.layers.push_back(l);
   	}
   	
   	for (Tmx::ObjectGroup* ob : map->GetObjectGroups()) {
   		for (Tmx::Object* o : ob->GetObjects()) {
   			if (o->GetType() == "SimpleObject") {
   				addObject(new SimpleObject(o, res));
   			} 
   		}
   	}
}