#include "TSP.h"
#include <fstream>
#include <vector>


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

  j= 0;

  ifile >> n;
  ifile >> e;

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
        ewp = sqrt(pow((x2-x1),2) + pow((y2-y1),2));
        add_edge(i,j,ewp,*adj_list);
      }
    }

  }

  vertex_set = vertices(*adj_list);
  for (int i = 0; i < n; i++){
    mrk.push_back(UNVISITED);
  }


  //for the dp function a 2^n by n vector
  for(int i = 0; i < pow(2,n); i++){
    dp.push_back(vector<double>());
    path.push_back(vector<int>());
    for(int j = 0; j<n; j++){
        dp[i].push_back(-1);
        path[i].push_back(-1);
    }
  }
  
  for(int i = 0; i < n ; i++)
    parent.push_back(-1);
  parent[0] = 0;
  ifile.close();
  min = m = INFINITY;


}

void TSP::print_path(vector<int> path) {
  cout << "Cycle:" << endl;
  for (int i = 0; i < n; i++){
    cout << path[i] << " ";
  }
  cout << endl << "Total Distance: " << distance(path) << endl;
}

//recursively returns the total distance of a path
double TSP::distance(vector<int> path, int i) {

  i = path.back();
  int start = i;
  double dist = 0;
  edge_des cur;
  while(path.size() > 1){
    i = path.back();
    path.pop_back();
    dist += edge_weight_map[int_to_edge(i,path.back())];
  }
  dist += edge_weight_map[int_to_edge(start,path.back())];
  return dist;

}


edge_des TSP::int_to_edge(int v, int u){
  out_edge_pair out_p = out_edges(index_map[v], *adj_list);
  for (;out_p.first != out_p.second; out_p.first++){
    if(target(*out_p.first, *adj_list) == index_map[u]){ 
      return *out_p.first;
    }
  }
  
  return *out_p.second;
}