#ifndef TSP_H
#define TSP_H

#include <boost/graph/adjacency_list.hpp>
#include <iostream>
#include <string>
#include <utility>


using namespace std;

enum mart_t {UNVISITED, VISITED};
enum graph_type {ADJ_LIST, ADJ_MATRIX};



class TSP{
	private:
		int n = 0; //number of verticies'
		int e = 0; //number of edges
		//adjacency list
		typedef boost::property<boost::edge_weight_t, double> EdgeWeightProperty;
		typedef boost::adjacency_list<boost::listS, boost::vecS, boost::undirectedS, boost::no_property, EdgeWeightProperty> Graph;
		




	public:
		TSP(const string& fileName);
		int getNeighbors() const;
		void tsp() const;
		void tsp_brute_force() const;

};


#endif
