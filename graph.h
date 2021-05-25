#ifndef GRAPH_H
#define GRAPH_H


#include "boost/graph/adjacency_list.hpp"
#include "boost/graph/adjacency_matrix.hpp"
#include <iostream>
#include <string>
#include <unordered_map>

using namespace std;
using namespace boost;

enum mart_t {UNVISITED, VISITED};
enum graph_type {ADJ_LIST, ADJ_MATRIX};


class Graph{
	private:
		int n; //number of verticies'
		int e;
		//unordered_map<int, string> state;	//map of vertex labels
		//adjacency matrix
		adjacency_matrix<undirectedS> adj_matrix;
		//adjacency list
		adjacency_list<undirectedS> adj_list;




	public:
		Graph(const string& fileName, graph_type);
		int getNeighbors() const;
		void tsp() const;
		void tsp_brute_force() const;

}


#endif
