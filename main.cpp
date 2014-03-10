#include <iostream>
#include <iomanip>
#include <cstdlib>
#include <ctime>
#include <algorithm>

#include "constants.h"
#include "Graph.h"
#include "OptimalPath.h"
#include "tspSolve.h"

int main(int argc, char **argv) {
	Graph graph;
	OptimalPath optimalPath;
	int V = DEFAULTV;
	unsigned int seed = 1;

	// Generate graph
	if ( argc >= 2 ) {
		if ( argc >= 4 ) {
			std::cerr << "Incorrect number of parameters. 0 or 2 or 3 expected. ";
			return 0; // shouldn't it return an error code?????
		}
		V = atoi(argv[1]);
		if ( V == 0 ) {
			std::cerr << "Incorrect parameter: " << argv[1] << ". Size of graph expected. ";
			return 0; // shouldn't it return an error code?????
		}
		if ( argc == 3 ) {
			seed = atoi(argv[2]);
		}
	}
	if ( argc != 3 ) {
		#ifndef DEBUG
		// cause a randomization in the seed based on time.
		// 10009 and 10007 because they are primes.
		seed = (time(NULL) % 10009);
		seed *= seed;
		seed %= 10009;
		seed *= seed;
		seed %= 10007;
		seed *= seed;
		seed %= 10009;
		srand(seed);
		seed = rand() % 10009;
		#endif
	}
	if ( V > MAXV ) {
			std::cerr << "Error: Too many cities.";
			return 0; // shouldn't it return an error code?????
	}
	if ( V <= 2 ) { // because it doesn't make sense for a TSP path to exist for less than 3 cities
			std::cerr << "Error: Too few cities.";
			return 0; // shouldn't it return an error code?????
	}
	srand(seed);
	graph.V = V;
	#ifdef SHOWCOSTS
	std::cout << "    Cities     Cost\n"
	          << "-------------- ----\n\n";
	#endif
	for ( int i = 0 ; i < V ; i++ ) {
		for ( int j = i+1 ; j < V ; j++ ) {
			int randomCost = (rand() % MAXCOST) + 1;
			graph.weight[i][j] = randomCost;
			graph.weight[j][i] = randomCost;
			#ifdef SHOWCOSTS
			std::cout << std::setw(4) << i << "  " << std::setw(4) << j << "   " << std::setw(5) << randomCost << "\n";
			#endif
		}
	}

	// Call solver

	tspSolve(graph,optimalPath);

	// Display solution
	std::cout << "\n\n"
	          << "Optimal TSP path:\n\t";
	for ( int i = 0 ; i < optimalPath.V ; i++ ) {
		std::cout << optimalPath.location[i] << " ";
	}

	// Display run info

	std::cout << "\n\nRun same test case again using: \n\"" << argv[0] << "\" " << V << " " << seed << "\n";

}
