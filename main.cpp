#include <iostream>
#include <queue>
#include "TSP.h"

using namespace std;

int main(){

	TSP tsp("test_distance_matrix.txt");
	//stack<double> d = tsp.tsp_brute_force();
	vector<int> test = tsp.nearest_neighbor_path();
	double d = tsp.distance(test);

	cout << "Nearest Neighbor cycle: ";
	for (int i=0; i < tsp.get_vertices(); i++){
		cout << test[i] << " ";

	}

	cout << endl << "Distance: " << d << endl << endl;

	tsp.tsp_brute();




	return 0;
	
}