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

	int closestVertex[MAXV][MAXV];

	void generateClosestVertex() {
		pair<int,int> distances[MAXV];
		for ( int i = 0 ; i < graph.V ; i++ ) {
			for ( int j = 0 ; j < graph.V ; j++ ) {
				distances[j] = make_pair(graph.weight[i][j],j);
			}
			sort(distances,distances+graph.V);
			for ( int j = 0 ; j < graph.V ; j++ ) {
				closestVertex[i][j] = distances[j].second;
			}
		}
	}

	struct Node {
		vector<int> path;
		bool visited[MAXV]; // visited outward path exists from that vertex
		int tourLength;

		friend bool operator < (const Node &n1, const Node &n2);

		Node() {
			visited[0] = true;
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
			
			// Have to test this commented section
			
			/*
			for ( int i = 0 ; i < graph.V ; i++ ) {
				if ( !visited[i] ) {
					int shortest;
					int j;
					for ( j = 0 ; j < graph.V && visited[closestVertex[i][j]] ; j++ ) {}
					shortest = graph.weight[i][closestVertex[i][j]];
					tourLength += shortest;
				}
			}
			*/
			
			// Workaround the commented section
			
			for ( int i = 0 ; i < graph.V ; i++ ) {
				if ( !visited[i] ) {
					tourLength += graph.weight[i][closestVertex[i][0]];
				}
			}
		}
	};

	bool operator < (const Node &n1, const Node &n2) { // reversed comparison to make priority_heap min_heap
		if ( n1.tourLength < n2.tourLength ) return false;
		if ( n1.tourLength == n2.tourLength && n1.path.size() > n2.path.size() ) return false;
		return true;
	}

	void tspSolve(Graph graph, OptimalPath &optimalPath) {
		TSPSolve::graph = graph;
		generateClosestVertex();
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

			// Found a new good path                                   // NOTE: Is this necessary?
			if ( node.path.size() == graph.V + 1 ) {
				bestLength = node.tourLength;
				bestPath = node.path;
				continue;
			}

			// Separately need to complete the tour (because, otherwise, premature completion may occur
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

		// May have TODO


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
