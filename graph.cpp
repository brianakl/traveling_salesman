#include "TSP.hpp"
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/dijkstra_shortest_paths.hpp>
#include <boost/graph/graph_traits.hpp>
#include <iostream>
#include <string>
#include <map>
#include <fstream>
#include <utility>

using namespace std;
using namespace boost;

TSP::TSP(const string& fileName){
  ifstream ifile;
  ifile.open(fileName);
  if (!ifile){
    cout << "File name: " << fileName << " does not exist" << endl;
    exit(1);
  }

  ifile >> this->n;
  ifile >> this->e;


  Graph adj_list(n);
  double matrix [this->n][this->n];

  for(int i = 0; i < this->n; i++){
    for(int j = 0; j < this->n; j++){
      ifile >> matrix[i][j];
    }
  }
  //boost::adjacency_list<boost::listS, boost::vecS, boost::undirectedS, boost::no_property, EdgeWeightProperty> adj_list(this->n);

  //add all verticies
  typedef graph_traits<Graph>::vertex_descriptor vertex_t;

  for (int i = 0; i < this->n; i++){
    vertex_t v = add_vertex(adj_list);
    
  }
  

  //add edges with weights
  //adds an edge between vertex i and j of weight matrix[i][j] to the graph
  EdgeWeightProperty ewp;
  for (int i = 1; i < this->n; i++){
    for (int j = 0; matrix[i][j] != 0.0; j++){
      ewp = matrix[i][j];
      add_edge(i,j, ewp, adj_list); 
    }
  }



  //how to access a vertex
  


  //get the property map for vertex indices
  //typedef property_map<Graph,vertex_index_t>::type IndexMap;
  index_map = get(vertex_index, adj_list);

  cout << "vertices(adj_list) = ";
  vertex_pair vp;
  vertex_des v;
  for (vp = vertices(adj_list); vp.first != vp.second; vp.first++){
    v = *vp.first;
    cout << index_map[v] << " ";
  }

  cout << endl;

  

  //accessing edges
  cout << "edges(adj_list) = ";
  edge_iter ei, ei_end;
  for (tie(ei,ei_end) = edges(adj_list); ei != ei_end; ei++){
    cout << "(" << index_map[source(*ei, adj_list)] << "," << index_map[target(*ei,adj_list)]
      << ") ";
  }
  cout << endl;


edge_weight_map = get(edge_weight_t(), adj_list);
edge_pair ep;
for (ep = edges(adj_list); ep.first != ep.second; ep.first++){
  cout << edge_weight_map[*ep.first] << " ";
}
cout << endl;

 



  ifile.close();


}
