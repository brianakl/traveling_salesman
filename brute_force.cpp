#include "TSP.h"


void TSP::tsp_brute(){
  tsp_brute_forceR(index_map[0],index_map[0]);
  print_path(best);
}

//this function finds the shortest path by trying every possible path recursively
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
