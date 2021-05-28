#include <iostream>
#include <queue>
#include "TSP.hpp"

using namespace std;

int main(){

	TSP tsp("tsp_distance_matrix.txt");
	//stack<double> d = tsp.tsp_brute_force();
	vector<int> test = tsp.nearest_neighbor_path();
	double d = tsp.distance(test);

	cout << "Nearest Neighbor path: ";
	for (int i=0; i < 5; i++){
		cout << test[i] << " ";

	}
	cout << endl;
	cout << endl << "Distance: " << d << endl;




	return 0;
	
}