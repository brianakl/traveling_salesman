#include "TSP.h"
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/graph_traits.hpp>
#include <boost/graph/kruskal_min_spanning_tree.hpp>
#include <iostream>
#include <map>
#include <fstream>
#include <utility>
#include <queue>
#include <vector>
#include <chrono>

using namespace std;
using namespace boost;

TSP::TSP(const string& fileName, input_type type){
  ifstream ifile;
  ifile.open(fileName);
  if (!ifile){
    cout << "File name: " << fileName << " does not exist" << endl;
    exit(1);
  }

  ifile >> this->n;
  ifile >> this->e;

  adj_list = new Graph(n);

  if (type == DIST_MATRIX){
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
      for (int j = 0; j < i ; j++){
        if (matrix[i][j] == 0) continue;
        ewp = matrix[i][j];
        add_edge(i,j, ewp, *adj_list); 
      }
    }


  } else if (type == COORDINATES){
    double f = 0, s = 0;
    vector<pair<double, double> > coords;
    for (int i = 0; i < n; i++){
      ifile >> f >> s;
      coords.push_back(make_pair(f,s));
    }
    //add edges
    EdgeWeightProperty ewp;
    double x1,x2,y1,y2;
    x1 = x2 = y1 = y2 = 0.0;
    //calculates the edge weights for a graph that is euclidean 
    for (int i = 1; i < n; i++){
      for (int j = 0; j < n; j++){
        if (i == j) continue;
        x1 = coords[i].first;
        y1 = coords[i].second;
        x2 = coords[j].first;
        y2 = coords[j].second;
        ewp = sqrt(pow(abs(x2-x1),2) + pow(abs(y2-y1),2));
        add_edge(i,j,ewp,*adj_list);
      }
    }

  }
  vertex_set = vertices(*adj_list);
  for (int i = 0; i < n; i++){
    mrk.push_back(UNVISITED);
  }

  ifile.close();
  min = 1000000;


}
void TSP::tsp_brute(){
  tsp_brute_forceR(index_map[0],index_map[0]);
  print_path(best);
}

//this function finds the shortest path by trying every possible path
void TSP::tsp_brute_forceR(vertex_des v, vertex_des start, double dist){
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


vector<int> TSP::christofides(){
  //create minimum spanning tree
  vector<edge_des> spanning_tree;
  kruskal_minimum_spanning_tree(*adj_list, back_inserter(spanning_tree));
  //calculate vertices with odd degree
  vector<int> visit(n,0);
  vector<int> odd_vert;
  for (int i =0 ; i < n-1;i++){
    visit[spanning_tree[i].m_source]++;
    visit[spanning_tree[i].m_target]++;
  }
  int odd = 0;
  for(int i =0; i < n; i++){
    if(visit[i] %2 == 0) continue;
    odd_vert.push_back(visit[i]);
    odd++;
  }
  //find minimum weight perfect matching
  vector<pair<int,int> > perfect_match = min_perfect_matching(odd_vert);


}

vector<pair<int,int> > TSP::min_perfect_matching(vector<int> verts){
  //create a matching 
  //pick a vertex and find a matching of minimum weight
  vector<pair<int,int> > ret;
  
  
}