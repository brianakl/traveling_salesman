#include <iostream>
#include "TSP.hpp"

using namespace std;

int main(){

	TSP tsp("tsp_distance_matrix.txt");
	stack<double> d = tsp.tsp_brute_force();



	return 0;
	
}