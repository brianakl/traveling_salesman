#include "TSP.hpp"
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


/*

  //how to access a vertex
  
  //get the property map for vertex indices
  //typedef property_map<Graph,vertex_index_t>::type IndexMap;
  this->index_map = get(vertex_index, *adj_list);

  //cout << "vertices(adj_list) = ";
  vertex_pair vp;
  vertex_des v;
  for (vp = vertices(*adj_list); vp.first != vp.second; vp.first++){
    v = *vp.first;
    cout << index_map[v] << " ";
  }

  cout << endl;

  

  //accessing edges
  
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

//this function finds the shortest path by trying every possible path
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
  edge_des ed;
  for (vp = vertices(*adj_list); vp.first != vp.second; vp.first++){
    cout << index_map[*vp.first];
    for (out_edge_pair out_i = out_edges(*vp.first, *adj_list); out_i.first != out_i.second; out_i.first++){
      vertex_des src = source(*out_i.first, *adj_list), 
                targ = target(*out_i.first, *adj_list);
      cout << " (" << index_map[src] << "," << index_map[targ] << ")" << 
              edge_weight_map[*out_i.first] << " ";
    }
    cout << endl;
  }

  return p;
  

}

//recursively returns the total distance of a path
double TSP::distance(vector<int> path, int i) const{
  out_edge_pair out_i;
  double ret = 0;

  //if the current vertex is the last vertex in the vector the the edge
  //weight between the first and last need to be returned
  if (path[i+1] == *path.end()){
    //1) find coressponding out edges
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
