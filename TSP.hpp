#ifndef TSP_H
#define TSP_H

#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/graph_traits.hpp>
#include <boost/property_map/property_map.hpp>
#include <map>
#include <iostream>
#include <string>
#include <utility>


using namespace std;
using namespace boost;

enum mart_t {UNVISITED, VISITED};
enum graph_type {ADJ_LIST, ADJ_MATRIX};



class TSP{
	private:
		int n = 0; //number of verticies'
		int e = 0; //number of edges
		//adjacency list
		typedef property<edge_weight_t, double> EdgeWeightProperty;
		typedef adjacency_list<vecS, vecS, undirectedS, no_property, EdgeWeightProperty> Graph;
		Graph adj_list();
		property_map<Graph, edge_weight_t>::type EdgeWeightMap;
		typedef graph_traits<Graph>::edge_iterator edge_it;
		typedef graph_traits<Graph>::vertex_iterator v_it;


		




	public:
		TSP(const string& fileName);
		int getNeighbors() const;
		void tsp() const;
		void tsp_brute_force() const;

};


#endif