#include <iostream>
#include "TSP.h"
#include <chrono>

using namespace std;

int main(){

	using chrono::high_resolution_clock;
    using chrono::duration_cast;
    using chrono::duration;
    using chrono::seconds;
	srand(time(0));

	TSP tsp("lp_test.txt", DIST_MATRIX);
	vector<int> test = tsp.nearest_neighbor_path();
	double d = tsp.distance(test);

	cout << "Nearest Neighbor cycle: \n";
	for (int i=0; i < tsp.get_vertices(); i++){
		cout << test[i] << " ";

	}
	cout << endl << "Distance: " << d << endl << endl << "Brute Force:"<< endl;

	auto t1 = high_resolution_clock::now();
	//tsp.tsp_brute();
	auto t2 = high_resolution_clock::now();
	auto s_int = duration_cast<seconds>(t2-t1);
	duration<double, std::milli> ms_double = t2 - t1;

    //cout << s_int.count() << "s\n";
    //cout << ms_double.count() << "ms\n";
	

	cout << endl << endl << "Christofides: ";
	vector<int> sdf = tsp.christofides();

	t1 = high_resolution_clock::now();
	//double dtsp = tsp.dynamic_tsp();
	//cout << "\n\nDynamic solution distance: " << dtsp << endl;
	t2 = high_resolution_clock::now();
	s_int = duration_cast<seconds>(t2-t1);
	ms_double = t2-t1;
	//cout << s_int.count() << "s\n";
    //cout << ms_double.count() << "ms\n";
	//tsp.print_dtsp();

	cout << endl;
	tsp.genetic_starter();

	




	return 0;
	
}