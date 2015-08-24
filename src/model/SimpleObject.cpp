#include <boost/algorithm/string.hpp>
#include "SimpleObject.h"

SimpleObject::SimpleObject(Tmx::Object const* o, Tmx::ObjectGroup const* textureLayer, hgeResourceManager* res) : IObject(o, res) {
	if (havePoly) {
    	if (o->GetProperties().HasProperty("IsObstacle"))
    		obstacle = o->GetProperties().GetIntProperty("IsObstacle");
    	else 
    		obstacle = true;

    	if (o->GetProperties().HasProperty("IsObstruct"))
    		obstruct = o->GetProperties().GetIntProperty("IsObstruct");
    	else 
    		obstruct = true;
    } else obstacle = obstruct = false;
    std::vector<std::string> strs;
	std::string str;
	boost::split(strs, str=(o->GetProperties().GetStringProperty("TexturesID")), boost::is_any_of(", "));
	for (std::string const& s : strs) {
		int id = std::stoi(s);
		for (Tmx::Object const* texture : textureLayer->GetObjects()) {
			if (texture->GetId() == id) {
				float rot = texture->GetRot();
				rot *= M_PI/180;
				Vector spos(texture->GetX(), texture->GetY());
				spos -= pos;
				int width = texture->GetWidth();
				int hight = texture->GetHeight();
				hgeSprite* spr = (res->GetSprite(texture->GetName().c_str()));
				if (spr == NULL) {
					std::cerr << "Can't find sprite: " << texture->GetName().c_str() << "\n";
					exit(0);
				}
				rData.push_back({*spr, spos, rot, width, hight});
				break;
			}
		}
	}
}
