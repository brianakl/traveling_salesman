#ifndef TSP_H
#define TSP_H

#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/graph_traits.hpp>
#include <boost/property_map/property_map.hpp>
#include <boost/dynamic_bitset.hpp>
#include <iostream>
#include <utility>
#include <unordered_map>
#include <queue>
#include <vector>


using namespace std;
using namespace boost;

enum mark_t {UNVISITED, VISITED, DISCOVERED, START};
enum input_type{DIST_MATRIX, COORDINATES};

//General Graph and Graph edge weight property
typedef property<edge_weight_t, double> EdgeWeightProperty;
//typedef property<vertex_color_t, default_color_type> VertexColorProperty;
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
//typedef property_map<Graph, edge_index_t>::type EdgeMap;
//typedef property_map<Graph, vertex_color_t>::type ColorMap;

//out edge iterator
typedef graph_traits<Graph>::out_edge_iterator out_edge_iter;
typedef pair<out_edge_iter, out_edge_iter> out_edge_pair;

inline size_t key(int i,int j) {return (size_t) i << 32 | (unsigned int) j;}


class TSP{
	private:
		int n; //number of vertices
		int e; //number of edges
		int min,c,p,best_genetic_distance, GEN_LIMIT = 4, j;
		double m;

		//for the dp function it creates a 2^n by n matrix
		vector<vector<double> > dp;
		vector<vector<int> > path;
		vector<int> parent,best_genetic_path;
		
		//adjacency list
		Graph *adj_list;

		//edge weight property map
		EdgeWeightMap edge_weight_map;
		vertex_pair vertex_set;
		vector<int> best;
		vector<int> cycle;
		vector<mark_t> mrk;
		//index map for vertices
		IndexMap index_map;

	public:
		TSP(const string& fileName, input_type);
		int getNeighbors() const;
		void tsp_brute();
		void tsp_brute_forceR(vertex_des, vertex_des, double dist = 0);
		double distance(vector<int>, int i = 0);
		vector<int> nearest_neighbor_path() const;
		void print_path(vector<int>);
		int get_vertices(){return n;};
		vector<int> christofides();
		vector<edge_des>* min_perfect_matching(vector<int>*, vector<edge_des>,int);
		edge_des int_to_edge(int,int);
		vector<int> hierholzer(vector<edge_des>);
		int hierholzer_euler_tourR(vector<edge_des>&,int, int);
		double dynamic_tsp(int = 1, int = 0); 
		void print_dtsp();
		int ith_bit(int);
		void genetic_starter(int pop = 50);
		void genetic_tsp(vector<vector<int> >&, vector<double>&, int, int gen = 0);
		vector<int> selection(vector<vector<int> >&, vector<double>&, int);
		vector<int> breed(vector<int>&, vector<int>&);
		void mutate(vector<int>&);


};


#endif