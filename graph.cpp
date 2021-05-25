#include "graph.h"
#include "boost/graph/adjacency_list.hpp"
#include "boost/graph/adjacency_matrix.hpp"
#include <iostream>
#include <string>
#include <ifstream>

using namespace std;
using namespace boost;

Graph::Graph(const string& fileName, graph_type gt){
  ifstream ifile;
  ifile.open(fileName);
  if (!ifile){
    cout << "File name: " << fileName << " does not exist" << endl;
    exit(1);
  }

  ifile >> n;
  ifile >> e;


  adj_list Graph(n);
  double matrix [n][n] = 0.0;

  for(int i = 0; i < n; i++){
    for(int j = 0; j < n; j++){
      ifile >> matrix[i][j];
    }
  }

  //add all verticies
  for (int i =0; i < n; i++){
    add_vertex(i,g);
  }
  //add edges with weights
  for (int i = 1; i < n; i++){
    for (int j = 0; j < n; j++){
      if (matrix[i][j] == 0.0) break;
      add_edge(i,j, matrix[i][j], Graph)
    }
  }



  ifile.close();


}
