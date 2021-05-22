#ifndef GRAPH_H
#define GRAPH_H


#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/adjacency_matrix.hpp>
#include <iostream>
#include <string>

using namespace std;
using namespace boost;

enum mart_t {UNVISITED, VISITED};

struct vertex{
	int id;
	vertex* ;
}

class Graph{
	private:
		//adjacency matrix
		adjacency_matrix<undirectedS> adj_matrix;
		//adjacency list
		adjacency_list<undirectedS> adj_list;




	public:
		Graph(const string& fileName);
		int getNeighbors() const;
		void tsp() const;
		void tsp_brute_force() const;

}


#endif
