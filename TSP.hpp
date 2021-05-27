#ifndef TSP_H
#define TSP_H

#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/graph_traits.hpp>
#include <boost/property_map/property_map.hpp>
#include <map>
#include <iostream>
#include <string>
#include <utility>
#include <stack>
#include <vector>


using namespace std;
using namespace boost;

enum mark_t {UNVISITED, VISITED};
typedef property<edge_weight_t, double> EdgeWeightProperty;

typedef adjacency_list<vecS, vecS, undirectedS, no_property, EdgeWeightProperty> Graph;

typedef property_map<Graph, edge_weight_t>::type EdgeWeightMap;
typedef property_map<Graph, vertex_index_t>::type IndexMap;
typedef graph_traits<Graph>::edge_iterator edge_iter;
typedef graph_traits<Graph>::edge_descriptor edge_des;
typedef pair<edge_iter, edge_iter> edge_pair;

typedef graph_traits<Graph>::vertex_iterator vertex_iter;
typedef graph_traits<Graph>::vertex_descriptor vertex_des;
typedef pair<vertex_iter, vertex_iter> vertex_pair;



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

};


#endif