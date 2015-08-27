#include "../Environment.h"
#include "../SimpleObject.h"

inline int norm(int a) {
	int i = 0;
	while ((1 << i) < a)
		++i;
	return (1 << i);
}

void Environment::initGraph() {
	/*for (Vertex& v : helpVertices)
		graph->addNode(v);*/
	for (Vertex& v : obstaclePolygon.ering)
		graph->addNode(v);
	for (geo::Ring<Vertex>& ring : obstaclePolygon.irings)
		for (Vertex& v : ring)
			graph->addNode(v);

	for (int i = 0; i < obstaclePolygon.ering.size() - 1; ++i) {
		Vertex const& c = (i == 0) ? obstaclePolygon.ering[obstaclePolygon.ering.size() - 2] : obstaclePolygon.ering[i-1];
		Vertex const& o = obstaclePolygon.ering[i];
		Vertex const& a = obstaclePolygon.ering[i+1];
		std::vector<Vertex> out;
		geo::visibleVertices(a, o, c, obstaclePolygon, out);
		for (Vertex const& v : out)
			graph->addEdge(o, v, sqrt(1.f*geo::distance2(o, v)));
	}
	
	/*for (geo::Ring<Vertex> const& ring : obstaclePolygon.irings) {
		for (int i = 0; i < ring.size() - 1; ++i) {
			Vertex const& a = (i == 0) ? ring[ring.size() - 2] : ring[i-1];
			Vertex const& o = ring[i];
			Vertex const& c = ring[i+1];
			std::vector<Vertex> out;
			geo::visibleVertices(a, o, c, obstaclePolygon, out);
			for (Vertex const& v : out)
				graph->addEdge(o, v, sqrt(1.f*geo::distance2(o, v)));
	
		}
	}*/
	
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
   	
   	obstructPolygon.ering = {{0, 0}, {width, 0}, {width, hight}, {0, hight}, {0, 0}};
   	geo::correct(obstructPolygon.ering);
   	

   	this->inflateRadius = map->GetProperties().GetIntProperty("InflateRadius");
   	
   	obstaclePolygon.ering = {{0, 0}, {width, 0}, {width, hight}, {0, hight}, {0, 0}};
   	geo::correct(obstaclePolygon.ering);

   	obstaclePolygon.ering = inflate(obstaclePolygon.ering, -inflateRadius);
   	
   	graph = new Graph<Vertex>([](Vertex const& a, Vertex const& b) {return sqrt(1.f*geo::distance2(a, b));});
   	
   	Tmx::ObjectGroup const* textureLayer = NULL;
   	for (Tmx::ObjectGroup const* og : map->GetObjectGroups()) {
   		if (og->GetName() == "TextureLayer") {
   			textureLayer = og;
   			break;
   		}
   	}
	
	for (Tmx::ObjectGroup* ob : map->GetObjectGroups()) {
   		if (ob->GetName() != "ObjectLayer")
   			continue;
   		for (Tmx::Object* o : ob->GetObjects()) {
   			if (!o->IsVisible())
   				continue;
   			if (o->GetType() == "SimpleObject") {
   				addObject(new SimpleObject(o, textureLayer, res));
   			} else if (o->GetType() == "LightSource") {
   				addLightSource(new LightSource(o, res));   				
   			}
   		}
   	}

   	for (Tmx::ObjectGroup* ob : map->GetObjectGroups()) {
   		if (ob->GetName() != "GraphLayer" || !ob->IsVisible())
   			continue;
   		for (Tmx::Object* o : ob->GetObjects()) {
   			if (!o->IsVisible())
   				continue;
   			if (o->GetType() == "Vertex") {
   				helpVertices.push_back(Vertex(o->GetX() + o->GetWidth()/2, o->GetY() + o->GetHeight()/2));
   			} 
   		}
   	}
   	initGraph();
}

void Environment::findVisibility(Vertex const& o, geo::Ring<Vertex>& out) const {
	out.clear();
	geo::visibilityPolygon(o, obstructPolygon, out);
}

bool Environment::findPath(Vertex const& a, Vertex const& b, std::vector<Vertex>& out) const {
	out.clear();
	std::vector<Vertex*> out1;
	bool ans = graph->solve(a, b, out1);
	for (Vertex* v : out1)
		out.push_back(*v);
	return ans;
}
	

void Environment::addObject(IObject* obj) {
	objects.push_back(obj);
	if (obj->isObstruct())
		obstructPolygon.irings.push_back(obj->getBounds());
	if (obj->isObstacle()) {
		geo::Ring<Vertex> ring = obj->getBounds();
		obstaclePolygon = subtract(obstaclePolygon, inflate(ring, inflateRadius));
	}
}

void Environment::addLightSource(LightSource* ls) {
	lightSources.push_back(ls);
	ls->setEnvironment(this);
}

void Environment::frame() {
	for (LightSource* ls : lightSources)
		ls->frame();
}

