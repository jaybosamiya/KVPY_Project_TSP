#include "constants.h"
#include "Graph.h"
#include "OptimalPath.h"

#include <algorithm>
#include <vector>
#include <queue>

#ifndef FILE_TSPSOLVE_H
#define FILE_TSPSOLVE_H

namespace TSPSolve {
	using namespace std;

	Graph graph;
	vector<int> bestPath;
	int bestLength = INFINITY;

	int closest[MAXV];

	void findClosest() {
		int distance;
		for ( int i = 0 ; i < graph.V ; i++ ) {
			distance = INFINITY;
			for ( int j = 0 ; j < graph.V ; j++ ) {
				distance =  min(distance,graph.weight[i][j]);
			}
			closest[i] = distance;
		}
	}

	struct Node {
		vector<int> path;
		bool visited[MAXV]; // visited := outward path is fixed from that vertex
		int tourLength;

		friend bool operator < (const Node &n1, const Node &n2);	// to handle ordering in priority_queue

		Node() {
			visited[0] = true;	// is this necessary? Write reason
			for ( int i = 1 ; i < MAXV ; i++ ) {
				visited[i] = false;
			}
			path.push_back(0); // start path from city 0 (without loss of generality)
		}

		void computeTourLength() {
			tourLength = 0;
			for ( int i = 1 ; i < path.size() ; i++ ) {
				tourLength += graph.weight[path[i-1]][path[i]];
			}

			for ( int i = 0 ; i < graph.V ; i++ ) {
				if ( !visited[i] ) {
					tourLength += closest[i];
				}
			}
		}
	};

	bool operator < (const Node &n1, const Node &n2) { 
		// reversed comparison to make priority_heap into a min heap
		if ( n1.tourLength < n2.tourLength ) return false;
		if ( n1.tourLength == n2.tourLength && n1.path.size() > n2.path.size() ) return false;
		return true;
	}

	void tspSolve(Graph graph, OptimalPath &optimalPath) {
		TSPSolve::graph = graph;
		findClosest();
		priority_queue<Node> validStates;
		Node node;
		node.computeTourLength();
		validStates.push(node);

		bestLength = INFINITY;

		while(!validStates.empty()) {
			node = validStates.top();
			validStates.pop();

			// Prune if unnecessary
			if ( node.tourLength >= bestLength ) {
				continue;
			}

			// Separately need to complete the tour (prevent premature completion at any other stage)
			if ( node.path.size() == graph.V ) {
				Node completeTour = node;
				completeTour.path.push_back(0);
				completeTour.computeTourLength();
				if ( completeTour.tourLength >= bestLength ) {
					continue;
				}
				bestLength = completeTour.tourLength;
				bestPath = completeTour.path;
				continue;
			}

			// Generate new states
			for ( int i = 0 ; i < graph.V ; i++ ) {
				if ( !node.visited[i] ) {
					Node branchState = node;
					branchState.path.push_back(i);
					branchState.visited[i] = true;
					branchState.computeTourLength();
					if ( branchState.tourLength >= bestLength ) {
						continue;
					}
					validStates.push(branchState);
				}
			}
		}

		for ( int i = 0 ; i < bestPath.size() ; i++ ) {
			optimalPath.location[i] = bestPath[i];
		}
		optimalPath.V = graph.V;
		optimalPath.length = bestLength;
	}
};

void tspSolve(Graph graph, OptimalPath &optimalPath) {
	TSPSolve::tspSolve(graph,optimalPath);
}

#endif
