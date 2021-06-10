#include "TSP.h"
#include <vector>



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