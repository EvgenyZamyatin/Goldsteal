#ifndef RESOURCES_H
#define RESOURCES_H
#include "ITileSet.h"
struct Resources {
	std::vector<ITileSet*> tileSets;	
	void find(int gid, int& idx, ITileSet* &ts) const {
		int nd;
		for (nd = 0; gid >= tileSets[nd]->size(); nd++) {
			gid -= tileSets[nd]->size();
		}
		idx=gid;
		ts=tileSets[nd];
	}
};

#endif