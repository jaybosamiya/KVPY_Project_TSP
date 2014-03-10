#include "constants.h"
#include "Graph.h"
#include "OptimalPath.h"

#ifndef FILE_TSPSOLVE_H
#define FILE_TSPSOLVE_H

void tspSolve(Graph graph, OptimalPath &optimalPath) {
	// TODO: Remove garbage code and rewrite

	// Garbage Code
	optimalPath.V = graph.V;
	for ( int i = 0 ; i < graph.V ; i++ ) {
		optimalPath.location[i] = i;
	}
}

#endif