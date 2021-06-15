tsp: main.o christofides.o graph.o nearest_neighbor.o brute_force.o dynamic_tsp.o
	g++ -o tsp main.o christofides.o graph.o nearest_neighbor.o brute_force.o tsp

main.o: TSP.h main.cpp graph.cpp christofides.cpp nearest_neighbor.cpp brute_force.cpp dynamic_tsp.cpp
	g++ -std=c++11 main.cpp graph.cpp christofides.cpp nearest_neighbor.cpp brute_force.cpp dynamic_tsp.cpp

graph.o: TSP.h graph.cpp
	g++ -std=c++11 graph.cpp

christofides.o: TSP.h christofides.cpp graph.cpp main.cpp
	g++ -std=c++11 christofides.cpp graph.cpp main.cpp

nearest_neighbor.o: TSP.h nearest_neighbor.cpp graph.cpp
	g++ -std=c++11 nearest_neighbor.cpp graph.cpp

brute_force.o: TSP.h brute_force.cpp
	g++ -std=c++11 brute_force.cpp

dynamic_tsp.o: TSP.h dynamic_tsp.cpp
	g++ -std=c++11 dynamic_tsp.cpp

clean:
	rm *.o