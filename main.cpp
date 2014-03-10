#include <iostream>
#include <iomanip>
#include <cstdlib>
#include <ctime>
#include <algorithm>
#include <cerrno>

#include "constants.h"
#include "Graph.h"
#include "OptimalPath.h"
#include "tspSolve.h"

int main(int argc, char **argv) {
    int startTime;
    int endTime;
    Graph graph;
    OptimalPath optimalPath;
    int V = DEFAULTV;
    bool manualInput = 0;
    bool uniformInput = 0;
    unsigned int seed = 1;

    // Generate graph
    if ( argc >= 4 ) {
        std::cerr << "Incorrect number of parameters. 0 or 2 or 3 expected. ";
        return E2BIG; // Argument list too big error
    }
    if ( argc >= 2 ) {
        V = atoi(argv[1]);
        if ( V == -1 ) {
            std::cin >> V;
        }
        if ( V == 0 ) {
            std::cerr << "Incorrect parameter: " << argv[1] << ". Size of graph expected. ";
            return EINVAL; // Invalid parameter error
        }
        if ( argc == 3 ) {
            seed = atoi(argv[2]);
            if ( seed == -1 ) {
                manualInput = true;
            }
            if ( seed == -2 ) {
                uniformInput = true;
            }
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
        return ERANGE; // Out of range error
    }
    if ( V <= 2 ) { // because it doesn't make sense for a TSP path to exist for less than 3 cities
        std::cerr << "Error: Too few cities.";
        return EDOM; // Domain of function error
    }
    srand(seed);
    graph.V = V;
#ifdef SHOWCOSTS
    std::cout << "    Cities     Cost\n"
              << "-------------- ----\n\n";
#endif
    if ( manualInput ) {
        for ( int i = 0 ; i < V ; i++ ) {
            graph.weight[i][i] = INFINITY;
            for ( int j = i+1 ; j < V ; j++ ) {
                std::cout << std::setw(4) << i+OFFSET << "  " << std::setw(4) << j+OFFSET << "   " << "   ";
                int cost;
                std::cin >> cost;
                graph.weight[i][j] = cost;
                graph.weight[j][i] = cost;

            }
        }
    } else {
        for ( int i = 0 ; i < V ; i++ ) {
            graph.weight[i][i] = INFINITY;
            for ( int j = i+1 ; j < V ; j++ ) {
                int randomCost = 1;
                if (!uniformInput) randomCost = (rand() % MAXCOST) + 1;
                graph.weight[i][j] = randomCost;
                graph.weight[j][i] = randomCost;
#ifdef SHOWCOSTS
                std::cout << std::setw(4) << i+OFFSET << "  " << std::setw(4) << j+OFFSET << "   " << std::setw(5) << randomCost << "\n";
#endif
            }
        }
    }

    // Call solver

    startTime = time(NULL);

    tspSolve(graph,optimalPath);

    // Display solution
    std::cout << "\n\n"
              << "Optimal TSP path:\n\t";
    for ( int i = 0 ; i < optimalPath.V ; i++ ) {
        std::cout << optimalPath.location[i]+OFFSET << " ";
    }
    std::cout << optimalPath.location[0]+OFFSET << "\n\t\tgives a length of "
              << optimalPath.length << "\n";


    // Display run info

    if ( !manualInput ) std::cout << "\n\nRun same test case again using: \n\"" << argv[0] << "\" " << V << " " << seed << "\n";

    endTime = time(NULL);

#ifndef DONTSHOWTIME
    std::cout << "\n\nRan in " << endTime - startTime << " seconds.";
#endif
}
