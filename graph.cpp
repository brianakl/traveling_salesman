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
  for (int i = 1; i < this->n; i++){
    for (int j = 0; matrix[i][j] != 0.0; j++){
      EdgeWeightProperty ewp = matrix[i][j];
      add_edge(i,j, ewp, adj_list); 
    }
  }

  for (int i = 0; i < this->n; i++){
    for (int j = 0; j < this->n; j++){
      cout << matrix[i][j] << " ";
    }
    cout << endl;
  }


  //how to access a vertex
  
  typedef graph_traits<Graph>::vertex_descriptor Vertex;

  //get the property map for vertex indices
  typedef property_map<Graph,vertex_index_t>::type IndexMap;
  IndexMap index = get(vertex_index, adj_list);

  cout << "vertices(adj_list) = ";
  typedef graph_traits<Graph>::vertex_iterator vertex_iter;
  pair<vertex_iter, vertex_iter> vp;
  for (vp = vertices(adj_list); vp.first != vp.second; vp.first++){
    Vertex v = *vp.first;
    cout << index[v] << " ";
  }

  cout << endl;

  

  //accessing edges
  cout << "edges(adj_list) = ";
  graph_traits<Graph>::edge_iterator ei, ei_end;
  for (tie(ei,ei_end) = edges(adj_list); ei != ei_end; ei++){
    cout << "(" << index[source(*ei, adj_list)] << "," << index[target(*ei,adj_list)]
      << ") ";
  }
  cout << endl;

  //accessing edge weights
  //does not work
  /*
  vector<double> d;

  Vertex s = *(vertices(adj_list).first);
  dijkstra_shortest_paths(adj_list,s,distance_map(&d[0]));

  cout << "distances from start vertex:" << endl;
  graph_traits<Graph>::vertex_iterator vi;
  for (vi = vertices(adj_list).first; vi != vertices(adj_list).second; ++vi){
    cout << "distance(" << index(*vi) << ") = "
         << d[*vi] << endl;
  }
  cout << endl;

  */

EdgeWeightMap = get(edge_weight_t(), adj_list);
pair<edge_it, edge_it> edge_pair;
for (edge_pair = edges(adj_list); edge_pair.first != edge_pair.second; edge_pair.first++){
  cout << EdgeWeightMap[*edge_pair.first] << " ";
}
cout << endl;

 



  ifile.close();


}
