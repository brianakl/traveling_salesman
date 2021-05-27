#include "TSP.hpp"
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/dijkstra_shortest_paths.hpp>
#include <boost/graph/graph_traits.hpp>
#include <boost/graph/graph_traits.hpp>
#include <iostream>
#include <string>
#include <map>
#include <fstream>
#include <utility>
#include <vector>

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

  adj_list = new Graph(n);
  double matrix [this->n][this->n];

  for(int i = 0; i < this->n; i++){
    for(int j = 0; j < this->n; j++){
      ifile >> matrix[i][j];
    }
  }

  //add edges with weights
  //adds an edge between vertex i and j of weight matrix[i][j] to the graph
  EdgeWeightProperty ewp;
  for (int i = 1; i < this->n; i++){
    for (int j = 0; matrix[i][j] != 0.0; j++){
      ewp = matrix[i][j];
      add_edge(i,j, ewp, *adj_list); 
    }
  }



  //how to access a vertex
  


  //get the property map for vertex indices
  //typedef property_map<Graph,vertex_index_t>::type IndexMap;
  this->index_map = get(vertex_index, *adj_list);

  cout << "vertices(adj_list) = ";
  vertex_pair vp;
  vertex_des v;
  for (vp = vertices(*adj_list); vp.first != vp.second; vp.first++){
    v = *vp.first;
    cout << index_map[v] << " ";
  }

  cout << endl;

  

  //accessing edges
  /*
  cout << "edges(adj_list) = ";
  edge_iter ei, ei_end;
  for (tie(ei,ei_end) = edges(adj_list); ei != ei_end; ei++){
    cout << "(" << index_map[source(*ei, adj_list)] << "," << index_map[target(*ei, adj_list)]
      << ") ";
  }
  cout << endl;


  this->edge_weight_map = get(edge_weight_t(), adj_list);
  edge_pair ep;
  for (ep = edges(adj_list); ep.first != ep.second; ep.first++)
    cout << edge_weight_map[*ep.first] << " ";
  cout << endl << endl;

  

  //edges + edge weights


  edge_pair edg, ep;
  cout << "edges(adj_list)distance:  ";
  //for (tie(ei,ei_end) = edges(adj_list), ep = edges(adj_list);ei != ei_end; ei++, ep.first++)
  for (edg = edges(*adj_list), ep = edges(*adj_list); edg.first != edg.second; ep.first++, edg.first++)
    cout << "(" << index_map[source(*edg.first,*adj_list)] << "," <<
            index_map[target(*edg.first,*adj_list)] << ")" << edge_weight_map[*ep.first] <<
            " ";
  cout << endl;
  */



  ifile.close();


}


stack<double> TSP::tsp_brute_force() const{
  
  stack<double> p;
  edge_pair ep;
  p.push(2.3);
  cout << endl << endl;
  for (ep = edges(*adj_list); ep.first != ep.second; ep.first++){
    cout << edge_weight_map[*ep.first] << " " << *ep.first << endl;
  }
  cout << endl;

  vertex_pair vp;
  out_edge_iter out_end;
  edge_des ed;
  for (vp = vertices(*adj_list); vp.first != vp.second; vp.first++){
    cout << index_map[*vp.first];
    for (out_edge_pair out_i = out_edges(*vp.first, *adj_list); out_i.first != out_i.second; out_i.first++){
      ed = *out_i.first;
      vertex_des src = source(ed, *adj_list), targ = target(ed, *adj_list);
      cout << " (" << index_map[src] << "," << index_map[targ] << ")" << edge_weight_map[*out_i.first] << " ";
    }
    cout << endl;

  }

  return p;

}

double TSP::distance(stack<edge_pair> path) const{
  
}
