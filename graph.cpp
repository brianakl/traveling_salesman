#include "TSP.hpp"
#include <boost/graph/adjacency_list.hpp>
#include <iostream>
#include <string>
#include <fstream>
#include <utility>

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


  Graph adj_list(n);
  double matrix [this->n][this->n];

  for(int i = 0; i < this->n; i++){
    for(int j = 0; j < this->n; j++){
      ifile >> matrix[i][j];
    }
  }
  //boost::adjacency_list<boost::listS, boost::vecS, boost::undirectedS, boost::no_property, EdgeWeightProperty> adj_list(this->n);

  //add all verticies
  /*
  for (int i =0; i < this->n; i++){
    boost::add_vertex(i,adj_list);
  }
  */

  //add edges with weights
  for (int i = 1; i < this->n; i++){
    for (int j = 0; j < this->n; j++){
      if (matrix[i][j] == 0.0) break;
      add_edge(i,j, matrix[i][j], adj_list); //adds an edge between vertex i and j of weight matrix[i][j] to the graph
    }
  }



  ifile.close();


}
