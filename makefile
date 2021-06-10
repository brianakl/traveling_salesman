tsp: main.o christofides.o graph.o nearest_neighbor.o brute_force.o
	g++ -o tsp main.o christofides.o graph.o nearest_neighbor.o brute_force.o tsp

main.o: TSP.h main.cpp graph.cpp christofides.cpp nearest_neighbor.cpp brute_force.cpp
	g++ -std=c++11 main.cpp graph.cpp christofides.cpp nearest_neighbor.cpp brute_force.cpp

graph.o: TSP.h graph.cpp
	g++ -std=c++11 graph.cpp

christofides.o: TSP.h christofides.cpp
	g++ -std=c++11 christofides.cpp

nearest_neighbor.o: TSP.h nearest_neighbor.cpp
	g++ -std=c++11 nearest_neighbor.cpp

brute_force.o: TSP.h brute_force.cpp
	g++ -std=c++11 brute_force.cpp

clean:
	rm *.o