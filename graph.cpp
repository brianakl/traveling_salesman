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
#include <unordered_map>

using namespace std;
using namespace boost;

//constructor
//takes a file name and input type
//input type can be a list of coordinates or a distance matrix
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
        ewp = sqrt(pow(x2-x1,2) + pow(y2-y1,2));
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

  cout << "MST: ";
  for (auto it : spanning_tree)
    cout << it << edge_weight_map[it] << " ";

  cout <<endl;

  vector<int> visit(n,0);
  vector<int> odd_vert;
  for (int i =0 ; i < n-1;i++){
    visit[spanning_tree[i].m_source]++;
    visit[spanning_tree[i].m_target]++;
  }
  int odd = 0;
  for(int i = 0; i < n; i++){
    if(visit[i] %2 == 0) continue;
    cout << i << " ";
    odd_vert.push_back(i);
    odd++;
  }
  
  
  //find minimum weight perfect matching (MPM)
  vector<edge_des>* perfect_match;
  cout << "seg" << endl;
  //*perfect_match = min_perfect_matching(&odd_vert,odd);
  perfect_match = min_perfect_matching(&odd_vert, odd);
  //add the edges of the MPM to the MST
  while(!perfect_match->empty()){
    spanning_tree.push_back(perfect_match->back());
    perfect_match->pop_back();
  }

  //calculate a euler tour
  //pick a vertex and using the MST + MPM pick out an edge to travel to 
  //store the order the vertices that are visited in
  //make sure that if the next edge being traveled to is the starting edge
  //that there arent any other edges to travel to first
  vector<int> euler_t = hierholzer(spanning_tree, spanning_tree.back().m_source, spanning_tree.back().m_source);

  exit(0);

  //remove duplicate vertices and return the solution
  vector<int> ret;
  bool duplicates[n];
  for (int i =0; i < n; i++) duplicates[i] = false;

  for (auto it : euler_t){
    if (duplicates[it])
      continue;
    ret.push_back(it);
  }

  distance(ret);
  print_path(ret);
  return ret;


}


vector<edge_des>* TSP::min_perfect_matching(vector<int>* verts, int odd){
  //create a matching 
  //pick a vertex and find a matching of minimum weight
  
  vector<edge_des> *ret = new vector<edge_des>;

  auto comp = [](edge_des u, edge_des v){return u.m_eproperty < v.m_eproperty;};

  priority_queue <edge_des, vector<edge_des>, decltype(comp)> pq(comp);
  cout << "odd verts ";
  for (auto it : *verts)
    cout << it << " ";
  //adding every edge to the pq
  for(int i = 0; i < odd; i++){
    for (int j = i + 1; j < odd; j++){
      pq.push(int_to_edge(verts->at(i),verts->at(j)));
      cout << "(" << verts->at(j) <<"," << verts->at(i) << ")" << " " << pq.top() << endl;
    }
  }

  cout << endl;
  //find the minimum edges till all vertices are perfect matched
  bool used[odd];
  for (int i = 0; i < odd; i++)
    used[i] = false;

  edge_des ed;

  for(int i = 0; !pq.empty(); i++){
    ed = pq.top();
    cout << ed << " " ;
    if (used[ed.m_source] == true || used[ed.m_target] == true) {
      pq.pop();
      continue;
    }
    used[ed.m_source] = true;
    used[ed.m_target] = true;
    ret->push_back(ed);
    pq.pop();
    
  }
  cout << endl;
  cout << ret->size();
  for (int i =0; i < ret->size(); i++)
    cout << ret->at(i) << " ";
  cout << endl;
  return ret;  
  
}

vector<int> TSP::hierholzer(vector<edge_des> tree, int start, int current){
  vector<int> ret;
  
}


//hierholzers algorithm
int TSP::hierholzer_euler_tourR(vector<edge_des> tree, int start, int current){
  
  vector<int> ret;
  //end case
  if (tree.empty()){
    ret.push_back(start);
    return ret;
  }
  int next = 0, ne = 0;
  vector<edge_des> degree;
  //gets the degree of the current vertex
  for(auto it : tree){
    if(it.m_target == current || it.m_target == current)
      degree.push_back(it);
  }
  //make sure that the next vertex isnt current

  if(degree.back().m_source == current) next = degree.back().m_target;
  else next = degree.back().m_source;
  //find next in tree and remove it
  int i =0;
  for (auto it:tree){
    i++;
    if (it == degree.back()) break;
  }
  tree.erase(tree.begin() + i);
  
  ret.push_back(current);
  cout << "1 ";
  vector<int> *rec = new vector<int>;
  rec = hierholzer_euler_tour(tree,start, next);
  exit(0);
  ret.insert(ret.end(),rec.begin(),rec.end());
  return ret;
}


edge_des TSP::int_to_edge(int v, int u){
  out_edge_pair out_p = out_edges(index_map[v], *adj_list);
  for (;out_p.first != out_p.second; out_p.first++){
    if(target(*out_p.first, *adj_list) == index_map[u]){ 
      cout << "return of int to edge: (" << target(*out_p.first,*adj_list)<< "," << source(*out_p.first,*adj_list) << ") \n";
      return *out_p.first;
    }
  }
  
  return *out_p.second;
}