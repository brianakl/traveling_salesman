#ifndef TSP_H
#define TSP_H

#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/graph_traits.hpp>
#include <boost/property_map/property_map.hpp>
#include <map>
#include <iostream>
#include <utility>
#include <stack>
#include <vector>


using namespace std;
using namespace boost;

enum mark_t {UNVISITED, VISITED};
//General Graph and Graph edge weight property
typedef property<edge_weight_t, double> EdgeWeightProperty;
typedef adjacency_list<vecS, vecS, undirectedS, no_property, EdgeWeightProperty> Graph;

//Edge typedefs
typedef graph_traits<Graph>::edge_iterator edge_iter;
typedef graph_traits<Graph>::edge_descriptor edge_des;
typedef pair<edge_iter, edge_iter> edge_pair;

//vertex typedef
typedef graph_traits<Graph>::vertex_iterator vertex_iter;
typedef graph_traits<Graph>::vertex_descriptor vertex_des;
typedef pair<vertex_iter, vertex_iter> vertex_pair;

//property maps
typedef property_map<Graph, vertex_index_t>::type IndexMap;
typedef property_map<Graph, edge_weight_t>::type EdgeWeightMap;

//out edge iterator
typedef graph_traits<Graph>::out_edge_iterator out_edge_iter;
typedef pair<out_edge_iter, out_edge_iter> out_edge_pair;



class TSP{
	private:
		int n; //number of vertices
		int e; //number of edges
		//adjacency list
		Graph *adj_list;

		//edge weight property map
		EdgeWeightMap edge_weight_map;
		//index map for vertices
		IndexMap index_map;

	public:
		TSP(const string& fileName);
		int getNeighbors() const;
		void tsp() const;
		stack<double> tsp_brute_force() const;
		double distance(stack<edge_pair>) const;
		stack<int> nearest_neighbor_path() const;


};


#endif