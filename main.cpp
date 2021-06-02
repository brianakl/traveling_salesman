#include <iostream>
#include <queue>
#include "TSP.h"
#include <chrono>

using namespace std;

int main(){

	using chrono::high_resolution_clock;
    using chrono::duration_cast;
    using chrono::duration;
    using chrono::seconds;

	TSP tsp("tsp_coordinates.txt", COORDINATES);
	vector<int> test = tsp.nearest_neighbor_path();
	double d = tsp.distance(test);

	cout << "Nearest Neighbor cycle: ";
	for (int i=0; i < tsp.get_vertices(); i++){
		cout << test[i] << " ";

	}

	cout << endl << "Distance: " << d << endl << endl;
	auto t1 = high_resolution_clock::now();
	tsp.tsp_brute();
	auto t2 = high_resolution_clock::now();
	//tsp.christofides();
	auto s_int = duration_cast<seconds>(t2-t1);
	duration<double, std::milli> ms_double = t2 - t1;

    cout << s_int.count() << "s\n";
    cout << ms_double.count() << "ms\n";

	vector<edge_des> sdf = tsp.min_perfect_matching(test,5);
	




	return 0;
	
}