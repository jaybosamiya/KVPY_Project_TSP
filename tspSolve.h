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

	vector<int> bestPath;
	int bestLength = INFINITY;

	struct Node {
		Graph state;
		vector<int> path;
		bool visited[MAXV]; // visited means inward path is not acceptable
		int tourLength;

		bool operator < (Node n) { // reversed comparison to make priority_heap min_heap
			if ( tourLength < n.tourLength ) return false;
			if ( tourLength == n.tourLength && path.length() > n.path.length() ) return false;
			return true;
		}

		Node() {
			visited[0] = true;
			for ( int i = 1 ; i < MAXV ; i++ ) {
				visited[i] = false;
			}
			path.push_back(0); // start path from city 0 (without loss of generality)
		}

		void computeTourLength() {
			tourLength = 0;
			for ( int i = 1 ; i < path.length() ; i++ ) {
				tourLength += state.weight[path[i-1]][path[i]];
			}
			for ( int i = 0 ; i < state.V ; i++ ) {
				if ( !visited[i] ) {
					int shortest = state.weight[i][0];
					for ( int j = 0 ; j < state.V ; j++ ) {
						shortest = min(shortest,state.weight[i][j]);
					}
					tourLength += shortest;
				}
			}
		}
	};

	void tspSolve(Graph graph, OptimalPath &optimalPath) {
		priority_queue<Node> validStates;
		Node node;
		node.state = graph;
		node.computeTourLength();
		validStates.push(node);

		bestLength = INFINITY;

		while(!valid.empty()) {
			node = validStates.top();
			validStates.pop();

			// Prune if unnecessary
			if ( node.tourLength >= bestLength ) {
				continue;
			}

			// Found a new good path                                   // NOTE: Is this necessary?
			if ( node.path.length() == node.state.V + 1 ) {
				bestLength = node.tourLength;
				bestPath = node.path;
				continue;
			}

			// Separately need to complete the tour (because, otherwise, premature completion may occur
			if ( node.path.length() == node.state.V ) {
				Node completeTour = node;
				completeTour.path.push_back(0);
				completeTour.computeTourLength();
				if ( completeTour.tourLength >= bestLength ) {
					continue;
				}
				bestLength = completeTour.tourLength;
				bestPath = complteTour.path;
				continue;
			}

			// Generate new states
			for ( int i = 0 ; i < node.state.V ; i++ ) {
				if ( !node.visited[i] ) {
// ?!?!?!?
				}
			}
		}
	}
};

void tspSolve(Graph graph, OptimalPath &optimalPath) {
	TSPSolve::tspSolve(graph,optimalPath);
}

#endif
