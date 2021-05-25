#ifndef GRAPH_H
#define GRAPH_H


#include "boost/graph/adjacency_list.hpp"
#include <iostream>
#include <string>
#include <unordered_map>

using namespace std;

enum mart_t {UNVISITED, VISITED};
enum graph_type {ADJ_LIST, ADJ_MATRIX};



class Graph{
	private:
		int n; //number of verticies'
		int e;
		//adjacency list
		typedef boost::property<edge_weight_t, double> EdgeWeightProperty;
		typedef boost::adjacency_list<boost::listS, boost::vecS, boost::undirectedS, boost::no_property, EdgeWeightProperty> adj_list;




	public:
		Graph(const string& fileName, graph_type);
		int getNeighbors() const;
		void tsp() const;
		void tsp_brute_force() const;

}


#endif
