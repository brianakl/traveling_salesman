#include "TSP.h"
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/graph_traits.hpp>
#include <iostream>
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


  vertex_set = vertices(*adj_list);
  for (int i = 0; i < n; i++){
    mrk.push_back(UNVISITED);
  }

  ifile.close();


}
void TSP::tsp_brute(){
  tsp_brute_forceR(index_map[0],index_map[0]);
  print_path(best);
}

//this function finds the shortest path by trying every possible path
void TSP::tsp_brute_forceR(vertex_des v, vertex_des start, int dist){
  cycle.push_back(index_map[v]);
  mrk[index_map[v]] = VISITED;
  bool atEnd = true;
  out_edge_pair out_i, out_u;
  for(out_i = out_edges(v, *adj_list); out_i.first != out_i.second; out_i.first++){
    if (mrk[index_map[target(*out_i.first, *adj_list)]] == UNVISITED){
      tsp_brute_forceR(target(*out_i.first, *adj_list), start, dist + edge_weight_map[*out_i.first]);
      atEnd = false;
    }
  }

  if (atEnd){
    for(out_u = out_edges(index_map[start], *adj_list); out_u.first != out_u.second; out_u.first++){
      if (target(*out_u.first, *adj_list) == index_map[v]) break;
    }
    dist += edge_weight_map[*out_u.first];
    if (dist < min){
      min = dist;
      best = cycle;
    }
  }
  mrk[index_map[v]] = UNVISITED;
  cycle.pop_back();
}

void TSP::print_path(vector<int> path) {
  cout << "Best Cycle:" << endl;
  for (int i = 0; i < n; i++){
    cout << path[i] << " ";
  }
  cout << endl << "Total Distance: " << distance(path) << endl;
}

//recursively returns the total distance of a path
double TSP::distance(vector<int> path, int i) const{
  out_edge_pair out_i;
  double ret = 0;

  //if the current vertex is the last vertex in the vector the the edge
  //weight between the first and last need to be returned
  if (path[i+1] == *path.end()){
    out_i = out_edges(index_map[path[0]], *adj_list);

    //find where target(out_i.first) == index_map[path[i]]
    for (;out_i.first != out_i.second; out_i.first++){
      if (target(*out_i.first, *adj_list) == index_map[path[i]]) break;
    }
    return edge_weight_map[*out_i.first];
  }

  for (out_i = out_edges(index_map[path[i]], *adj_list); out_i.first != out_i.second; out_i.first++){
    if (target(*out_i.first, *adj_list) == index_map[path[i+1]]) break;
  }
  return edge_weight_map[*out_i.first] + distance(path, ++i);
}

//creates a path using the nearest unvisited neighbor as the next choice
vector<int> TSP::nearest_neighbor_path() const{
  vector<int> ret;
  vertex_pair current = vertices(*adj_list), next;
  out_edge_pair out_i = out_edges(*current.first, *adj_list), shortest_edge = out_i, q;
  vertex_des closestTarg = target(*out_i.first, *adj_list);

  bool visited[n];

  for (int i =0; i <n ; i++)
    visited[i] = false;

  
  visited[0] = true;
  ret.push_back(0);
  for(int i = 0; i < n-1; i++){
    out_i = out_edges(*current.first, *adj_list);
    shortest_edge = out_i;

    //shortest_edge needs to be set to the first available edge
    //vertex it targets needs to not be visited
    
    for (;shortest_edge.first+1 != shortest_edge.second; shortest_edge.first++){
      if (!visited[index_map[target(*shortest_edge.first, *adj_list)]]) break;
    }

    //traversing through the out_edges to find one with a lower edge weight


    closestTarg = target(*(shortest_edge.first), *adj_list);

    for(;out_i.first != out_i.second; out_i.first++){
      //if this node is unvisited and the distance between current node > closest node then continue
      if (!visited[index_map[target(*out_i.first, *adj_list)]]) {
        if (edge_weight_map[*shortest_edge.first] > edge_weight_map(*out_i.first)){
          closestTarg = target(*out_i.first, *adj_list);
          shortest_edge = out_i;

        }
      }      
    }
    visited[index_map[*current.first]] = true;
    ret.push_back(index_map[closestTarg]);

    //search through the vertex list to find a vertex_des that matches closestTarg
    for (next = vertices(*adj_list); next.first != next.second; next.first++){
      if (*next.first == closestTarg) break;
    }
    if (next.first == next.second)  break;

    current.first = next.first;

    
  }


  return ret;
  

}
