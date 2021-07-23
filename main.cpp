#include <iostream>
#include "TSP.h"
#include <fstream>

using namespace std;

int main(){

	srand(time(0));


	cout << "Welcome to the tsp solver" << endl;

	string filename;
	int itype = -1;
	ifstream ifile;

	while (true){
		cout << "Please enter a txt file name:	";

		cin >> filename;
		ifile.open(filename);
		if (!ifile){
			cout << "File name: " << filename << " does not exist" << endl;
			continue;
		} else break;
	}
	ifile.close();

	while (itype < 0 || itype > 1){
		cout << "Please enter the file enter 1 for coordinates or 0 for distance matrix:	";
		
		cin >> itype;

		if (itype < 0 || itype > 1){
			cout << "Please enter either 0 or 1 " << itype << endl;
		}
	}

	TSP tsp(filename, static_cast<input_type>(itype));

	int in = -1;

	while(in != 0) {
		cout << "\nEnter which algorithm you'd like to run on the data" << endl
			 << "1. Nearest Neighbor" << endl
			 << "2. Brute Force" << endl
			 << "3. Christofides" << endl
			 << "4. Dynamic Programming" << endl
			 << "5. Genetic Algorithm" << endl
			 << "6. EXIT" << endl;

		cin >> in;
		vector<int> temp;

		switch (in){

			case 6:
				exit(0);
				break;
			case 1:
				cout << "Nearest Neighbor"<< endl;
				tsp.nearest_neighbor_path();
				break;
			case 2:
				cout << "Brute Force: "<< endl;
				tsp.tsp_brute();
				break;
			case 3:
				cout << "Christofides" << endl;
				tsp.christofides();
				break;
			case 4:
				cout << "Dynamic Programming" << endl;
				tsp.dynamic_tsp();
				break;
			case 5:
				cout << "Genetic Algorithm" << endl;
				tsp.genetic_starter();
				break;		
			default:
				cout << "Please enter numbers 1-6";
				break;
		}

		
	}


	return 0;
	
}