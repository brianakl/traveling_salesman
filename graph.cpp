#include "graph.h"

#include <iostream>
#include <string>
#include <ifstream>

Graph::Graph(const string& fileName, graph_type gt){
  ifstream ifile;
  ifile.open(fileName);
  if (!ifile){
    cout << "File name: " << fileName << " does not exist" << endl;
    exit(1);
  }

  ifile >> n;
  ifile >> m;

  if (gt == ADJ_LIST){
    adj_list g(n);
  } else {
    adj_matrix g(n);
  }

  ifile.close();


}
