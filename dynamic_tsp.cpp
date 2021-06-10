#include "TSP.h"


using namespace std;

vector<int> TSP::dynamic_tsp(){
    vector<dynamic_bitset<> > mem(n,dynamic_bitset<>(pow(2,n)));
    vector<int> ret;
    setup(mem,0);
    solve(mem,0);

    auto minCost = findMinCost(mem,0);
    ret = findOptimalTour(mem,0);

    return ret;
}


void TSP::setup(vector<dynamic_bitset<> > &mem,int start){
    for (int i =0; i<n; i++){
        if (i == start) continue;
        mem[i][1 << start | 1 << i] = edge_weight_map[int_to_edge(start,i)];
    }
}