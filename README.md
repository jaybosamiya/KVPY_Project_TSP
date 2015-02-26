Optimal Solution to TSP using Branch and Bound
==============================================

-------------------------

Objective
---------

The Travelling Salesman/person Problem (TSP) asks the following question: Given a list of cities and the distances between each pair of cities, what is the shortest possible route that visits each city exactly once and returns to the origin city? The naïve solution to solving it would be to enumerate all possible paths and choose the one with minimum path length. A better solution would be to prune away all paths that are necessarily sub-optimal even before enumerating them. This method is called Branch and Bound. The objective is to implement this technique and find the best TSP tour for a given set of cities (represented as a complete weighted graph).

Solution
--------
 
For each city (vertex) to be visited exactly once, it is trivial to show that, in the TSP tour, there will be exactly two edges that have this vertex as the end point. Also, since the roads are bi-directional and have same distance both ways, we can assign one edge as incoming and other as outgoing for each vertex (without loss of generality). Now, consider the edges that are outgoing from each vertex. These will have weights that are equal to or greater than the smallest outgoing edge from that vertex. This means that we can obtain a lower bound on a TSP tour if even part of it is fixed. At each point of time, one can store the best TSP tour obtained so far and discard any partial tour whose lower bound is greater than (or equal to) the current best. In this way, multiple tours which are sub-optimal are discarded in one swoop without the need to enumerate each one of them.
 
It becomes useful to implement this using “best first” method of traversal of the state space. This can be done by storing each state as a node in a priority queue ordered by lower bound of the partial tour.

By pre calculating the shortest outgoing edge from each vertex, it becomes even more faster to calculate the lower bound on the partial tour.

When no more state space nodes remain in the priority queue, the optimal tour has been found.


License
-------

[MIT License](http://jay.mit-license.org/2013)
