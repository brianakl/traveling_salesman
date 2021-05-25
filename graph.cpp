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


  if (gt == ADJ_LIST){
    adj_list g(n);
  } else {
    adj_matrix g(n);
  }
  int origin = 0, dest = 0;
  double coordinates [2][n] = 0.0;
  //coordinates input
  while(ifile >> origin && ifile >> dest){

  }

  //adjacency list input


  ifile.close();


}
