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
  out_edge_iter out_end;
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

//returns the total distance of a path
double TSP::distance(queue<int> path) const{
  vertex_pair vp = vertices(*adj_list), dist;
  out_edge_pair out_i;
  double ret = 0;
  //find the vertex that matches the first node of the path
  while (index_map[*vp.first] != path.front() && vp.first+1 != vp.second){
    vp.first++;
  }
  dist = vp;

  out_i = out_edges(*dist.first, *adj_list);
  //find where the target of edge = vp



  return edge_weight_map[*out_i.first] + distance(path);
}

//creates a path using the nearest unvisited neighbor as the next choice
queue<int> TSP::nearest_neighbor_path() const{
  queue<int> ret;
  vertex_pair current = vertices(*adj_list), next;
  out_edge_pair out_i = out_edges(*current.first, *adj_list), shortest_edge = out_i, q;
  vertex_des closestTarg = target(*out_i.first, *adj_list),
             closestS = source(*out_i.first, *adj_list),
             temp;
  bool visited[n];

  for (int i =0; i <n ; i++)
    visited[i] = false;

  
  visited[0] = true;
  ret.push(0);
  for(int i = 0; i < n-1; i++){
    out_i = out_edges(*current.first, *adj_list);
    temp = source(*out_i.first, *adj_list);
    shortest_edge = out_i;

    //shortest_edge needs to be set to the first available edge
    //vertex it targets needs to not be visited
    
    while (visited[index_map[target(*shortest_edge.first, *adj_list)]] && shortest_edge.first+1 != shortest_edge.second)
      shortest_edge.first++;

    
    //traversing through the out_edges to find one with a lower edge weight


    closestTarg = target(*(shortest_edge.first), *adj_list);
    closestS = source(*(shortest_edge.first), *adj_list);
    //cout << "ClosestS: " <<index_map[closestS] << " ClosestTarg: " << index_map[closestTarg] << endl;
    for(;out_i.first != out_i.second; out_i.first++){
      //if this node is unvisited and the distance between current node > closest node then continue
      if (!visited[index_map[target(*out_i.first, *adj_list)]]) {
        //cout << "vertex: " << index_map[target(*out_i.first, *adj_list)] << endl;
        if (edge_weight_map[*shortest_edge.first] > edge_weight_map(*out_i.first)){
          closestTarg = target(*out_i.first, *adj_list);
          closestS = source(*out_i.first, *adj_list);
          shortest_edge = out_i;
          //cout << "ClosestTarg: " << index_map[closestTarg] << endl;
        }
      }      
    }
    visited[index_map[closestS]] = true;
    ret.push(index_map[closestTarg]);
    //search through the vertex list to find a vertex_des that matches closestTarg
    next = vertices(*adj_list);
    for (; next.first != next.second && *next.first != closestTarg; next.first++){

    }
    if (next.first == next.second)  break;

    //cout << "current.first : " << index_map[*current.first] << endl << "next.first: " << index_map[*next.first] << endl << endl;
    current.first = next.first;
    //cout << "new current.first: " << index_map[*current.first] << endl << endl;
    
  }


  return ret;
  

}
