#ifndef GRAPH_H
#define GRAPH_H
#include <functional>
#include <micropather.h>
#include "../Geometry.h"

template<typename NODE_TYPE>
struct Graph : micropather::Graph {
	
	Graph(std::function<float(NODE_TYPE const&, NODE_TYPE const&)> f) : f(f) {
		pather = new micropather::MicroPather(this);
		nodeCount = 0;	
	}

	virtual ~Graph() {
		delete pather;
	}
	
	virtual float LeastCostEstimate( void* stateStart, void* stateEnd ) {
		NODE_TYPE const& a = nodes[(int)stateStart];		
		NODE_TYPE const& b = nodes[(int)stateEnd];
		return f(a, b);
	}

	virtual void AdjacentCost( void* state, std::vector< micropather::StateCost > *adjacent ) {
		adjacent->insert(adjacent->end(), adjacencies[(int)state].begin(), adjacencies[(int)state].end()); 
	}

	virtual void  PrintStateInfo( void* state ) {};

	void addNode(NODE_TYPE& node) {	
		node.nodeNumber = nodeCount++;
		adjacencies.push_back({});
		nodes.push_back(node);
	}

	void addEdge(NODE_TYPE const& a, NODE_TYPE const& b, float const& cost) {
		#ifdef DEBUG
			assert(a.nodeNumber != -1 && b.nodeNumber != -1);
		#endif
		adjacencies[a.nodeNumber].push_back(micropather::StateCost((void*)(b.nodeNumber), cost));
		//adjacencies[b.nodeNumber].push_back(micropather::StateCost((void*)&a, cost));		
	}

	std::vector<NODE_TYPE> const& getNodes() const {
		return nodes;
	}

	std::vector<NODE_TYPE>& getNodes() {
		return nodes;
	}

	std::vector<NODE_TYPE> getAdjacent(NODE_TYPE const& v) {
		std::vector<NODE_TYPE> ans;
		for (micropather::StateCost const& sc : adjacencies[v.nodeNumber])
			ans.push_back(nodes[(int)(sc.state)]);
		return ans;
	}

	bool solve(NODE_TYPE const& a, NODE_TYPE const& b, std::vector<NODE_TYPE*>& out) {
		#ifdef DEBUG
			assert(a.nodeNumber != -1 && b.nodeNumber != -1);
		#endif
		float cost;
		return (pather->Solve((void*)(a.nodeNumber), (void*)(b.nodeNumber), (std::vector<void*>*)&out, &cost)) == micropather::MicroPather::SOLVED;
	}

private:
	int nodeCount;
	std::vector<std::vector<micropather::StateCost>> adjacencies;
	std::vector<NODE_TYPE> nodes;
	std::function<float(NODE_TYPE const&, NODE_TYPE const&)> f;

	micropather::MicroPather *pather;
};

#endif