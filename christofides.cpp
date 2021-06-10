#include "TSP.h"
#include <boost/graph/kruskal_min_spanning_tree.hpp>
#include <iostream>
#include <queue>
#include <vector>


using namespace std;
using namespace boost;

vector<int> TSP::christofides(){
  //create minimum spanning tree
  vector<edge_des> spanning_tree;
  kruskal_minimum_spanning_tree(*adj_list, back_inserter(spanning_tree));
  //calculate vertices with odd degree


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
    odd_vert.push_back(i);
    odd++;
  }
  
  //find minimum weight perfect matching (MPM)
  vector<edge_des>* perfect_match;
  //*perfect_match = min_perfect_matching(&odd_vert,odd);
  perfect_match = min_perfect_matching(&odd_vert, spanning_tree, odd);
  //add the edges of the MPM to the MST

  while(!perfect_match->empty()){
    spanning_tree.push_back(perfect_match->back());
    perfect_match->pop_back();
  }
  
  //calculate a euler tour
  //pick a vertex and using the MST + MPM pick out an edge to travel to 
  //store the order the vertices are visited in
  //make sure that if the next edge being traveled to is the starting edge
  //that there arent any other edges to travel to first
  vector<int> euler_t = hierholzer(spanning_tree);


  //remove duplicate vertices and return the solution
  vector<int> ret;
  bool duplicates[n];
  for (int i =0; i < n; i++) duplicates[i] = false;

  for (auto it : euler_t){
    if (duplicates[it])
      continue;
    ret.push_back(it);
    duplicates[it] = true;
  }

  //distance(ret);
  print_path(ret);
  return ret;
  exit(0);


}

vector<edge_des>* TSP::min_perfect_matching(vector<int>* verts, vector<edge_des> tree, int odd){
  //create a matching 
  //pick a vertex and find a matching of minimum weight
  
  vector<edge_des> *ret = new vector<edge_des>;
  auto comp = [](edge_des u, edge_des v){return u.m_eproperty < v.m_eproperty;};
  priority_queue <edge_des, vector<edge_des>, decltype(comp)> pq(comp);

  //adding every edge to the pq
  for(int i = 0; i < odd; i++){
    for (int j = i + 1; j < odd; j++){
      pq.push(int_to_edge(verts->at(i),verts->at(j)));
    }
  }

  //find the minimum edges till all vertices are perfect matched
  bool used[odd];
  for (int i = 0; i < odd; i++)
    used[i] = false;

  edge_des ed;
  for(int i = 0; !pq.empty(); i++){
    ed = pq.top();
    if (used[ed.m_source] == true || used[ed.m_target] == true) {
      pq.pop();
      continue;
    }
    used[ed.m_source] = true;
    used[ed.m_target] = true;
    ret->push_back(ed);
    pq.pop();
    
  }
  return ret;  
  
}

vector<int> TSP::hierholzer(vector<edge_des> tree){
  //needs to resemble a DFS search
  //start at a vertex and traverse down an edge till you have no other edges to travel down
  //push the order of vertices in a stack
  //when out of edges to traverse push stack to path and back track while checking for other edges

  vector<int> path;
  vector<int> ret;
  int current = 0;
  path.push_back(current);
  vector<edge_des> degree;


  while (!path.empty()){
    current = path.back();

    degree.clear();  
    //gets all the edges of the current vertex
    for(auto it : tree){
      if(it.m_target == current || it.m_source == current){
        degree.push_back(it);
      }
    }
    //if there are no edges from current
    //push to ret and pop from path
    if (degree.empty()){
      ret.push_back(current);
      path.pop_back();
    } else{
      //if there exists an edge then push current to path and 
      //remove the edge from graph
      if (degree.back().m_source == current){
        path.push_back(degree.back().m_target);
        int i =0;
        for (auto it:tree){
          if (it == degree.back()) break;
          i++;
        }
        tree.erase(tree.begin() + i);
      } else if (degree.back().m_target == current){
        path.push_back(degree.back().m_source);
        int i =0;
        for (auto it:tree){
          if (it == degree.back()) break;
          i++;
        }
        tree.erase(tree.begin() + i);
      }   
    }
  }
    return ret;

}